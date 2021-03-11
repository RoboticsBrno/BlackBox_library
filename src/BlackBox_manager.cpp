#include "BlackBox_manager.hpp"

namespace BlackBox {
BlackBox_manager::BlackBox_manager(/* args */) {
}

void BlackBox_manager::init() {
    m_ldc.init();
    m_lock.init();
    m_power.init();
    m_rtc.init();
}

LDC& BlackBox_manager::ldc() {
    return m_ldc;
}

Lock& BlackBox_manager::lock() {
    return m_lock;
}

Power& BlackBox_manager::power() {
    return m_power;
}

Ring& BlackBox_manager::ring() {
    return m_ring;
}

RTC& BlackBox_manager::rtc() {
    return m_rtc;
}

} // namespace BlackBox
