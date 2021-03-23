/**
 * @file BlackBox_Power.hpp
 * @author Tomáš Rohlínek (haberturdeur)
 * @brief Library for controling power pins
 */

#pragma once

#include "BlackBox_pinout.hpp"

#include "driver/gpio.h"
#include <mutex>

namespace BlackBox {
/**
 * @brief Class for controling power pins
 */
class Power {
private:
    const char* m_tag = "Power";

    mutable std::recursive_mutex m_mutex;

    const PowerPin m_powerAll;
    const PowerPin m_power5V;
    const PowerPin m_powerLDC;

    bool m_isAllOn;
    bool m_is5VOn;
    bool m_isLDCOn;

    gpio_config_t m_powerConfig;

    void setDefault();

public:
    Power(PowerPin powerAll = Pins::Power::PowerAll,
        PowerPin power5V = Pins::Power::Power5V,
        PowerPin powerLDC = Pins::Power::PowerLDC);

    ~Power() = default;

    void init();

    void turnOn(PowerPin);
    void turnOff(PowerPin);

    void turnOn();
    void turnOff();

    void turnOnLDC();
    void turnOffLDC();

    void turnOn5V();
    void turnOff5V();
};

} // namespace BlackBox