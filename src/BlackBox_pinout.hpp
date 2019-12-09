#pragma once

namespace BlackBox
{
    const int LED_COUNT = 60;
    const int DATA_PIN = 14;
    const int TOOGLE_PIN = 12;
    const int CHANNEL = 0;


    static const gpio_num_t SDA_GPIO = GPIO_NUM_23;
    static const gpio_num_t SCL_GPIO = GPIO_NUM_22;

    static const gpio_num_t INT_SQW = GPIO_NUM_21;
    static const gpio_num_t INT_QMC = GPIO_NUM_19;
    static const gpio_num_t INT_MPU = GPIO_NUM_25;

    static const gpio_num_t RST_RTC = GPIO_NUM_15;

    static const gpio_num_t SW0 = GPIO_NUM_18;
    static const gpio_num_t ENC_A = GPIO_NUM_5;
    static const gpio_num_t ENC_B = GPIO_NUM_17;

    static const gpio_num_t RXGPS = GPIO_NUM_16;
    static const gpio_num_t TXGPS = GPIO_NUM_4;

    static const gpio_num_t ESP_SHUTDOWN = GPIO_NUM_0;

    static const gpio_num_t TX485 = GPIO_NUM_2;
    static const gpio_num_t RX485 = GPIO_NUM_27;
    static const gpio_num_t MODE485 = GPIO_NUM_13;

    static const gpio_num_t LED_SHUTDOWN = GPIO_NUM_12;
    static const gpio_num_t LED = GPIO_NUM_14;

    static const gpio_num_t LOCK = GPIO_NUM_26;
} // namespace BlackBox