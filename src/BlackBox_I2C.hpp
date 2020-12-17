#pragma once

#include "driver/i2c.h"
#include <mutex>

namespace BlackBox {
class I2CTransmission {
private:
    I2CTransmission(const I2CTransmission&) = delete;
    I2CTransmission& operator=(const I2CTransmission&) = delete;

    i2c_cmd_handle_t m_handle;
    std::recursive_mutex m_mutex;
    bool m_sent;

public:
    I2CTransmission();
    I2CTransmission(i2c_cmd_handle_t* const);
    I2CTransmission(I2CTransmission&&) = default;
    I2CTransmission& operator=(I2CTransmission&&) = default;

    ~I2CTransmission();

    esp_err_t start(uint8_t i_address);
    esp_err_t write(const uint8_t* const i_data, size_t i_dataLengt, i2c_ack_type_t i_ackType);
    esp_err_t read(const uint8_t* const i_data, size_t i_dataLengt, i2c_ack_type_t i_ackType);
    esp_err_t stop();
    esp_err_t send(bool i_noStopBit = 0);
    void reset(bool i_send = 0);

    i2c_cmd_handle_t* raw();
    void detach();
};
}