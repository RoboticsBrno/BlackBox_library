#pragma once

#include "BlackBox_LDC.hpp"
#include "BlackBox_Lock.hpp"
#include "BlackBox_Power.hpp"
#include "BlackBox_Ring.hpp"
#include "BlackBox_RTC.hpp"

namespace BlackBox {
class BlackBox_manager {
private:
    BlackBox_manager();

    LDC m_ldc;
    Lock m_lock;
    Power m_power;
    Ring m_ring;
    RTC m_rtc;

public:
    BlackBox_manager(BlackBox_manager const&) = delete;
    void operator=(BlackBox_manager const&) = delete;

    static BlackBox_manager& singleton()
    {
        static BlackBox_manager instance;
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
