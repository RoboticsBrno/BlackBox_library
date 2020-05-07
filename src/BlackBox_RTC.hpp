#pragma once

#include "BlackBox_LEDring.hpp"
#include "BlackBox_pinout.hpp"
#include <cstdint>
#include <ctime>
#include <ds3231.h>
#include <stdio.h>
#include <string.h>

namespace BlackBox {


class BlackBox_RTC {
    friend class BlackBox_interface;
    friend class BlackBox_manager;

private:
    BlackBox_RTC(bool i_initialize = 0)
    {
        if (i_initialize)
            init();
    }

    i2c_dev_t dev;

public:
    ~BlackBox_RTC() { ESP_ERROR_CHECK(ds3231_free_desc(&dev)); }

    void init();

    void time(tm* o_time);
    float temperature();

    esp_err_t setTime(tm* i_tm);
};
    // void convertToTm(tm* o_tm, time_t* i_time) { *o_tm = *localtime(i_time); }
    // void convertToTime(time_t* o_time, tm* i_tm) { *o_time = mktime(i_tm); }
    // void convertToBBTime(Time_t* o_Time, time_t* o_time);
} // namespace BlackBox


