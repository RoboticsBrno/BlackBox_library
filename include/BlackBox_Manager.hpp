#pragma once

#include "BlackBox_LDC.hpp"
#include "BlackBox_Lock.hpp"
#include "BlackBox_Power.hpp"
#include "BlackBox_Ring.hpp"
#include "BlackBox_RTC.hpp"

namespace BlackBox {
class Manager {
private:
    Manager();

    LDC m_ldc;
    Lock m_lock;
    Power m_power;
    Ring m_ring;
    RTC m_rtc;

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

};

} // namespace BlackBox
