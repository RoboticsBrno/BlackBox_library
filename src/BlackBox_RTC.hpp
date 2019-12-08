#pragma once
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ds3231.h>
#include <string.h>


namespace BlackBox
{
    class BlackBox_RTC
    {
    private:
        i2c_dev_t dev;
        const gpio_num_t SDA_GPIO = ;
        const gpio_num_t SCL_GPIO = ;
    public:
        BlackBox_RTC(/* args */);
        ~BlackBox_RTC();
    };
    
    BlackBox_RTC::BlackBox_RTC(/* args */)
    {
        memset(&dev, 0, sizeof(i2c_dev_t));
        ESP_ERROR_CHECK(ds3231_init_desc(&dev, 0, SDA_GPIO, SCL_GPIO));
    }
    
    BlackBox_RTC::~BlackBox_RTC()
    {
    }
    
} // namespace BlackBox
