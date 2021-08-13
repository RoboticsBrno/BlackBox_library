#include "Dsp.hpp"
#ifdef BB_POWER

#include "library/BlackBox_Power.hpp"

#include "library/BlackBox_pinout.hpp"

#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"
#include "soc/adc_channel.h"

#include <algorithm>
#include <mutex>

namespace BlackBox {
void Power::setDefault() {
    std::scoped_lock l(m_mutex);
    gpio_set_level(m_powerAll.pinNumber, m_powerAll.defaultLevel);
    gpio_set_level(m_powerLDC.pinNumber, m_powerLDC.defaultLevel);
    gpio_set_level(m_power5V.pinNumber, m_power5V.defaultLevel);
}

void Power::readVoltage() {
    std::scoped_lock l(m_mutex);
    unsigned voltage;
    esp_adc_cal_get_voltage(static_cast<const adc_channel_t>(m_channel), m_characteristic.get(), &voltage);
    m_voltage = voltage * 4; 
}

Power::Power(Pins::PowerPin i_powerAll,
    Pins::PowerPin i_power5V,
    Pins::PowerPin i_powerLDC,
    gpio_num_t i_usbConnectionCheck,
    adc1_channel_t i_channel,
    adc_bits_width_t i_width)
    : m_powerAll(i_powerAll)
    , m_power5V(i_power5V)
    , m_powerLDC(i_powerLDC)
    , m_usbConnectionCheck(i_usbConnectionCheck)
    , m_channel(i_channel)
    , m_width(i_width)
    , m_characteristic(std::make_unique<esp_adc_cal_characteristics_t>())
    , m_isAllOn(i_powerAll.defaultLevel)
    , m_is5VOn(i_power5V.defaultLevel)
    , m_isLDCOn(i_powerLDC.defaultLevel)
    , m_powerConfig {
        .pin_bit_mask = ((1ULL << i_powerAll.pinNumber) | (1ULL << i_power5V.pinNumber) | (1ULL << i_powerLDC.pinNumber)),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    }
    , m_usbConnectionCheckConfig {
        .pin_bit_mask = (1ULL << i_usbConnectionCheck),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    } {
}

void Power::init() {
    std::scoped_lock l(m_mutex);
    setDefault();
    gpio_config(&m_powerConfig);
    gpio_config(&m_usbConnectionCheckConfig);

    adc1_config_width(m_width);
    adc1_config_channel_atten(m_channel, ADC_ATTEN_DB_6);

    auto valueType = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_6, m_width, 1100, m_characteristic.get());

    if (valueType == ESP_ADC_CAL_VAL_EFUSE_TP)
        ESP_LOGI(m_tag, "Characterized using Two Point Value\n");
    else if (valueType == ESP_ADC_CAL_VAL_EFUSE_VREF)
        ESP_LOGI(m_tag, "Characterized using eFuse Vref\n");
    else
        ESP_LOGI(m_tag, "Characterized using Default Vref\n");

    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK)
        ESP_LOGI(m_tag, "eFuse Two Point: Supported\n");
    else
        ESP_LOGI(m_tag, "eFuse Two Point: NOT supported\n");

    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK)
        ESP_LOGI(m_tag, "eFuse Vref: Supported\n");
    else
        ESP_LOGI(m_tag, "eFuse Vref: NOT supported\n");
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

int Power::batteryVoltage(bool update) {
    std::scoped_lock l(m_mutex);
    if(update)
        readVoltage();
    return m_voltage;
}

int Power::batteryPercentage(bool i_update) {
    return std::max(std::min(((batteryVoltage(i_update) - s_baseVoltage) * 100 / s_voltageDifference), 100), 0);
}

bool Power::checkBatteryLevel(unsigned i_batteryLevel, bool i_act) {
    bool check = batteryVoltage(true) < i_batteryLevel;
    if (check && i_act)
        turnOff();
    return !check;
}

bool Power::usbConnected() {
    std::scoped_lock l(m_mutex);

    return gpio_get_level(m_usbConnectionCheck);
}

} // namespace BlackBox
#endif
