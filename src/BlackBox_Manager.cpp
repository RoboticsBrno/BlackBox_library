#include "BlackBox_Manager.hpp"

namespace BlackBox {
Manager::Manager(/* args */) {
}

void Manager::init() {
    m_ldc.init();
    m_lock.init();
    m_power.init();
    m_rtc.init();
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

} // namespace BlackBox
