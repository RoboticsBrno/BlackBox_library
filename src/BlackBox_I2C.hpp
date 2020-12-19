/**
 * @file BlackBox_I2C.hpp
 * @author Tomáš Rohlínek (haberturdeur)
 * @brief library for working with I2C
 * 
 * @copyright Copyright (c) 2020 RoboticsBrno (RobotikaBrno)
 */
#pragma once

#include "driver/i2c.h"
#include <mutex>
#include <stdint.h>

namespace BlackBox {

/**
 * @brief RAII wrapper for ESP-IDFs i2c commands
 */
class I2CTransmission {
private:
    I2CTransmission(const I2CTransmission&) = delete;
    I2CTransmission& operator=(const I2CTransmission&) = delete;

    mutable std::recursive_mutex m_mutex;
    const char* m_tag = "I2C_Transmission";

    i2c_cmd_handle_t m_handle;
    bool m_sent = false;

public:
    /**
     * @brief Construct a new clean I2CTransmission object
     * 
     */
    I2CTransmission();

    /**
     * @brief Construct a new I2CTransmission object from existing handle
     * 
     */
    I2CTransmission(i2c_cmd_handle_t const);

    I2CTransmission(I2CTransmission&&) = default;
    I2CTransmission& operator=(I2CTransmission&&) = default;

    ~I2CTransmission();

    /**
     * @brief Queue start bit into transmission
     * 
     * @return
     *     - ESP_OK Success
     *     - ESP_ERR_INVALID_ARG Parameter error
     */
    esp_err_t startBit();

    /**
     * @brief Queue stop bit into transmission
     * 
     * @return
     *     - ESP_OK Success
     *     - ESP_ERR_INVALID_ARG Parameter error
     */
    esp_err_t stopBit();

    /**
     * @brief Add write of 1 byte into transmission 
     * 
     * @param data data byte to write
     * @param ackType enable ack checks
     * 
     * @return
     *     - ESP_OK Success
     *     - ESP_ERR_INVALID_ARG Parameter error
     */
    esp_err_t writeByte(uint8_t data, i2c_ack_type_t ackType = I2C_MASTER_ACK);

    /**
     * @brief Add read of 1 byte into transmission
     * 
     * @param data buffer to put read data into
     * @param ackType ack value for read command
     * 
     * @return
     *     - ESP_OK Success
     *     - ESP_ERR_INVALID_ARG Parameter error
     */
    esp_err_t readByte(uint8_t* data, i2c_ack_type_t ackType = I2C_MASTER_ACK);

    /**
     * @brief Add write of buffer into transmission 
     * 
     * @param data data byte to write
     * @param dataLength length of data
     * @param ackType enable ack checks
     * 
     * @return
     *     - ESP_OK Success
     *     - ESP_ERR_INVALID_ARG Parameter error
     */
    esp_err_t write(uint8_t* data, size_t dataLength, i2c_ack_type_t ackType = I2C_MASTER_ACK);

    /**
     * @brief Add read of buffer into transmission
     * 
     * @param data buffer to put read data into
     * @param dataLength length of data
     * @param ackType ack value for read command
     * 
     * @return
     *     - ESP_OK Success
     *     - ESP_ERR_INVALID_ARG Parameter error
     */
    esp_err_t read(uint8_t* data, size_t dataLength, i2c_ack_type_t ackType = I2C_MASTER_ACK);

    /**
     * @brief Send queued commands
     * 
     * @param i2cNum I2C port
     * @param ticksToWait maximum wait ticks
     * 
     * @return
     *     - ESP_OK Success
     *     - ESP_ERR_INVALID_ARG Parameter error
     *     - ESP_FAIL Sending command error, slave doesn't ACK the transfer.
     *     - ESP_ERR_INVALID_STATE I2C driver not installed or not in master mode.
     *     - ESP_ERR_TIMEOUT Operation timeout because the bus is busy.
     */
    esp_err_t send(i2c_port_t i2cNum, TickType_t ticksToWait);

    /**
     * @brief Reset this transmission for future use
     */
    void reset();

    /**
     * @brief Get the raw handle to I2C_cmd
     * 
     * @return raw handle
     */
    i2c_cmd_handle_t raw();

    /**
     * @brief Detach the RAII object from i2c_cmd_handle
     * 
     */
    void detach();
};

/**
 * @brief Base class for I2C devices
 */
class I2CDevice {
private:
    uint16_t m_address;

public:
    I2CDevice(uint16_t address);
    ~I2CDevice() = default;

    /**
     * @brief Returns address of I2C device specified on inicialization
     * 
     * @return address 
     */
    uint16_t address();
};
}