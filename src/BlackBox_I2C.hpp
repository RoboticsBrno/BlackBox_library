#pragma once

#include "driver/i2c.h"
#include <mutex>

namespace BlackBox {
class I2CTransmission {
private:
    I2CTransmission(const I2CTransmission&) = delete;
    I2CTransmission& operator=(const I2CTransmission&) = delete;

    mutable std::recursive_mutex m_mutex;
    const char* m_tag = "I2C_Transmission";

    i2c_cmd_handle_t m_handle;
    bool m_sent = false;

public:
    I2CTransmission();
    I2CTransmission(i2c_cmd_handle_t const);
    I2CTransmission(I2CTransmission&&) = default;
    I2CTransmission& operator=(I2CTransmission&&) = default;

    ~I2CTransmission();

    esp_err_t startBit();
    esp_err_t stopBit();

    esp_err_t writeByte(uint8_t data, i2c_ack_type_t ackType = I2C_MASTER_ACK);
    esp_err_t readByte(uint8_t* data, i2c_ack_type_t ackType = I2C_MASTER_ACK);

    esp_err_t write(uint8_t* data, size_t dataLength, i2c_ack_type_t ackType = I2C_MASTER_ACK);
    esp_err_t read(uint8_t* data, size_t dataLength, i2c_ack_type_t ackType = I2C_MASTER_ACK);

    esp_err_t send(i2c_port_t i2cNum, TickType_t ticksToWait);

    void reset();
    i2c_cmd_handle_t raw();
    void detach();
};
}