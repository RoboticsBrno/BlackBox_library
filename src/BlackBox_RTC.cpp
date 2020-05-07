#include "BlackBox_RTC.hpp"

namespace BlackBox {
void BlackBox_RTC::init()
{
    ESP_ERROR_CHECK(i2cdev_init());
    memset(&dev, 0, sizeof(i2c_dev_t));
    ESP_ERROR_CHECK(ds3231_init_desc(&dev, I2C_NUM_0, BlackBox::SDA_GPIO, BlackBox::SCL_GPIO));
}

void BlackBox_RTC::time(tm* o_time)
{
    ESP_ERROR_CHECK(ds3231_get_time(&dev, o_time));
}

float BlackBox_RTC::temperature(){
    float out;
    ESP_ERROR_CHECK(ds3231_get_temp_float(&dev, &out));
    return out;
}

esp_err_t BlackBox_RTC::setTime(tm* i_tm)
{
    esp_err_t err;
    int i = 0;
    do {
        err = ds3231_set_time(&dev, i_tm);
        printf("%i",i);
        i++;
    } while (err != ESP_OK);
    printf("\n");
    return err;
}
//ESP_ERR_TIMEOUT 

// void convertToBBTime(Time_t* o_Time, time_t* i_time)
// {
//     tm _tm;
//     convertToTm(&_tm, i_time);
//     *o_Time = _tm.tm_hour * 3600 + _tm.tm_min * 60 + _tm.tm_sec;
// }
} // namespace BlackBox
