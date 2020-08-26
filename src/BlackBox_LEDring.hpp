#pragma once

#include "BlackBox_pinout.hpp"
#include <SmartLeds.h>
#include <cstdint>

namespace BlackBox {
    
class index_t {
private:
    unsigned m_value;

    void swap(const index_t i_input) {
        m_value = i_input.m_value;
    }

public:
    index_t()
        : m_value(0) {}

    index_t(unsigned i_input)
        : m_value(i_input % BlackBox::LED_COUNT) {}

    index_t& operator+=(const uint8_t i_input) {
        m_value = (m_value + i_input) % BlackBox::LED_COUNT;
        return *this;
    }

    index_t& operator-=(const uint8_t i_input) {
        if (i_input > m_value)
            m_value += BlackBox::LED_COUNT;
        m_value = m_value - i_input;
        return *this;
    }

    index_t& operator++() {
        m_value = (m_value < BlackBox::LED_COUNT) ? m_value + 1 : 0;
        return *this;
    }

    index_t& operator--() {
        m_value = (m_value > 0) ? m_value - 1 : (BlackBox::LED_COUNT - 1);
        return *this;
    }

    index_t& operator=(const index_t i_input) {
        swap(i_input);
        return *this;
    }

    index_t& operator=(const uint8_t i_input) {
        m_value = i_input % BlackBox::LED_COUNT;
        return *this;
    }

    operator unsigned() const { return m_value; }

    uint8_t value() const { return m_value; }
};

class BlackBox_LEDring {
    friend class BlackBox_manager;

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

    uint8_t getIntenzity();

    void writeLEDstate();

public:
    SmartLed& leds();

    void init();

    void toogleLEDring();
    void toogleLEDring(bool i_ledState);

    void setIntenzity(uint8_t i_intenzity);
    void setDarkModeIntenzity(uint8_t i_intenzity);

    void toogleDarkMode();
    void toogleDarkMode(bool i_darkMode);

    void setSecondsColor(Rgb i_color);
    void setMinutesColor(Rgb i_color);
    void setHoursColor(Rgb i_color);

    void setLayerOpacity(uint8_t i_opacity);
    void setFirstPointOpacity(uint8_t i_opacity);
    void toogleForceFirstPointOpacity();
    void toogleForceFirstPointOpacity(bool i_forceFirstPointOpacity);

    void toogle12hrMode();
    void toogle12hrMode(bool i_12hrMode);

    void prepare(Rgb i_buffer[BlackBox::LED_COUNT]);
    void prepare(BlackBox::Time_t i_time);

    void clear();
    void show();
    void show(Rgb i_buffer[BlackBox::LED_COUNT]);

    // void showTime(BlackBox::Time_t i_time); // FIXME: Fix this function
    void showLevel(int8_t i_level, Rgb i_color, uint8_t i_opacity, int i_clockwise, bool i_clear = 1);
    void showArc(uint8_t i_from, uint8_t i_to, Rgb i_color, uint8_t i_opacity, int i_clockwise, bool i_clear = 1);
    void showCircle(Rgb i_color, uint8_t i_opacity, bool i_clear = 1);
};

} // namespace BlackBox
