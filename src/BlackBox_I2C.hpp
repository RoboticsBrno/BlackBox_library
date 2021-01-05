/**
 * @file BlackBox_I2C.hpp
 * @author Tomáš Rohlínek (haberturdeur)
 * @brief library for working with I2C
 * 
 * @copyright Copyright (c) 2020 RoboticsBrno (RobotikaBrno)
 */
#pragma once

#include "driver/i2c.h"
#include <atomic>
#include <cstdint>
#include <mutex>

namespace I2C {

/**
 * @brief RAII wrapper for ESP-IDFs i2c commands
 */
class Transmission {
private:
    Transmission(const Transmission&) = delete;
    Transmission& operator=(const Transmission&) = delete;

    mutable std::recursive_mutex m_mutex;
    const char* m_tag = "I2C_Transmission";

    i2c_cmd_handle_t m_handle;
    bool m_sent = false;

public:
    /**
     * @brief Construct a new clean Transmission object
     * 
     */
    Transmission();

    /**
     * @brief Construct a new Transmission object from existing handle
     * 
     */
    Transmission(i2c_cmd_handle_t const);

    Transmission(Transmission&&) = default;
    Transmission& operator=(Transmission&&) = default;

    ~Transmission();

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
    esp_err_t writeByte(std::uint8_t data, i2c_ack_type_t ackType = I2C_MASTER_ACK);

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
    esp_err_t readByte(std::uint8_t* data, i2c_ack_type_t ackType = I2C_MASTER_ACK);

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
    esp_err_t write(std::uint8_t* data, size_t dataLength, i2c_ack_type_t ackType = I2C_MASTER_ACK);

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
    esp_err_t read(std::uint8_t* data, size_t dataLength, i2c_ack_type_t ackType = I2C_MASTER_ACK);

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
class Device {
protected:
    std::uint16_t m_address;

    Device(std::uint16_t address);
    
public:
    virtual ~Device() = default;

    /**
     * @brief Returns address of I2C device specified on inicialization
     * @return address 
     */
    std::uint16_t address();
};

namespace Ports {
constexpr char const* tag = "I2C_Port_Guard";

static std::atomic<bool> initializedPorts[I2C_NUM_MAX];

/**
 * @brief Initilaize given I2C port.
 * 
 * @param port 
 * @param config 
 * @param slaveRxBuffer 
 * @param slaveTxBuffer 
 * @param intrAllockationFlag 
 */
void init(i2c_port_t, i2c_config_t, size_t slaveRxBuffer = 0, size_t slaveTxBuffer = 0, int intrAllockationFlag = 0);

/**
 * @brief Configure given I2C port.
 *        @note
 *        The port must already be initialized otherwise the configuration won't happen
 * 
 * @param port 
 * @param config 
 */
void config(i2c_port_t port, i2c_config_t config);

/**
 * @brief Deinitialize given I2C port.
 *        @note
 *        The port must already be initialized
 * 
 * @param port
 */
void deinit(i2c_port_t);

/**
 * @brief Returns whether or not is given I2C port initialized
 * 
 * @return initialization state
 */
bool isInitialized(i2c_port_t);
};

}