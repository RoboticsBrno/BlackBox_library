#pragma once

#include "BlackBox_interface.hpp"
#include "BlackBox_pinout.hpp"


namespace BlackBox
{
    class BlackBox_manager
    {
    private:
        BlackBox_manager();

        BlackBox_interface m_interface;

    public:
        BlackBox_manager(BlackBox_manager const&) = delete;
        void operator=(BlackBox_manager const &) = delete;

        static BlackBox_manager& get()
        {
            static BlackBox_manager instance;
            return instance;
        }
        
        BlackBox_interface& interface(){ return m_interface; } 
        
    };

} // namespace BlackBox
