
#include "BlackBox_LEDring.hpp"
#include <map>

namespace BlackBox
{
    BlackBox_LEDring::BlackBox_LEDring():
        m_leds(LED_WS2812B, BlackBox::LED_COUNT, BlackBox::LED_DATA_GPIO, BlackBox::CHANNEL, DoubleBuffer)
    {
          
    }
    BlackBox_LEDring::BlackBox_LEDring(bool i_initialize):
        m_leds(LED_WS2812B, BlackBox::LED_COUNT, BlackBox::LED_DATA_GPIO, BlackBox::CHANNEL, DoubleBuffer)
    {
        if (i_initialize)
            init();
    }

    void BlackBox_LEDring::init(){
        gpio_pad_select_gpio(BlackBox::LED_TOOGLE_GPIO);
        gpio_set_direction(BlackBox::LED_TOOGLE_GPIO, GPIO_MODE_OUTPUT);
        writeLEDstate();
    }

    void BlackBox_LEDring::pass(Rgb i_buffer[ BlackBox::LED_COUNT ]){
        for (int i = 0; i < BlackBox::LED_COUNT; i++)
        {
            m_leds[i] = i_buffer[i];
        }  
    }

    void BlackBox_LEDring::prepare(Rgb i_buffer[ BlackBox::LED_COUNT ]){
        for (int i = 0; i < BlackBox::LED_COUNT; i++)
        {
            i_buffer[i].stretchChannelsEvenly(getIntenzity());
        }
        pass(i_buffer);
    }

    void BlackBox_LEDring::prepare(BlackBox::Time_t i_time){
        Rgb buffer[ BlackBox::LED_COUNT ];
        buffer[(i_time.getHours()*5)%60] += m_hours_color;
        buffer[i_time.getMinutes()%60] += m_minutes_color;
        buffer[i_time.getSeconds()%60] += m_seconds_color;
        prepare(buffer);
    }

    void BlackBox_LEDring::clear(){
        Rgb buffer[ BlackBox::LED_COUNT ];
        for (int i = 0; i < BlackBox::LED_COUNT; i++)
        {
            buffer[i] = Rgb(0,0,0);
        }
        show(buffer);
    }

    void BlackBox_LEDring::show(){
        m_leds.show();
    }

    void BlackBox_LEDring::show(Rgb i_buffer[ BlackBox::LED_COUNT ]){
        prepare(i_buffer);
        show();
    }

    void BlackBox_LEDring::showTime(BlackBox::Time_t i_time){
        prepare(i_time);
        show();
    }

    void BlackBox_LEDring::showCircle(Rgb i_color, uint8_t i_opacity, bool i_clear){
        Rgb buffer[BlackBox::LED_COUNT];
        for (int i = 0; i < BlackBox::LED_COUNT; i++){
            buffer[i] = i_color;
        }
        prepare(buffer);
        show();
    }

    void BlackBox_LEDring::showLevel(int8_t i_level, Rgb i_color, uint8_t i_opacity, int i_clockwise, bool i_clear){
        if (i_clear) clear();
    }
} // namespace BlackBox
