#include "BlackBox_I2C.hpp"

#include "esp_log.h"

#include <mutex>
#include <cstdint>

namespace I2C {
Transmission::Transmission()
    : m_handle(i2c_cmd_link_create()) {
}

Transmission::Transmission(i2c_cmd_handle_t const i_handle)
    : m_handle(i_handle) {
}

Transmission::~Transmission() {
    std::scoped_lock lock(m_mutex);
    i2c_cmd_link_delete(m_handle);
    m_handle = NULL;
}

esp_err_t Transmission::startBit() {
    std::scoped_lock lock(m_mutex);
    return i2c_master_start(m_handle);
}

esp_err_t Transmission::stopBit() {
    std::scoped_lock lock(m_mutex);
    return i2c_master_stop(m_handle);
}

esp_err_t Transmission::writeByte(std::uint8_t i_data, i2c_ack_type_t i_ackType) {
    std::scoped_lock lock(m_mutex);
    return i2c_master_write_byte(m_handle, i_data, i_ackType);
}

esp_err_t Transmission::readByte(std::uint8_t* o_data, i2c_ack_type_t i_ackType) {
    return i2c_master_read_byte(m_handle, o_data, i_ackType);
}

esp_err_t Transmission::write(std::uint8_t* i_data, size_t i_dataLengt, i2c_ack_type_t i_ackType) {
    std::scoped_lock lock(m_mutex);
    return i2c_master_write(m_handle, i_data, i_dataLengt, i_ackType);
}

esp_err_t Transmission::read(std::uint8_t* o_data, size_t i_dataLengt, i2c_ack_type_t i_ackType) {
    std::scoped_lock lock(m_mutex);
    return i2c_master_read(m_handle, o_data, i_dataLengt, i_ackType);
}

esp_err_t Transmission::send(i2c_port_t i_i2cNum, TickType_t i_ticksToWait) {
    std::scoped_lock lock(m_mutex);
    m_sent = true;
    return i2c_master_cmd_begin(i_i2cNum, m_handle, i_ticksToWait);
}

void Transmission::reset() {
    std::scoped_lock lock(m_mutex);
    if (!m_sent)
        ESP_LOGW(m_tag, "Reseting without sending data!");
    i2c_cmd_link_delete(m_handle);
    m_handle = i2c_cmd_link_create();
    m_sent = false;
}

i2c_cmd_handle_t Transmission::raw() {
    std::scoped_lock lock(m_mutex);
    return m_handle;
}

void Transmission::detach() {
    std::scoped_lock lock(m_mutex);
    m_handle = NULL;
}

Device::Device(std::uint16_t i_address)
    : m_address(i_address) {
}

std::uint16_t Device::address() {
    return m_address;
}

namespace Ports {
void init(i2c_port_t i_port, i2c_config_t i_config, size_t i_slaveRxBuffer, size_t i_slaveTxBuffer, int i_intrAllockationFlag) {
    if (mutexes[i_port].try_lock()) {
        ESP_ERROR_CHECK(i2c_param_config(i_port, &i_config));
        ESP_ERROR_CHECK(i2c_driver_install(i_port, i_config.mode, i_slaveRxBuffer, i_slaveTxBuffer, i_intrAllockationFlag));
    }
}

void config(i2c_port_t i_port, i2c_config_t i_config) {
    if (isInitialized(i_port)) {
        ESP_ERROR_CHECK(i2c_param_config(i_port, &i_config));
    } else {
        mutexes[i_port].unlock(); // oops
        ESP_LOGE(tag, "Trying configuring without init on port: %i", i_port);
    }
}

void deinit(i2c_port_t i_port) {
    if (isInitialized(i_port)) {
        ESP_ERROR_CHECK(i2c_driver_delete(i_port));
        mutexes[i_port].unlock();
    } else {
        ESP_LOGE(tag, "Trying to deinit without init on port: %i", i_port);
    }
}

bool isInitialized(i2c_port_t i_port) {
    if (mutexes[i_port].try_lock()) {
        mutexes[i_port].unlock(); // oops
        return false;
    }
    return true;
}
}
}