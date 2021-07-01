#include "library/BlackBox_Manager.hpp"

#include "library/BlackBox_pinout.hpp"

namespace BlackBox {
Manager::Manager()
#if defined(BB_TOUCHPAD) && defined (BB_DOORS)
    : m_touchpad{16, 0b1110, 1, 1, 1, 1} // FIXME: Remove this piece of ductape
    , m_doors{Door(Pins::Doors::DoorPins[0], LEDC_TIMER_0, LEDC_CHANNEL_0), Door(Pins::Doors::DoorPins[1], LEDC_TIMER_1, LEDC_CHANNEL_1), Door(Pins::Doors::DoorPins[2], LEDC_TIMER_2, LEDC_CHANNEL_2), Door(Pins::Doors::DoorPins[3], LEDC_TIMER_3, LEDC_CHANNEL_3)}
#elif defined(BB_TOUCHPAD)
    : m_touchpad{16, 0b1110, 1, 1, 1, 1} // FIXME: Remove this piece of ductape
#elif defined(BB_DOORS)
    : m_doors{Door(Pins::Doors::DoorPins[0], LEDC_TIMER_0, LEDC_CHANNEL_0), Door(Pins::Doors::DoorPins[1], LEDC_TIMER_1, LEDC_CHANNEL_1), Door(Pins::Doors::DoorPins[2], LEDC_TIMER_2, LEDC_CHANNEL_2), Door(Pins::Doors::DoorPins[3], LEDC_TIMER_3, LEDC_CHANNEL_3)}
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

#ifdef BB_DOORS
for (auto& i : m_doors)
    i.init();
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

#ifdef BB_DOORS
Door& Manager::door(int number) {
    return m_doors.at(number);
}

std::array<Door, 4>& Manager::doors() {
    return m_doors;
}
#endif

} // namespace BlackBox
