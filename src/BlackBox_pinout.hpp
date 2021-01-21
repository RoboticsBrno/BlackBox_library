/**
 * @file BlackBox_pinout.hpp
 * @author Tomáš Rohlínek (haberturdeur)
 * @brief pinout of BlackBox board
 */

#pragma once

#include "driver/gpio.h"

namespace BlackBox {

namespace Pins {

namespace Leds {
constexpr gpio_num_t Data = GPIO_NUM_19;
}

namespace I2C {
constexpr gpio_num_t SCL = GPIO_NUM_22;
constexpr gpio_num_t SDA = GPIO_NUM_23;
}

namespace UART {
constexpr gpio_num_t TX = GPIO_NUM_1;
constexpr gpio_num_t RX = GPIO_NUM_3;
}

namespace Interrupts {
constexpr gpio_num_t LDC = GPIO_NUM_21;

constexpr gpio_num_t Magnetometer = GPIO_NUM_25;

constexpr gpio_num_t RTC = GPIO_NUM_26;

constexpr gpio_num_t Barometer = GPIO_NUM_27;

constexpr gpio_num_t Gyroscope1 = GPIO_NUM_32;
constexpr gpio_num_t Gyroscope2 = GPIO_NUM_33;

constexpr gpio_num_t Accelerometer1 = GPIO_NUM_34;
constexpr gpio_num_t Accelerometer2 = GPIO_NUM_35;
}

namespace Lock {
constexpr gpio_num_t A = GPIO_NUM_0;
constexpr gpio_num_t B = GPIO_NUM_4;

constexpr gpio_num_t Motor = GPIO_NUM_12;
}

namespace IR {
constexpr gpio_num_t Receiver = GPIO_NUM_15;
constexpr gpio_num_t Transmitter = GPIO_NUM_2;
}

namespace Power {
constexpr gpio_num_t ShutDown_All = GPIO_NUM_5;
constexpr gpio_num_t ShutDown_Leds = GPIO_NUM_13;
constexpr gpio_num_t ShutDown_5V = GPIO_NUM_14;
constexpr gpio_num_t ShutDown_LDC = GPIO_NUM_18;
constexpr gpio_num_t ShutDown = ShutDown_All;

constexpr gpio_num_t BatteryLevel = GPIO_NUM_39;
}
} // namespace BlackBox::Pins
} // namespace BlackBox