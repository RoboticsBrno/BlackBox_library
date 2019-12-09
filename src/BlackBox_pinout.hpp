#pragma once

#include "esp_system.h"

namespace BlackBox
{
    static const int LED_COUNT = 60;
    static const gpio_num_t DATA_PIN = GPIO_NUM_14;
    static const gpio_num_t TOOGLE_PIN = GPIO_NUM_12;
    static const int CHANNEL = 0;

    static const gpio_num_t SDA = GPIO_NUM_23;
    static const gpio_num_t SCL = GPIO_NUM_22;

} // namespace BlackBox
