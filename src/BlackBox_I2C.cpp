#include "BlackBox_I2C.hpp"

#include "esp_log.h"

#include <mutex>
#include <stdint.h>

namespace BlackBox {
I2CTransmission::I2CTransmission()
    : m_handle(i2c_cmd_link_create()) {
}

I2CTransmission::I2CTransmission(i2c_cmd_handle_t const i_handle)
    : m_handle(i_handle) {
}

I2CTransmission::~I2CTransmission() {
    std::scoped_lock lock(m_mutex);
    i2c_cmd_link_delete(m_handle);
    m_handle = NULL;
}

esp_err_t I2CTransmission::startBit() {
    std::scoped_lock lock(m_mutex);
    return i2c_master_start(m_handle);
}

esp_err_t I2CTransmission::stopBit() {
    std::scoped_lock lock(m_mutex);
    return i2c_master_stop(m_handle);
}

esp_err_t I2CTransmission::writeByte(uint8_t i_data, i2c_ack_type_t i_ackType) {
    std::scoped_lock lock(m_mutex);
    return i2c_master_write_byte(m_handle, i_data, i_ackType);
}

esp_err_t I2CTransmission::readByte(uint8_t* o_data, i2c_ack_type_t i_ackType) {
    return i2c_master_read_byte(m_handle, o_data, i_ackType);
}

esp_err_t I2CTransmission::write(uint8_t* i_data, size_t i_dataLengt, i2c_ack_type_t i_ackType) {
    std::scoped_lock lock(m_mutex);
    return i2c_master_write(m_handle, i_data, i_dataLengt, i_ackType);
}

esp_err_t I2CTransmission::read(uint8_t* o_data, size_t i_dataLengt, i2c_ack_type_t i_ackType) {
    std::scoped_lock lock(m_mutex);
    return i2c_master_read(m_handle, o_data, i_dataLengt, i_ackType);
}

esp_err_t I2CTransmission::send(i2c_port_t i_i2cNum, TickType_t i_ticksToWait) {
    std::scoped_lock lock(m_mutex);
    m_sent = true;
    return i2c_master_cmd_begin(i_i2cNum, m_handle, i_ticksToWait);
}

void I2CTransmission::reset() {
    std::scoped_lock lock(m_mutex);
    if (!m_sent)
        ESP_LOGW(m_tag, "Reseting without sending data!");
    i2c_cmd_link_delete(m_handle);
    m_handle = i2c_cmd_link_create();
    m_sent = false;
}

i2c_cmd_handle_t I2CTransmission::raw() {
    std::scoped_lock lock(m_mutex);
    return m_handle;
}

void I2CTransmission::detach() {
    std::scoped_lock lock(m_mutex);
    m_handle = NULL;
}

I2CDevice::I2CDevice(uint16_t i_address)
    : m_address(i_address) {
}

uint16_t I2CDevice::address() {
    return m_address;
}

void I2CPorts::init(i2c_port_t i_port, i2c_config_t i_config, size_t i_slaveRxBuffer, size_t i_slaveTxBuffer, int i_intrAllockationFlag) {
    if (s_mutexes[i_port].try_lock()) {
        ESP_ERROR_CHECK(i2c_param_config(i_port, &i_config));
        ESP_ERROR_CHECK(i2c_driver_install(i_port, i_config.mode, i_slaveRxBuffer, i_slaveTxBuffer, i_intrAllockationFlag));
    }
}

void I2CPorts::config(i2c_port_t i_port, i2c_config_t i_config) {
    if (isInitialized(i_port)) {
        ESP_ERROR_CHECK(i2c_param_config(i_port, &i_config));
    } else {
        s_mutexes[i_port].unlock(); // oops
        constexpr char const* tag = "I2C_Port_Guard";
        ESP_LOGE(tag, "Trying configuring without init on port: %i", i_port);
    }
}

void I2CPorts::deinit(i2c_port_t i_port) {
    if (isInitialized(i_port)) {
        ESP_ERROR_CHECK(i2c_driver_delete(i_port));
        s_mutexes[i_port].unlock();
    } else {
        constexpr char const* tag = "I2C_Port_Guard";
        ESP_LOGE(tag, "Trying to deinit without init on port: %i", i_port);
    }
}

bool I2CPorts::isInitialized(i2c_port_t i_port) {
    if (s_mutexes[i_port].try_lock()) {
        s_mutexes[i_port].unlock(); // oops
        return false;
    }
    return true;
}
}