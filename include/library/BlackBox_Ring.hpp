/**
 * @file BlackBox_Ring.hpp
 * @author Tomáš Rohlínek (haberturdeur)
 * @brief Library for controling the LED Ring
 */

#pragma once

#include "library/BlackBox_pinout.hpp"
#include "library/BlackBox_Touchpad.hpp"
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

/**
 * @brief Data type for handling polar coords based on LED Ring model
 */
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

/**
 * @brief Class for controling the LED Ring
 */
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
    /**
     * @brief Construct a new Ring object
     * 
     * @param count Count of Smart LEDs
     */
    Ring(int count = ledCount);

    /**
     * @brief Draw buffered frame
     */
    void show();

    /**
     * @brief Wait for drawing to finish
     * 
     * @note Is called from show() anyway
     */
    void wait();
    // void show(Page&); // FIXME: What implementation would be best (page)?

    /**
     * @brief Draw arc onto frame
     * 
     * @param colour
     * @param beginnig
     * @param ending
     * @param arcType
     */
    void drawArc(Rgb colour, Index beginnig, Index ending, ArcType arcType = ArcType::ShorterDistance);

    /**
     * @brief Draw full circle onto frame
     * 
     * @param colour
     */
    void drawCircle(Rgb colour);

    /**
     * @brief Draw whole frame
     * 
     * @param buffer 
     */
    void draw(std::unique_ptr< Rgb[] > buffer);

    /**
     * @brief Clear active frame
     * 
     * @note Just draws black circle
     */
    void clear();

    const Rgb& operator[](const Index&) const;
    Rgb& operator[](const Index&);

    /**
     * @brief Enable dark mode
     */
    void enableDarkMode();

    /**
     * @brief Disable dark mode
     */
    void disableDarkMode();

    /**
     * @brief Set dark mode value
     * 
     * @param value
     */
    void setDarkModeValue(int value);

    /**
     * @brief Set new origin for drawing
     * 
     * @param beginning
     */
    void rotate(Index beginning);
};

} // namespace BlackBox
