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

#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_adc_cal.h"
#include "soc/adc_channel.h"
#include <cnl/fixed_point.h>
#include <memory>
#include <mutex>

namespace BlackBox {
/**
 * \~czech @brief Třída pro ovládání napájení desky
 * \~english @brief Class for controling power pins
 */
class Power {
private:
    using Number = cnl::scaled_integer<std::int32_t, cnl::power<-5>>;


    const char* m_tag = "Power";

    mutable std::recursive_mutex m_mutex;

    static constexpr unsigned s_batteryVoltages[2] = {3700, 4150};
    static constexpr unsigned s_baseVoltage = s_batteryVoltages[0];
    static constexpr unsigned s_maxVoltage = s_batteryVoltages[1];
    static constexpr unsigned s_voltageDifference = s_maxVoltage - s_baseVoltage;


    const Pins::PowerPin m_powerAll;
    const Pins::PowerPin m_power5V;
    const Pins::PowerPin m_powerLDC;

    const adc1_channel_t m_channel;
    const adc_bits_width_t m_width;

    std::unique_ptr<esp_adc_cal_characteristics_t> m_characteristic;

    bool m_isAllOn;
    bool m_is5VOn;
    bool m_isLDCOn;

    unsigned m_voltage;

    gpio_config_t m_powerConfig;

    void setDefault();

    void readVoltage();

public:
    Power(Pins::PowerPin powerAll = Pins::Power::PowerAll,
        Pins::PowerPin power5V = Pins::Power::Power5V,
        Pins::PowerPin powerLDC = Pins::Power::PowerLDC,
        adc1_channel_t = ADC1_GPIO39_CHANNEL,
        adc_bits_width_t = ADC_WIDTH_BIT_12);

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

    unsigned batteryVoltage(bool update = false);
    unsigned batteryPercentage(bool update = false);
};

} // namespace BlackBox
#endif
