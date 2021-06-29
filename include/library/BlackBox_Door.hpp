#pragma once

#include "Dsp.hpp"
#ifdef BB_DOORS

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "library/BlackBox_pinout.hpp"
#include <mutex>

namespace BlackBox {
class Door {
private:
    const char* m_tag = "Door";

    bool m_isLocked;
    static constexpr int s_duty[2] = {1638, 7864};

    mutable std::recursive_mutex m_mutex;

    const Pins::DoorPin m_pins;

    ledc_timer_config_t m_timerConfig;
    ledc_channel_config_t m_channelConfig;

    gpio_config_t m_tamperCheckConfig;

    void drive(bool locked);

    void readTamperCheck();

public:
    ~Door() = default;

    Door(Pins::DoorPin pins, 
        ledc_timer_t timer = LEDC_TIMER_0,
        ledc_channel_t channel = LEDC_CHANNEL_0);

    void init();

    void lock();
    void unlock();

    bool locked();
    bool closed();
};
} // namespace BlackBox
#endif
