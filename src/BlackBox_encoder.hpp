#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <mutex>

#include "esp_log.h"
#include <driver/gpio.h>
#include <driver/pcnt.h>


#include "BlackBox_LEDring.hpp"
#include "BlackBox_pinout.hpp"

#define DEFAULT_RELEASE_TIMEOUT 10000000
#define DEFAULT_DEBOUNCE_TIME 10000
#define DEFAULT_LONG_CLICK_TIMEOUT 500000
#define DEFAULT_DOUBLE_CLICK_TIMEOUT 500000

#define PCNT_UNIT PCNT_UNIT_0
#define PCNT_H_LIM_VAL 60
#define PCNT_L_LIM_VAL -60

#define SW_GPIO_SEL 1ULL << BlackBox::ENC_SW_GPIO

namespace BlackBox {

typedef enum {
    BUTTON_RELEASED = 0,
    BUTTON_PRESSED = 1,
    BUTTON_LONGPRESSED = 2,
} button_state_t;

static const pcnt_config_t pcntConfig = {
    // Set PCNT input signal and control GPIOs
    .pulse_gpio_num = BlackBox::ENC_A_GPIO,
    .ctrl_gpio_num = BlackBox::ENC_B_GPIO,
    // What to do when control input is low or high?
    .lctrl_mode = PCNT_MODE_REVERSE, // Reverse counting direction if low
    .hctrl_mode = PCNT_MODE_KEEP, // Keep the primary counter mode if high
    // What to do on the positive / negative edge of pulse input?
    .pos_mode = PCNT_COUNT_INC, // Count up on the positive edge
    .neg_mode = PCNT_COUNT_DIS, // Keep the counter value on the negative edge
    // Set the maximum and minimum limit values to watch
    .counter_h_lim = PCNT_H_LIM_VAL,
    .counter_l_lim = PCNT_L_LIM_VAL,
    .unit = PCNT_UNIT,
    .channel = PCNT_CHANNEL_0,
};

static const gpio_config_t gpioConfig = {
    .pin_bit_mask = SW_GPIO_SEL,
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_ENABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
};

class BlackBox_encoder {
    friend class BlackBox_interface;

private:
    BlackBox_encoder() = default;
    BlackBox_encoder(uint32_t i_releaseTimeout, uint32_t m_debounceTime, uint32_t i_longClickTimeout, uint32_t i_doubleClickTimeout);

    const char* m_tag = "BB_encoder";

    uint32_t m_releaseTimeout = DEFAULT_RELEASE_TIMEOUT; // 10 s
    uint32_t m_debounceTime = DEFAULT_DEBOUNCE_TIME; // 10 ms
    uint32_t m_longClickTimeout = DEFAULT_LONG_CLICK_TIMEOUT; // 500 ms
    uint32_t m_doubleClickTimeout = DEFAULT_DOUBLE_CLICK_TIMEOUT; // 500 ms



    void initEncoder();
    void initButton();
    void initFlags(uint8_t i_flags);
    // void init();

    esp_err_t readEncoder();
    void readButton();
    // esp_err_t read();


public:
    index_t valueIndex(bool i_renew = 0);
    uint8_t value(bool i_renew = 0);
    button_state_t buttonState(bool i_renew = 0);
    // bool awaitRelease(uint32_t i_releaseTimeout = m_releaseTimeout);


    void setReleaseTimeout(uint32_t i_releaseTimeout);
    void setDebounceTime(uint32_t i_debounceTime);
    void setLongClickTimeout(uint32_t i_longClickTimeout);
    void setDoubleClickTimeout(uint32_t i_doubleClickTimeout);


    std::recursive_mutex& mutex() { return m_mutex; }

    ~BlackBox_encoder() = default;

    const char* tag() { return m_tag; }
};

} // namespace BlackBox
