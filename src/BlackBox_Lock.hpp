#pragma once

#include "BlackBox_pinout.hpp"
#include "driver/gpio.h"
#include "driver/ledc.h"

namespace BlackBox {
class Lock {
private:
    const char* m_tag = "Lock";

    enum LockState {
        Unlocked = 0,
        Locked = 1,
    } m_state;

    gpio_num_t m_motor;
    gpio_num_t m_encoderA;
    gpio_num_t m_encoderB;

    ledc_timer_config_t m_timerConfig;
    ledc_channel_config_t m_channelConfig;

    gpio_config_t m_encoderConfig;

    void drive(LockState, int duty = 0b10); // FIXME: test maximum reliable speed and use it

public:
    ~Lock() = default;

    Lock(gpio_num_t motor = Pins::Lock::Motor,
        gpio_num_t encoderA = Pins::Lock::A,
        gpio_num_t encoderB = Pins::Lock::B,
        ledc_timer_t timer = LEDC_TIMER_0,
        ledc_channel_t channel = LEDC_CHANNEL_0);

    void init();

    void lock();

    void unlock();

    bool locked();
};
} // namespace BlackBox
