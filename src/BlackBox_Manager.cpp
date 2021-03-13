#include "BlackBox_Manager.hpp"

namespace BlackBox {
Manager::Manager()
    : m_touchpad{16, 0b1110, 1, 1, 1, 1} { // FIXME: Remove this piece of ductape
}

void Manager::init() {
    m_ldc.init();
    m_lock.init();
    m_power.init();
    m_rtc.init();
    m_touchpad.init(&m_ldc);
}

LDC& Manager::ldc() {
    return m_ldc;
}

Lock& Manager::lock() {
    return m_lock;
}

Power& Manager::power() {
    return m_power;
}

Ring& Manager::ring() {
    return m_ring;
}

RTC& Manager::rtc() {
    return m_rtc;
}

Touchpad& Manager::touchpad() {
    return m_touchpad;
}

} // namespace BlackBox
