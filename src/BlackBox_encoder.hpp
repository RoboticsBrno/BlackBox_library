#pragma once

#include <cstdint>
#include <encoder.h>
#include <esp_idf_lib_helpers.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <functional>
#include <string.h>

#include "BlackBox_pinout.hpp"

#define EV_QUEUE_LEN 5

namespace BlackBox {

typedef std::function<void(rotary_encoder_event_t)> encoder_function_t;

class BlackBox_encoder {
    friend class BlackBox_interface;

private:
    BlackBox_encoder() = default;

    QueueHandle_t m_eventQueue;
    rotary_encoder_t m_encoder;
    rotary_encoder_event_t m_lastEvent;

    encoder_function_t m_onPress = [](rotary_encoder_event_t e) { printf("Button pressed\n"); };
    encoder_function_t m_onRelease = [](rotary_encoder_event_t e) { printf("Button released\n"); };
    encoder_function_t m_onClick = [](rotary_encoder_event_t e) { printf("Button clicked\n"); };
    encoder_function_t m_onLongPress = [](rotary_encoder_event_t e) { printf("Button long pressed\n"); };
    encoder_function_t m_onValueChange = [](rotary_encoder_event_t e) { printf("Encoder value change by %d\n", e.diff); };

public:
    void init();
    void act();

    rotary_encoder_event_t& lastEvent() { return m_lastEvent; }
    rotary_encoder_event_t& receiveNewEvent();

    QueueHandle_t& getQueueHandle() { return m_eventQueue; };

    void setFunction(rotary_encoder_event_type_t event, encoder_function_t function);

    ~BlackBox_encoder() = default;
};

} // namespace BlackBox
