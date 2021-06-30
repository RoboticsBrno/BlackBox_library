#include "Dsp.hpp"
#ifdef BB_I2C

#include "library/BlackBox_I2C.hpp"

#include "esp_log.h"
#include <cstdint>
#include <mutex>

namespace I2C {
Transmission::Transmission()
    : m_handle(i2c_cmd_link_create()) {
}

Transmission::Transmission(i2c_cmd_handle_t i_handle)
    : m_handle(std::exchange(i_handle, nullptr)) {
}

Transmission::Transmission(Transmission&& other) {
    std::scoped_lock lock(other.m_mutex);
    m_handle = std::exchange(other.m_handle, nullptr);
    m_sent = std::exchange(other.m_sent, true);
}

Transmission& Transmission::operator=(Transmission&& other) {
    if (this != &other) {
        std::scoped_lock lock(other.m_mutex, m_mutex);
        if (m_handle != nullptr) {
            i2c_cmd_link_delete(m_handle);
            m_handle = nullptr;
        }
        m_handle = std::exchange(other.m_handle, nullptr);
        m_sent = std::exchange(other.m_sent, true);
    }
    return *this;
}

Transmission::~Transmission() {
    std::scoped_lock lock(m_mutex);
    if (m_handle != nullptr) {
        i2c_cmd_link_delete(m_handle);
        m_handle = nullptr;
    }
}

esp_err_t Transmission::startBit() {
    std::scoped_lock lock(m_mutex);
    return i2c_master_start(m_handle);
}

esp_err_t Transmission::stopBit() {
    std::scoped_lock lock(m_mutex);
    return i2c_master_stop(m_handle);
}

esp_err_t Transmission::writeByte(std::uint8_t i_data, bool i_ackCheck) {
    std::scoped_lock lock(m_mutex);
    return i2c_master_write_byte(m_handle, i_data, i_ackCheck);
}

esp_err_t Transmission::readByte(std::uint8_t* o_data, ACKValue i_ackValue) {
    return i2c_master_read_byte(m_handle, o_data, static_cast<i2c_ack_type_t>(i_ackValue));
}

esp_err_t Transmission::write(std::uint8_t* i_data, size_t i_dataLength, bool i_ackCheck) {
    std::scoped_lock lock(m_mutex);
    return i2c_master_write(m_handle, i_data, i_dataLength, i_ackCheck);
}

esp_err_t Transmission::read(std::uint8_t* o_data, size_t i_dataLength, ACKValue i_ackValue) {
    std::scoped_lock lock(m_mutex);
    return i2c_master_read(m_handle, o_data, i_dataLength, static_cast<i2c_ack_type_t>(i_ackValue));
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

constexpr Port::Port(int i_port)
    : m_port(i_port)
    , m_config(defaultConfig) {
}

void Port::init(i2c_port_t i_port, const i2c_config_t& i_config, size_t i_slaveRxBuffer, size_t i_slaveTxBuffer, int i_intrAllockationFlag) {
    bool expected = false;
    if (!s_initializedPorts[i_port].compare_exchange_strong(expected, true))
        return;
    ESP_ERROR_CHECK(i2c_param_config(i_port, &i_config));
    ESP_ERROR_CHECK(i2c_driver_install(i_port, i_config.mode, i_slaveRxBuffer, i_slaveTxBuffer, i_intrAllockationFlag));
    stop(i_port);
}

void Port::config(i2c_port_t i_port, const i2c_config_t& i_config) {
    if (isInitialized(i_port)) {
        ESP_ERROR_CHECK(i2c_param_config(i_port, &i_config));
    } else {
        ESP_LOGE(s_tag, "Trying configuring without init on port: %i", i_port);
    }
}

void Port::deinit(i2c_port_t i_port) {
    if (isInitialized(i_port)) {
        ESP_ERROR_CHECK(i2c_driver_delete(i_port));
        s_initializedPorts[i_port].store(false);
    } else {
        ESP_LOGE(s_tag, "Trying to deinit without init on port: %i", i_port);
    }
}

bool Port::isInitialized(i2c_port_t i_port) {
    return s_initializedPorts[i_port].load();
}

void Port::sendSoftwareReset(i2c_port_t i_port) {
    I2C::Transmission transmission;
    transmission.startBit();
    transmission.writeByte(0, false);
    transmission.writeByte(0b00000110, false);
    transmission.stopBit();
    transmission.send(i_port);
}

void Port::stop(i2c_port_t i_port) {
    I2C::Transmission transmission;
    transmission.stopBit();
    transmission.send(i_port);
}

void Port::init(const i2c_config_t& i_config, size_t i_slaveRxBuffer, size_t i_slaveTxBuffer, int i_intrAllocationFlag) {
    init(m_port, i_config, i_slaveRxBuffer, i_slaveTxBuffer, i_intrAllocationFlag);
}

void Port::config(const i2c_config_t& i_config) {
    config(m_port, i_config);
}

void Port::deinit() {
    deinit(m_port);
}

bool Port::isInitialized() {
    return isInitialized(m_port);
}

void Port::sendSoftwareReset() {
    sendSoftwareReset(m_port);
}

void Port::stop() {
    stop(m_port);
}

constexpr i2c_port_t Port::port() {
    return m_port;
}

constexpr Port::operator i2c_port_t() const {
    return m_port;
}

std::array<std::atomic<bool>, I2C_NUM_MAX> Port::s_initializedPorts = {};

Device::Device(std::uint16_t i_address, Port i_port)
    : m_address(i_address)
    , m_port(i_port) {
}

Device::Device(std::uint16_t i_address, i2c_port_t i_port)
    : m_address(i_address)
    , m_port(i_port) {
}

uint8_t Device::readByte(std::uint8_t i_registerAddress) {
    std::uint8_t data;
    I2C::Transmission transmission;
    transmission.startBit();
    transmission.writeByte((m_address << 1) | I2C_MASTER_WRITE, EnableACKCheck);
    transmission.writeByte(i_registerAddress, EnableACKCheck);
    transmission.startBit();
    transmission.writeByte((m_address << 1) | I2C_MASTER_READ, EnableACKCheck);
    transmission.readByte(&data, NACK);
    transmission.stopBit();
    transmission.send(m_port);
    return data;
}

void Device::readBytes(std::uint8_t i_registerAddress, std::uint8_t* o_data, size_t i_dataLength) {
    I2C::Transmission transmission;
    transmission.startBit();
    transmission.writeByte((m_address << 1) | I2C_MASTER_WRITE, EnableACKCheck);
    transmission.writeByte(i_registerAddress, EnableACKCheck);
    transmission.startBit();
    transmission.writeByte((m_address << 1) | I2C_MASTER_READ, EnableACKCheck);
    transmission.read(o_data, i_dataLength, LastNACK);
    transmission.stopBit();
    transmission.send(m_port);
}

std::uint16_t Device::readWord(std::uint8_t i_registerAddress) {
    I2C::Transmission transmission;
    std::uint8_t data[] = { 0, 0 };
    transmission.startBit();
    transmission.writeByte((m_address << 1) | I2C_MASTER_WRITE, EnableACKCheck);
    transmission.writeByte(i_registerAddress, EnableACKCheck);
    transmission.startBit();
    transmission.writeByte((m_address << 1) | I2C_MASTER_READ, EnableACKCheck);
    transmission.read(data, 2, LastNACK);
    transmission.stopBit();
    transmission.send(m_port);
    return ((data[0] << 8) | data[1]);
}

void Device::writeByte(std::uint8_t i_registerAddress, std::uint8_t i_data) {
    I2C::Transmission transmission;
    transmission.startBit();
    transmission.writeByte((m_address << 1) | I2C_MASTER_WRITE, EnableACKCheck);
    transmission.writeByte(i_registerAddress, EnableACKCheck);
    transmission.writeByte(i_data, EnableACKCheck);
    transmission.stopBit();
    transmission.send(m_port);
}

void Device::writeBytes(std::uint8_t i_registerAddress, std::uint8_t* i_data, size_t i_dataLength) {
    I2C::Transmission transmission;
    transmission.startBit();
    transmission.writeByte((m_address << 1) | I2C_MASTER_WRITE, EnableACKCheck);
    transmission.writeByte(i_registerAddress, EnableACKCheck);
    transmission.write(i_data, i_dataLength, EnableACKCheck);
    transmission.stopBit();
    transmission.send(m_port);
}

void Device::writeWord(std::uint8_t i_registerAddress, std::uint16_t i_data) {
    I2C::Transmission transmission;
    std::uint8_t* data = reinterpret_cast<std::uint8_t*>(&i_data);
    std::swap(data[0], data[1]);
    transmission.startBit();
    transmission.writeByte((m_address << 1) | I2C_MASTER_WRITE, EnableACKCheck);
    transmission.writeByte(i_registerAddress, EnableACKCheck);
    transmission.write(data, 2, EnableACKCheck);
    transmission.stopBit();
    transmission.send(m_port);
}

std::uint16_t Device::address() const {
    return m_address;
}

Port Device::port() const {
    return m_port;
}

void Device::init() {
    m_port.init();
}
}
#endif
