#include "BlackBox_manager.hpp"

namespace BlackBox
{
    BlackBox_manager::BlackBox_manager(/* args */): m_leds(LED_WS2812B, LED_COUNT, DATA_PIN, CHANNEL, DoubleBuffer )
    {
    }
    
} // namespace BlackBox
