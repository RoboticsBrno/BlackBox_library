/**
 * @file BlackBox_Ring.hpp
 * @author Tomáš Rohlínek (haberturdeur)
 * \~czech @brief Knihovna pro ovládání LED kruhu
 * \~english @brief Library for controling the LED Ring
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
 * \~czech @brief Datový typ pro práci s pozicí na LED kruhu
 * \~english @brief Data type for handling polar coords based on LED Ring model
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
 * \~czech @brief Třída pro ovládání LED kruhu
 * \~english @brief Class for controling the LED Ring
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
     * \~czech @brief Vytvoří nový Ring objekt
     * \~english @brief Construct a new Ring object
     * 
     * @param count Count of Smart LEDs
     */
    Ring(int count = ledCount);

    /**
     * \~czech @brief Vykreslí nachystaný snímek
     * \~english @brief Draw buffered frame
     */
    void show();

    /**
     * \~czech @brief Počká na dokončení vykreslování
     * \~english @brief Wait for drawing to finish
     * 
     * @note Is called from show() anyway
     */
    void wait();
    // void show(Page&); // FIXME: What implementation would be best (page)?

    /**
     * \~czech @brief Nakreslí oblouk na chystaný snímek
     * \~english @brief Draw arc onto frame
     * 
     * @param colour
     * @param beginnig
     * @param ending
     * @param arcType
     */
    void drawArc(Rgb colour, Index beginnig, Index ending, ArcType arcType = ArcType::ShorterDistance);

    /**
     * \~czech @brief Nakreslí kruh na chystaný snímek
     * \~english @brief Draw full circle onto frame
     * 
     * @param colour
     */
    void drawCircle(Rgb colour);

    /**
     * \~czech @brief Nakreslí snímek z uložené předlohy
     * \~english @brief Draw whole frame
     * 
     * @param buffer 
     */
    void draw(std::unique_ptr< Rgb[] > buffer);

    /**
     * \~czech @brief Vyčistí aktivní snímek
     * \~english @brief Clear active frame
     * 
     * @note Just draws black circle
     */
    void clear();

    const Rgb& operator[](const Index&) const;
    Rgb& operator[](const Index&);

    /**
     * \~czech @brief Povolí tmavý režim
     * \~english @brief Enable dark mode
     */
    void enableDarkMode();

    /**
     * \~czech @brief Zakáže tmavý režim
     * \~english @brief Disable dark mode
     */
    void disableDarkMode();

    /**
     * \~czech @brief Nastaví maximální hodnotu pro LED v tmavém režimu
     * \~english @brief Set dark mode value
     * 
     * @param value
     */
    void setDarkModeValue(int value);

    /**
     * \~czech @brief Nastaví nový počátek (pozici 0) => otočí displej
     * \~english @brief Set new origin for drawing
     * 
     * @param beginning
     */
    void rotate(Index beginning);
};

} // namespace BlackBox
