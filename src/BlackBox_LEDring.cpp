
#include "BlackBox_LEDring.hpp"
#include <map>

namespace BlackBox {
BlackBox_LEDring::BlackBox_LEDring()
    : m_leds(LED_WS2812B, BlackBox::LED_COUNT, BlackBox::LED_DATA_GPIO, BlackBox::CHANNEL, DoubleBuffer) {
}
BlackBox_LEDring::BlackBox_LEDring(bool i_initialize)
    : m_leds(LED_WS2812B, BlackBox::LED_COUNT, BlackBox::LED_DATA_GPIO, BlackBox::CHANNEL, DoubleBuffer) {
    if (i_initialize)
        init();
}

void BlackBox_LEDring::pass(Rgb i_buffer[BlackBox::LED_COUNT]) {
    for (int i = 0; i < BlackBox::LED_COUNT; i++) {
        m_leds[i] = i_buffer[i];
    }
}

uint8_t BlackBox_LEDring::getIntenzity() {
    return m_darkMode ? m_darkModeIntenzity : m_intenzity;
}

void BlackBox_LEDring::writeLEDstate() {
    gpio_set_level(BlackBox::LED_TOOGLE_GPIO, m_ledState);
}

SmartLed& BlackBox_LEDring::leds() {
    return m_leds;
}

void BlackBox_LEDring::init() {
    gpio_pad_select_gpio(BlackBox::LED_TOOGLE_GPIO);
    gpio_set_direction(BlackBox::LED_TOOGLE_GPIO, GPIO_MODE_OUTPUT);
    writeLEDstate();
}

void BlackBox_LEDring::toogleLEDring() {
    m_ledState = !m_ledState;
    writeLEDstate();
}

void BlackBox_LEDring::toogleLEDring(bool i_ledState) {
    m_ledState = i_ledState;
    writeLEDstate();
}

void BlackBox_LEDring::setIntenzity(uint8_t i_intenzity) {
    m_intenzity = i_intenzity;
}

void BlackBox_LEDring::setDarkModeIntenzity(uint8_t i_intenzity) {
    m_darkModeIntenzity = i_intenzity;
}

void BlackBox_LEDring::toogleDarkMode() {
    m_darkMode = !m_darkMode;
}

void BlackBox_LEDring::toogleDarkMode(bool i_darkMode) {
    m_darkMode = i_darkMode;
}

void BlackBox_LEDring::setSecondsColor(Rgb i_color) {
    m_secondsColor = i_color;
}

void BlackBox_LEDring::setMinutesColor(Rgb i_color) {
    m_minutesColor = i_color;
}

void BlackBox_LEDring::setHoursColor(Rgb i_color) {
    m_hoursColor = i_color;
}

void BlackBox_LEDring::setLayerOpacity(uint8_t i_opacity) {
    m_layerOpacity = i_opacity;
}

void BlackBox_LEDring::setFirstPointOpacity(uint8_t i_opacity) {
    m_firstPointOpacity = i_opacity;
}

void BlackBox_LEDring::toogleForceFirstPointOpacity() {
    m_forceFirstPointOpacity = !m_forceFirstPointOpacity;
}

void BlackBox_LEDring::toogleForceFirstPointOpacity(bool i_forceFirstPointOpacity) {
    m_forceFirstPointOpacity = i_forceFirstPointOpacity;
}

void BlackBox_LEDring::toogle12hrMode() {
    m_12hrMode = !m_12hrMode;
}

void BlackBox_LEDring::toogle12hrMode(bool i_12hrMode) {
    m_12hrMode = i_12hrMode;
}

void BlackBox_LEDring::prepare(Rgb i_buffer[BlackBox::LED_COUNT]) {
    for (int i = 0; i < BlackBox::LED_COUNT; i++) {
        i_buffer[i].stretchChannelsEvenly(getIntenzity());
    }
    pass(i_buffer);
}

void BlackBox_LEDring::prepare(BlackBox::Time_t i_time) {
    Rgb buffer[BlackBox::LED_COUNT];
    buffer[(i_time.getHours() * 5) % 60] += m_hoursColor;
    buffer[i_time.getMinutes() % 60] += m_minutesColor;
    buffer[i_time.getSeconds() % 60] += m_secondsColor;
    prepare(buffer);
}

void BlackBox_LEDring::clear() {
    Rgb buffer[BlackBox::LED_COUNT];
    for (int i = 0; i < BlackBox::LED_COUNT; i++) {
        buffer[i] = Rgb(0, 0, 0);
    }
    show(buffer);
}

void BlackBox_LEDring::show() {
    m_leds.show();
}

void BlackBox_LEDring::show(Rgb i_buffer[BlackBox::LED_COUNT]) {
    prepare(i_buffer);
    show();
}

void BlackBox_LEDring::showTime(BlackBox::Time_t i_time) {
    prepare(i_time);
    show();
}

void BlackBox_LEDring::showCircle(Rgb i_color, uint8_t i_opacity, bool i_clear) {
    Rgb buffer[BlackBox::LED_COUNT];
    for (int i = 0; i < BlackBox::LED_COUNT; i++) {
        buffer[i] = i_color;
    }
    prepare(buffer);
    show();
}

void BlackBox_LEDring::showLevel(int8_t i_level, Rgb i_color, uint8_t i_opacity, int i_clockwise, bool i_clear) {
    if (i_clear)
        clear();
}
} // namespace BlackBox
