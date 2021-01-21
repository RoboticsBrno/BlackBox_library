#pragma once

#include "BlackBox_pinout.hpp"
#include <SmartLeds.h>
#include <memory>
#include <mutex>

namespace BlackBox {

constexpr unsigned ledCount = 60;
constexpr unsigned channel = 0;

enum class ArcType {
    ShorterDistance = 0,
    LongerDistance,
    Clockwise,
    CounterClockwise,
};

class Index {
private:
    int m_value;
    int trim(int index);
    Index& trimThis();

public:
    Index(int index = 0);

    int value() const;

    Index& operator+=(const Index&);
    Index& operator-=(const Index&);

    Index& operator++();
    Index& operator--();

    operator int() const;
};

class Ring {
private:
    SmartLed m_leds;
    std::unique_ptr< Rgb[] > m_buffer;

    const int m_count;

    int m_darkModeValue; // max value
    bool m_isDarkModeEnabled;

    Index m_beginning;

    mutable std::recursive_mutex m_mutex;

protected:

public:
    Ring(int count = ledCount);
    void show();
    void wait() {
        m_leds.wait();
    }
    // void show(Page&); // FIXME: What implementation would be best (page)?

    // void drawArc(Rgb, Index beginnig, Index ending, ArcType arkType = ArcType::ShorterDistance);
    // void drawCircle(Rgb);
    // void draw(std::unique_ptr< Rgb[] > buffer);

    const Rgb& operator[](Index) const;
    Rgb& operator[](Index);

    void enableDarkMode();
    void disableDarkMode();
    void setDarkModeValue(int value);

    void rotate(Index beginning);
};

} // namespace BlackBox
