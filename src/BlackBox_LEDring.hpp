#pragma once

#include "BlackBox_pinout.hpp"
#include <SmartLeds.h>
#include <cstdint>

namespace BlackBox {
class index_t {
private:
    uint8_t m_index;

    void swap(index_t i_input)
    {
        m_index = i_input();
    }

public:
    index_t& operator+=(uint8_t i_input)
    {
        m_index = (m_index + i_input) % BlackBox::LED_COUNT;
        return *this;
    }

    index_t& operator-=(uint8_t i_input)
    {
        if (i_input > m_index)
            m_index += BlackBox::LED_COUNT;
        m_index = m_index - i_input;
        return *this;
    }

    index_t& operator++()
    {
        m_index = (m_index < BlackBox::LED_COUNT) ? m_index + 1 : 0;
        return *this;
    }

    index_t& operator--()
    {
        m_index = (m_index > 0) ? m_index - 1 : (BlackBox::LED_COUNT - 1);
        return *this;
    }

    index_t& operator=(index_t i_input)
    {
        swap(i_input);
        return *this;
    }

    index_t& operator=(uint8_t i_input)
    {
        m_index = i_input % BlackBox::LED_COUNT;
        return *this;
    }

    uint8_t operator()()
    {
        return m_index;
    }

};

class Time_t {
private:
    uint32_t m_seconds;

public:
    Time_t()
        : m_seconds(0)
    {
    }

    Time_t(uint32_t i_seconds)
        : m_seconds(i_seconds)
    {
    }

    Time_t(uint8_t i_hours, uint8_t i_minutes, uint8_t i_seconds)
        : m_seconds(i_seconds + (i_minutes * 60) + (i_hours * 3600))
    {
    }

    uint8_t getHours() { return m_seconds / 3600; }
    uint8_t getMinutes() { return (m_seconds % 3600) / 60; }
    uint8_t getSeconds() { return (m_seconds % 3600) % 60; }

    uint32_t getTotalSeconds() { return m_seconds; }
};

class BlackBox_LEDring {
    friend class BlackBox_interface;

private:
    BlackBox_LEDring();

    BlackBox_LEDring(bool i_initialize);

    uint8_t m_intenzity = 255;

    bool m_darkMode = 0;

    Rgb m_secondsColor = { 255, 0, 0 };
    Rgb m_minutesColor = { 0, 255, 0 };
    Rgb m_hoursColor = { 0, 0, 255 };

    uint8_t m_darkModeIntenzity = 10;
    uint8_t m_layerOpacity = 0;
    uint8_t m_firstPointOpacity = 127;

    bool m_forceFirstPointOpacity = 0;
    bool m_12hrMode = 1;
    bool m_calendarMode = 1;
    bool m_ledState = 0;

    SmartLed m_leds;

    void pass(Rgb i_buffer[BlackBox::LED_COUNT]);

    uint8_t getIntenzity() { return m_dark_mode ? m_darkModeIntenzity : m_intenzity; }

    void writeLEDstate()
    {
        gpio_set_level(BlackBox::LED_TOOGLE_GPIO, m_ledState);
    }

public:
    SmartLed& leds()
    {
        return m_leds;
    }

    void init();

    void toogleLEDring()
    {
        m_ledState = !m_ledState;
        writeLEDstate();
    }

    void toogleLEDring(bool i_ledState)
    {
        m_ledState = i_ledState;
        writeLEDstate();
    }

    void setIntenzity(uint8_t i_intenzity)
    {
        m_intenzity = i_intenzity;
    }

    void setDarkModeIntenzity(uint8_t i_intenzity)
    {
        m_darkModeIntenzity = i_intenzity;
    }

    void toogleDarkMode()
    {
        m_dark_mode = !m_dark_mode;
    }

    void toogleDarkMode(bool i_darkMode)
    {
        m_dark_mode = i_darkMode;
    }

    void setSecondsColor(Rgb i_color)
    {
        m_secondsColor = i_color;
    }

    void setMinutesColor(Rgb i_color)
    {
        m_minutesColor = i_color;
    }

    void setHoursColor(Rgb i_color)
    {
        m_hoursColor = i_color;
    }

    void setLayerOpacity(uint8_t i_opacity)
    {
        m_layerOpacity = i_opacity;
    }

    void setFirstPointOpacity(uint8_t i_opacity)
    {
        m_firstPointOpacity = i_opacity;
    }

    void toogleForceFirstPointOpacity()
    {
        m_forceFirstPointOpacity = !m_forceFirstPointOpacity;
    }

    void toogleForceFirstPointOpacity(bool i_forceFirstPointOpacity)
    {
        m_forceFirstPointOpacity = i_forceFirstPointOpacity;
    }

    void toogle12hrMode()
    {
        m_12hrMode = !m_12hrMode;
    }

    void toogle12hrMode(bool i_12hrMode)
    {
        m_12hrMode = i_12hrMode;
    }

    void prepare(Rgb i_buffer[BlackBox::LED_COUNT]);
    void prepare(BlackBox::Time_t i_time);

    void clear();
    void show();
    void show(Rgb i_buffer[BlackBox::LED_COUNT]);

    void showTime(BlackBox::Time_t i_time);
    void showLevel(int8_t i_level, Rgb i_color, uint8_t i_opacity, int i_clockwise, bool i_clear = 1);
    void showArc(uint8_t i_from, uint8_t i_to, Rgb i_color, uint8_t i_opacity, int i_clockwise, bool i_clear = 1);
    void showCircle(Rgb i_color, uint8_t i_opacity, bool i_clear = 1);
};

} // namespace BlackBox
