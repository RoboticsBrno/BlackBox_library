#pragma once

#include "BlackBox_pinout.hpp"
#include "BlackBox_RTC.hpp"
#include <cstdint>

namespace BlackBox {
class BlackBox_manager {
private:
    BlackBox_manager();

    BlackBox_interface m_interface;
    BlackBox_RTC m_rtc;

public:
    BlackBox_manager(BlackBox_manager const&) = delete;
    void operator=(BlackBox_manager const&) = delete;

    static BlackBox_manager& singleton()
    {
        static BlackBox_manager instance;
        return instance;
    }
    BlackBox_interface& interface() { return m_interface; }
    BlackBox_RTC& rtc() { return m_rtc; }
};

} // namespace BlackBox
