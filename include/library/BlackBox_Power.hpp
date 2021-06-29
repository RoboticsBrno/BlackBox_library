/**
 * @file BlackBox_Power.hpp
 * @author Tomáš Rohlínek (haberturdeur)
 * \~czech @brief Knihovna pro ovládání napájení desky
 * \~english @brief Library for controling power pins
 */

#pragma once

#include "Dsp.hpp"
#ifdef BB_POWER

#include "library/BlackBox_pinout.hpp"

#include "driver/gpio.h"
#include <mutex>

namespace BlackBox {
/**
 * \~czech @brief Třída pro ovládání napájení desky
 * \~english @brief Class for controling power pins
 */
class Power {
private:
    const char* m_tag = "Power";

    mutable std::recursive_mutex m_mutex;

    const Pins::PowerPin m_powerAll;
    const Pins::PowerPin m_power5V;
    const Pins::PowerPin m_powerLDC;

    bool m_isAllOn;
    bool m_is5VOn;
    bool m_isLDCOn;

    gpio_config_t m_powerConfig;

    void setDefault();

public:
    Power(Pins::PowerPin powerAll = Pins::Power::PowerAll,
        Pins::PowerPin power5V = Pins::Power::Power5V,
        Pins::PowerPin powerLDC = Pins::Power::PowerLDC);

    ~Power() = default;

    void init();

    void turnOn(Pins::PowerPin);
    void turnOff(Pins::PowerPin);

    void turnOn();
    void turnOff();

    void turnOnLDC();
    void turnOffLDC();

    void turnOn5V();
    void turnOff5V();
};

} // namespace BlackBox
#endif
