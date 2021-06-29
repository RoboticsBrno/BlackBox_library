#include "library/BlackBox_Manager.hpp"

namespace BlackBox {
Manager::Manager()
#ifdef BB_TOUCHPAD
    : m_touchpad{16, 0b1110, 1, 1, 1, 1} // FIXME: Remove this piece of ductape
#endif
{
}

void Manager::init() {
#ifdef BB_LDC
    m_ldc.init();
#endif

#ifdef BB_LOCK
    m_lock.init();
#endif

#ifdef BB_POWER
    m_power.init();
#endif

#ifdef BB_RTC
    m_rtc.init();
#endif

#ifdef BB_TOUCHPAD
    m_touchpad.init(&m_ldc);
#endif
}

#ifdef BB_LDC
LDC& Manager::ldc() {
    return m_ldc;
}
#endif

#ifdef BB_LOCK
Lock& Manager::lock() {
    return m_lock;
}
#endif

#ifdef BB_POWER
Power& Manager::power() {
    return m_power;
}
#endif

#ifdef BB_RING
Ring& Manager::ring() {
    return m_ring;
}
#endif

#ifdef BB_RTC
RTC& Manager::rtc() {
    return m_rtc;
}
#endif

#ifdef BB_TOUCHPAD
Touchpad& Manager::touchpad() {
    return m_touchpad;
}
#endif

} // namespace BlackBox
