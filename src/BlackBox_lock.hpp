#pragma once

#include <driver/timer.h>
#include <esp_system.h>

#include "BlackBox_pinout.hpp"
#include <cstdint>

namespace BlackBox {
class BlackBox_Lock {
private:
public:
    void set(const gpio_num_t hall = GPIO_NUM_33, const gpio_num_t motor = BlackBox::LOCK_GPIO);
};

} // namespace BlackBox
