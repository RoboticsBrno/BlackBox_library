#include "BlackBox_encoder.hpp"

namespace BlackBox
{
    void BlackBox_encoder::init(){
        m_event_queue = xQueueCreate(EV_QUEUE_LEN, sizeof(rotary_encoder_event_t));
        ESP_ERROR_CHECK(rotary_encoder_init(m_event_queue));
        memset(&m_encoder, 0, sizeof(rotary_encoder_t));
        m_encoder.pin_a = BlackBox::ENC_A_GPIO;
        m_encoder.pin_b = BlackBox::ENC_B_GPIO;
        m_encoder.pin_btn = BlackBox::ENC_SW_GPIO;
        ESP_ERROR_CHECK(rotary_encoder_add(&m_encoder));
    }

    void BlackBox_encoder::act(){
        xQueueReceive(m_event_queue, &m_event, portMAX_DELAY);
        switch (m_event.type){
            case RE_ET_BTN_PRESSED:
                    m_on_press(m_event);
                break;
            case RE_ET_BTN_RELEASED:
                    m_on_release(m_event);
                break;
            case RE_ET_BTN_CLICKED:
                    m_on_click(m_event);
                break;
            case RE_ET_BTN_LONG_PRESSED:
                    m_on_long_press(m_event);
                break;
            case RE_ET_CHANGED:
                    m_on_value_change(m_event);
                break;
        }
    }

    void BlackBox_encoder::setFunction(rotary_encoder_event_type_t event, encoder_function_t function){
        switch (event){
            case RE_ET_BTN_PRESSED:
                    m_on_press = function;
                break;
            case RE_ET_BTN_RELEASED:
                    m_on_release = function;
                break;
            case RE_ET_BTN_CLICKED:
                    m_on_click = function;
                break;
            case RE_ET_BTN_LONG_PRESSED:
                    m_on_long_press = function;
                break;
            case RE_ET_CHANGED:
                    m_on_value_change = function;
                break;
        }
    }

    rotary_encoder_event_t &BlackBox_encoder::receiveNewEvent(){
        xQueueReceive(m_event_queue, &m_event, portMAX_DELAY);
        return getLastEvent();
    }
} // namespace BlackBox