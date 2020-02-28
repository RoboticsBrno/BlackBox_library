#pragma once

#include "BlackBox_LEDring.hpp"
#include "BlackBox_encoder.hpp"
namespace BlackBox{
    class BlackBox_interface
    {
        friend class BlackBox_manager;
    private:
        BlackBox_interface(/* args */);

        BlackBox_LEDring m_LEDring;
        BlackBox_encoder m_encoder;

    public:
        ~BlackBox_interface();

        BlackBox_LEDring& LEDring() { return m_LEDring; }
        BlackBox_encoder& encoder() { return m_encoder; }
    };

}
