#pragma once

#include "BlackBox_LEDring.hpp"
#include "BlackBox_encoder.hpp"
#include "BlackBox_page.hpp"
#include <cstdint>
#include <vector>

namespace BlackBox
{
    class BlackBox_interface
    {
        friend class BlackBox_manager;
    private:
        BlackBox_interface();

        BlackBox_LEDring m_LEDring;
        BlackBox_encoder m_encoder;
        std::vector< BlackBox_page > m_pages;

        uint8_t m_active_page = 0;

    public:
        ~BlackBox_interface();

        BlackBox_LEDring& LEDring() { return m_LEDring; }
        BlackBox_encoder& encoder() { return m_encoder; }

        index_t storePage( BlackBox_page& i_page );
        void deletePage( index_t i_index );
        void activatePage( index_t i_index );
        void activateClock();
    };

}
