#pragma once

#include "BlackBox_pinout.hpp"
#include "BlackBox_Touchpad.hpp"
#include <SmartLeds.h>
#include <memory>
#include <mutex>

namespace BlackBox {

constexpr int ledCount = 60;
constexpr int channel = 0;

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
    Index(Coords other);

    int value() const;

    Index& operator+=(const Index&);
    Index& operator-=(const Index&);

    Index& operator++();
    Index& operator--();

    bool operator<(const Index&) const;
    bool operator>(const Index&) const;
    bool operator<=(const Index&) const;
    bool operator>=(const Index&) const;
    bool operator==(const Index&) const;
    bool operator!=(const Index&) const;

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

    // uint32 blend(unsigned color1, unsigned color2, unsigned alpha);

protected:

public:
    Ring(int count = ledCount);
    void show();
    void wait();
    // void show(Page&); // FIXME: What implementation would be best (page)?

    void drawArc(Rgb, Index beginnig, Index ending, ArcType arcType = ArcType::ShorterDistance);
    void drawCircle(Rgb);
    void draw(std::unique_ptr< Rgb[] > buffer);

    void clear();

    const Rgb& operator[](const Index&) const;
    Rgb& operator[](const Index&);

    void enableDarkMode();
    void disableDarkMode();
    void setDarkModeValue(int value);

    void rotate(Index beginning);
};

} // namespace BlackBox
