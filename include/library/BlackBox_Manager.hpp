/**
 * @file BlackBox_Manger.hpp
 * @author Tomáš Rohlínek (haberturdeur)
 * \~czech @brief Knihovna řídící všechny ostatní knihovny
 * \~english @brief Library controling all of the other hardware libraries
 */

#pragma once

#include "Dsp.hpp"

#include "library/BlackBox_LDC.hpp"
#include "library/BlackBox_Lock.hpp"
#include "library/BlackBox_Power.hpp"
#include "library/BlackBox_Ring.hpp"
#include "library/BlackBox_RTC.hpp"
#include "library/BlackBox_Touchpad.hpp"

namespace BlackBox {
/**
 * \~czech @brief Třída řídící všechny ostatní knihovny hardwarové abstrakce
 * \~english @brief Class controling all of the other hardware abstraction classes
 */
class Manager {
private:
    Manager();
#ifdef BB_LDC
    LDC m_ldc;
#endif

#ifdef BB_LOCK
    Lock m_lock;
#endif

#ifdef BB_POWER
    Power m_power;
#endif

#ifdef BB_RING
    Ring m_ring;
#endif

#ifdef BB_RTC
    RTC m_rtc;
#endif

#ifdef BB_TOUCHPAD
    Touchpad m_touchpad;
#endif

public:
    Manager(Manager const&) = delete;
    void operator=(Manager const&) = delete;

    static Manager& singleton()
    {
        static Manager instance;
        return instance;
    }

    void init();

#ifdef BB_LDC
    LDC& ldc();
#endif

#ifdef BB_LOCK
    Lock& lock();
#endif

#ifdef BB_POWER
    Power& power();
#endif

#ifdef BB_RING
    Ring& ring();
#endif

#ifdef BB_RTC
    RTC& rtc();
#endif

#ifdef BB_TOUCHPAD
    Touchpad& touchpad();
#endif

};

} // namespace BlackBox
