#include "Dsp.hpp"
#ifdef BB_POWER

#include "library/BlackBox_Power.hpp"

#include "library/BlackBox_pinout.hpp"

#include "driver/gpio.h"
#include <mutex>

namespace BlackBox {
void Power::setDefault() {
    std::scoped_lock l(m_mutex);
    gpio_set_level(m_powerAll.pinNumber, m_powerAll.defaultLevel);
    gpio_set_level(m_powerLDC.pinNumber, m_powerLDC.defaultLevel);
    gpio_set_level(m_power5V.pinNumber, m_power5V.defaultLevel);
}

Power::Power(Pins::PowerPin i_powerAll,
    Pins::PowerPin i_power5V,
    Pins::PowerPin i_powerLDC)
    : m_powerAll(i_powerAll)
    , m_power5V(i_power5V)
    , m_powerLDC(i_powerLDC)
    , m_isAllOn(i_powerAll.defaultLevel)
    , m_is5VOn(i_power5V.defaultLevel)
    , m_isLDCOn(i_powerLDC.defaultLevel)
    , m_powerConfig {
        .pin_bit_mask = ((1ULL << i_powerAll.pinNumber) | (1ULL << i_power5V.pinNumber) | (1ULL << i_powerLDC.pinNumber)),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    } {
}

void Power::init() {
    std::scoped_lock l(m_mutex);
    setDefault();
    gpio_config(&m_powerConfig);
}

void Power::turnOn(Pins::PowerPin i_powerPin) {
    std::scoped_lock l(m_mutex);
    gpio_set_level(i_powerPin.pinNumber, i_powerPin.onLevel);
}

void Power::turnOff(Pins::PowerPin i_powerPin) {
    std::scoped_lock l(m_mutex);
    gpio_set_level(i_powerPin.pinNumber, !i_powerPin.onLevel);
}

void Power::turnOn() {
    turnOn(m_powerAll);
}

void Power::turnOff() {
    turnOff(m_powerAll);
}

void Power::turnOnLDC() {
    turnOn(m_powerLDC);
}

void Power::turnOffLDC() {
    turnOff(m_powerLDC);
}

void Power::turnOn5V() {
    turnOn(m_power5V);
}

void Power::turnOff5V() {
    turnOff(m_power5V);
}
} // namespace BlackBox
#endif
