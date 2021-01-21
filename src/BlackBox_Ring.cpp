#include "BlackBox_Ring.hpp"

#include "BlackBox_pinout.hpp"
#include <SmartLeds.h>
#include <mutex>

namespace BlackBox {
int Index::trim(int index) {
    return index % ledCount;
}

Index& Index::trimThis() {
    m_value = trim(m_value);
    return *this;
}

Index::Index(int i_index)
    : m_value(trim(i_index)) {
}

int Index::value() const {
    return m_value;
}

Index& Index::operator+=(const Index& i_other) {
    m_value += i_other.m_value;
    return trimThis();
}

Index& Index::operator-=(const Index& i_other) {
    m_value -= i_other.m_value;
    return trimThis();
}

Index& Index::operator++() {
    m_value++;
    return trimThis();
}

Index& Index::operator--() {
    m_value--;
    return trimThis();
}

Index::operator int() const {
    return m_value;
}

Ring::Ring(int i_count)
    : m_leds(LED_WS2812, i_count, Pins::Leds::Data, channel, DoubleBuffer)
    , m_buffer(new Rgb[i_count])
    , m_count(i_count)
    , m_darkModeValue(10)
    , m_isDarkModeEnabled(false)
    , m_beginning(0) {
}

void Ring::show() {
    std::scoped_lock l(m_mutex);
    Index j = m_beginning;
    for (int i = 0; i < m_count; i++) {
        m_leds[j] = m_buffer[i];
        if (m_isDarkModeEnabled)
            m_leds[j].stretchChannelsEvenly(m_darkModeValue);
        ++j;
    }
    while (!m_leds.wait())
        m_leds.show();
}

// void Ring::drawCircle(Rgb i_rgb) {
//     std::scoped_lock l(m_mutex);
//     for (int i = 0; i < m_count; i++) {
//         m_leds[i] = i_rgb;
//         m_leds[i].stretchChannelsEvenly(m_darkModeValue);
//     }
//     while (!m_leds.wait())
//         m_leds.show();
// }

// void Ring::draw(std::unique_ptr<Rgb[]> i_buffer) {
// }

void Ring::enableDarkMode() {
    std::scoped_lock l(m_mutex);
    m_isDarkModeEnabled = true;
}

void Ring::disableDarkMode() {
    std::scoped_lock l(m_mutex);
    m_isDarkModeEnabled = false;
}

void Ring::setDarkModeValue(int i_value) {
    std::scoped_lock l(m_mutex);
    m_darkModeValue = i_value;
}

const Rgb& Ring::operator[](Index i_index) const {
    std::scoped_lock l(m_mutex);
    return m_buffer[i_index];
}

Rgb& Ring::operator[](Index i_index) {
    std::scoped_lock l(m_mutex);
    return m_buffer[i_index];
}

void Ring::rotate(Index i_beginning) {
    std::scoped_lock l(m_mutex);
    m_beginning = i_beginning;
}

} // namespace BlackBox