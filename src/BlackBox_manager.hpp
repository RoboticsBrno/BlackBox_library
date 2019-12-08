#pragma once

#include <SmartLeds.h>
#include "BlackBox_pinout.hpp"

namespace BlackBox
{
    class BlackBox_manager
    {
    private:
        

        SmartLed m_leds;

    public:
        BlackBox_manager();
        void operator=(BlackBox_manager const &) = delete;

        static BlackBox_manager &get()
        {
            static BlackBox_manager instance;
            return instance;
        }
        
    };

} // namespace BlackBox
