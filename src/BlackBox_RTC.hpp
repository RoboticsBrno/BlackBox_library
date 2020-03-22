#pragma once
#include "BlackBox_pinout.hpp"
#include <RtcDateTime.h>
#include <cstdint>
#include <ctime>
#include <ds3231.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <string.h>

namespace BlackBox {
struct tm default_time = {
    .tm_sec = 10,
    .tm_min = 50,
    .tm_hour = 13,
    .tm_mday = 9,
    .tm_mon = 9, // 0-based
    .tm_year = 2016
};

class BlackBox_RTC {
private:
    i2c_dev_t dev;
    struct tm now;

public:
    BlackBox_RTC(bool set_time = 0, tm* set = &default_time);
    tm& get_time();
};

BlackBox_RTC::BlackBox_RTC(bool set_time, tm* set)
{
    memset(&dev, 0, sizeof(i2c_dev_t));
    ESP_ERROR_CHECK(ds3231_init_desc(&dev, I2C_NUM_0, BlackBox::SDA_GPIO, BlackBox::SCL_GPIO));
    if (set_time)
        ESP_ERROR_CHECK(ds3231_set_time(&dev, set));
}

tm& BlackBox_RTC::get_time()
{
    ds3231_get_time(&dev, &now);
    return now;
}

} // namespace BlackBox
