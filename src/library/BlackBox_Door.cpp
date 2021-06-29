#include "Dsp.hpp"
#ifdef BB_DOORS

#include "library/BlackBox_Door.hpp"

#include "driver/gpio.h"
#include "driver/ledc.h"

namespace BlackBox {
void Door::drive(bool i_locked) {
    std::scoped_lock l(m_mutex);
    
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, m_channelConfig.channel, s_duty[i_locked]);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, m_channelConfig.channel);
}

void Door::readTamperCheck() {
}

Door::Door(Pins::DoorPin i_pins,
    ledc_timer_t i_timer,
    ledc_channel_t i_channel)
    : m_pins(i_pins)
    , m_timerConfig {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_16_BIT,
        .timer_num = i_timer,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK,
    }
    , m_channelConfig {
        .gpio_num = m_pins.servo,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = i_channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = i_timer,
        .duty = 0,
        .hpoint = 0,
    }
    , m_tamperCheckConfig {
        .pin_bit_mask = (1ULL << m_pins.tamperCheck),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    } {}

void Door::init() {
    std::scoped_lock l(m_mutex);

    ledc_timer_config(&m_timerConfig);
    ledc_channel_config(&m_channelConfig);

    gpio_config(&m_tamperCheckConfig); // FIXME: Update this to newer version with new hall sensor
}

void Door::lock() {
    drive(true);
}

void Door::unlock() {
    drive(false);
}

bool Door::locked() {
    std::scoped_lock l(m_mutex);

    return (ledc_get_duty(LEDC_HIGH_SPEED_MODE, m_channelConfig.channel) == s_duty[1]);
}

bool Door::closed() {
    std::scoped_lock l(m_mutex);

    return gpio_get_level(m_pins.tamperCheck);
}

} // namespace BlackBox
#endif
