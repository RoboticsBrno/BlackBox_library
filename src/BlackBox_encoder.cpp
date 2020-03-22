#include "BlackBox_encoder.hpp"

namespace BlackBox {
void BlackBox_encoder::init()
{
    m_eventQueue = xQueueCreate(EV_QUEUE_LEN, sizeof(rotary_encoder_event_t));
    ESP_ERROR_CHECK(rotary_encoder_init(m_eventQueue));
    memset(&m_encoder, 0, sizeof(rotary_encoder_t));
    m_encoder.pin_a = BlackBox::ENC_A_GPIO;
    m_encoder.pin_b = BlackBox::ENC_B_GPIO;
    m_encoder.pin_btn = BlackBox::ENC_SW_GPIO;
    ESP_ERROR_CHECK(rotary_encoder_add(&m_encoder));
}

void BlackBox_encoder::act()
{
    xQueueReceive(m_eventQueue, &m_lastEvent, portMAX_DELAY);
    switch (m_lastEvent.type) {
    case RE_ET_BTN_PRESSED:
        m_onPress(m_lastEvent);
        break;
    case RE_ET_BTN_RELEASED:
        m_onRelease(m_lastEvent);
        break;
    case RE_ET_BTN_CLICKED:
        m_onClick(m_lastEvent);
        break;
    case RE_ET_BTN_LONG_PRESSED:
        m_onLongPress(m_lastEvent);
        break;
    case RE_ET_CHANGED:
        m_onValueChange(m_lastEvent);
        break;
    }
}

void BlackBox_encoder::setFunction(rotary_encoder_event_type_t event, encoder_function_t function)
{
    switch (event) {
    case RE_ET_BTN_PRESSED:
        m_onPress = function;
        break;
    case RE_ET_BTN_RELEASED:
        m_onRelease = function;
        break;
    case RE_ET_BTN_CLICKED:
        m_onClick = function;
        break;
    case RE_ET_BTN_LONG_PRESSED:
        m_onLongPress = function;
        break;
    case RE_ET_CHANGED:
        m_onValueChange = function;
        break;
    }
}

rotary_encoder_event_t& BlackBox_encoder::receiveNewEvent()
{
    xQueueReceive(m_eventQueue, &m_lastEvent, portMAX_DELAY);
    return lastEvent();
}
} // namespace BlackBox