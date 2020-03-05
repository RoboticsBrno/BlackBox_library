#pragma once

#include <SmartLeds.h>
#include "BlackBox_pinout.hpp"
#include "RtcDateTime.h"

namespace BlackBox
{
    class index_t{
    private:
        uint8_t m_index;
        void swap(index_t i_input){
            m_index = i_input();
        }
    public:
        index_t& operator+=(uint8_t i_input){
            uint16_t value = m_index + i_input;
            value += i_input;
            m_index = value % BlackBox::LED_COUNT;
            return *this;
        }
        index_t& operator-=(uint8_t i_input){
            if(i_input>m_index){
                m_index = BlackBox::LED_COUNT-i_input+m_index;
            }else{
                m_index = m_index-i_input;
            }
            return *this;
        }

        index_t& operator++(){
            m_index = m_index<BlackBox::LED_COUNT ? m_index+1 : 0; 
            return *this;
        }
        index_t& operator--(){
            m_index = m_index>0 ? m_index-1 : BlackBox::LED_COUNT-1; 
            return *this;
        }

        index_t& operator=(index_t i_input){
            swap(i_input);
            return *this;
        }

        index_t& operator=(uint8_t i_input){
            m_index = i_input % BlackBox::LED_COUNT;
            return *this;
        }

        uint8_t operator()(){
            return m_index;
        }
    };

    class Time_t
    {
        private:
            uint32_t m_seconds;
        public:
            Time_t():
                m_seconds(0){}
            Time_t(uint32_t i_seconds):
                m_seconds(i_seconds){}
            Time_t(uint8_t i_hours, uint8_t i_minutes, uint8_t i_seconds):
                m_seconds(i_seconds+(i_minutes*60)+(i_hours*3600)){}
            Time_t(RtcDateTime i_datetime):
                m_seconds(i_datetime.Second()+(i_datetime.Minute()*60)+(i_datetime.Hour()*3600)){}

            uint8_t getHours(){ return m_seconds / 3600; }
            uint8_t getMinutes(){ return (m_seconds % 3600)/60; }
            uint8_t getSeconds(){ return (m_seconds % 3600)%60; }

            uint32_t getTotalSeconds(){ return m_seconds; };
    };


    class BlackBox_LEDring
    {
        friend class BlackBox_interface;
    private:
        BlackBox_LEDring();
        BlackBox_LEDring(bool i_initialize);

        uint8_t m_intenzity = 255;
        uint8_t m_dark_mode_intenzity = 10;
        bool m_dark_mode = 0;

        Rgb m_seconds_color = {255, 0, 0};
        Rgb m_minutes_color = {0, 255, 0};
        Rgb m_hours_color = {0, 0, 255};

        uint8_t m_layer_opacity = 0;
        uint8_t m_first_point_opacity = 127;
        bool m_force_first_point_opacity = 0;

        bool m_12hr_mode = 1;
        bool m_calendar_mode = 1;

        bool m_LED_state = 0;

        SmartLed m_leds;

        void pass(Rgb i_buffer[ BlackBox::LED_COUNT ]);
        uint8_t getIntenzity(){ return m_dark_mode ? m_dark_mode_intenzity : m_intenzity; }
        void writeLEDstate(){
            gpio_set_level(BlackBox::LED_TOOGLE_GPIO, m_LED_state);
        }
    public:
        SmartLed& leds(){
            return m_leds;
        }

        void init();

        void toogleLEDring(){
            m_LED_state = !m_LED_state;
            writeLEDstate();
        }
        void toogleLEDring(bool i_LED_state){
            m_LED_state = i_LED_state;
            writeLEDstate();
        }

        void setIntenzity(uint8_t i_intenzity)
            { m_intenzity = i_intenzity; }
        void setDarkModeIntenzity(uint8_t i_intenzity)
            { m_dark_mode_intenzity = i_intenzity; }
        void toogleDarkMode()
            { m_dark_mode = !m_dark_mode; }
        void toogleDarkMode(bool i_dark_mode)
            { m_dark_mode = i_dark_mode; }

        void setSecondsColor(Rgb i_color)
            { m_seconds_color = i_color; }
        void setMinutesColor(Rgb i_color)
            { m_minutes_color = i_color; }
        void setHoursColor(Rgb i_color)
            { m_hours_color = i_color; }

        void setLayerOpacity(uint8_t i_opacity)
            { m_layer_opacity = i_opacity; }
        void setFirstPointOpacity(uint8_t i_opacity)
            { m_first_point_opacity = i_opacity; }
        void toogleForceFirstPointOpacity()
            { m_force_first_point_opacity = !m_force_first_point_opacity; }
        void toogleForceFirstPointOpacity(bool i_force_first_point_opacity)
            { m_force_first_point_opacity = i_force_first_point_opacity; }

        void toogle12hrMode()
            { m_12hr_mode = !m_12hr_mode; }
        void toogle12hrMode(bool i_12hr_mode)
            { m_12hr_mode = i_12hr_mode; }

        void prepare(Rgb i_buffer[ BlackBox::LED_COUNT ]);
        void prepare(BlackBox::Time_t i_time);

        void clear();
        void show();
        void show(Rgb i_buffer[ BlackBox::LED_COUNT ]);

        void showTime(BlackBox::Time_t i_time);
        void showLevel(int8_t i_level, Rgb i_color, uint8_t i_opacity, int i_clockwise, bool i_clear = 1);
        void showArc(uint8_t i_from, uint8_t i_to, Rgb i_color, uint8_t i_opacity, int i_clockwise, bool i_clear = 1);
        void showCircle(Rgb i_color, uint8_t i_opacity, bool i_clear = 1);
    };

    
} // namespace BlackBox
