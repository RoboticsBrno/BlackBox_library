/**
 * @file BlackBox_Manger.hpp
 * @author Tomáš Rohlínek (haberturdeur)
 * @brief Library controling all of the other hardware libraries
 */

#pragma once

#include "library/BlackBox_LDC.hpp"
#include "library/BlackBox_Lock.hpp"
#include "library/BlackBox_Power.hpp"
#include "library/BlackBox_Ring.hpp"
#include "library/BlackBox_RTC.hpp"
#include "library/BlackBox_Touchpad.hpp"

namespace BlackBox {
/**
 * @brief Class controling all of the other hardware abstraction classes
 */
class Manager {
private:
    Manager();

    LDC m_ldc;
    Lock m_lock;
    Power m_power;
    Ring m_ring;
    RTC m_rtc;
    Touchpad m_touchpad;

public:
    Manager(Manager const&) = delete;
    void operator=(Manager const&) = delete;

    static Manager& singleton()
    {
        static Manager instance;
        return instance;
    }

    void init();

    LDC& ldc();
    Lock& lock();
    Power& power();
    Ring& ring();
    RTC& rtc();
    Touchpad& touchpad();

};

} // namespace BlackBox
