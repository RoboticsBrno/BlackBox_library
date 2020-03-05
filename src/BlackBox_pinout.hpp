#pragma once

#include "esp_system.h"

namespace BlackBox
{
    static const int LED_COUNT = 60;
    static const gpio_num_t LED_DATA_GPIO = GPIO_NUM_14;
    static const gpio_num_t LED_TOOGLE_GPIO = GPIO_NUM_12;
    static const int CHANNEL = 0;

    static const gpio_num_t SDA_GPIO = GPIO_NUM_23;
    static const gpio_num_t SCL_GPIO = GPIO_NUM_22;

    static const gpio_num_t RTC_ALARM_INT_GPIO = GPIO_NUM_21;
    static const gpio_num_t RTC_SQW_GPIO = GPIO_NUM_21;
    static const gpio_num_t QMC_INT_GPIO = GPIO_NUM_19;
    static const gpio_num_t MPU_INT_GPIO = GPIO_NUM_25;

    static const gpio_num_t RTC_RST_GPIO = GPIO_NUM_15;

    static const gpio_num_t ENC_SW_GPIO = GPIO_NUM_18;
    static const gpio_num_t ENC_A_GPIO = GPIO_NUM_5;
    static const gpio_num_t ENC_B_GPIO = GPIO_NUM_17;

    static const gpio_num_t GPS_RX_GPIO = GPIO_NUM_16;
    static const gpio_num_t GPS_TX_GPIO = GPIO_NUM_4;

    static const gpio_num_t ESP_SHUTDOWN_GPIO = GPIO_NUM_0;

    static const gpio_num_t RS485_TX_GPIO = GPIO_NUM_2;
    static const gpio_num_t RS485_RX_GPIO = GPIO_NUM_27;
    static const gpio_num_t RS485_DTR_GPIO = GPIO_NUM_13;

    static const gpio_num_t LOCK_GPIO = GPIO_NUM_26;
} // namespace BlackBox