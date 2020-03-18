#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>
#include <encoder.h>
#include <esp_idf_lib_helpers.h>
#include <functional>
#include <cstdint>

#include "BlackBox_pinout.hpp"

#define EV_QUEUE_LEN 5

namespace BlackBox
{

    typedef  std::function< void( rotary_encoder_event_t ) > encoder_function_t;

    class BlackBox_encoder
    {
        friend class BlackBox_interface;
    private:
        BlackBox_encoder() = default;

        QueueHandle_t m_event_queue;
        rotary_encoder_t m_encoder;
        rotary_encoder_event_t m_event;

        encoder_function_t m_on_press           = [] ( rotary_encoder_event_t e ) { printf( "Button pressed\n" ); };
        encoder_function_t m_on_release         = [] ( rotary_encoder_event_t e ) { printf( "Button released\n" ); };
        encoder_function_t m_on_click           = [] ( rotary_encoder_event_t e ) { printf( "Button clicked\n" ); };
        encoder_function_t m_on_long_press      = [] ( rotary_encoder_event_t e ) { printf( "Button long pressed\n" ); };
        encoder_function_t m_on_value_change    = [] ( rotary_encoder_event_t e ) { printf( "Encoder value change by %d\n", e.diff ); };
    public:
        void init();
        void act();

        rotary_encoder_event_t& getLastEvent() { return m_event; }
        rotary_encoder_event_t& receiveNewEvent();

        QueueHandle_t& getQueueHandle() { return m_event_queue; };


        void setFunction( rotary_encoder_event_type_t event, encoder_function_t function );

        ~BlackBox_encoder() = default;
    };

} // namespace BlackBox
