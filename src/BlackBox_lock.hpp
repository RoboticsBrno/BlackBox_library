#pragma once

#include <esp_system.h>
#include <driver/timer.h>

#include "BlackBox_pinout.hpp"


namespace BlackBox
{
    class BlackBox_Lock
    {
    private:

    public:
        esp_err_t set(const gpio_num_t hall = GPIO_NUM_33, const gpio_num_t motor = BlackBox::LOCK);

    };
    
    
} // namespace BlackBox
