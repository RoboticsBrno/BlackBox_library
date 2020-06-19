#include "BlackBox_encoder.hpp"

namespace BlackBox {

void BlackBox_encoder::initEncoder() {
    ESP_LOGD(m_tag, "Initializing encoder.");
    ESP_LOGI(m_tag, "Locking mutex.");
    std::lock_guard<std::recursive_mutex> encoderLock(m_mutex);
    ESP_ERROR_CHECK(pcnt_unit_config(&pcntConfig));

    ESP_ERROR_CHECK(pcnt_set_filter_value(PCNT_UNIT, 100));
    ESP_ERROR_CHECK(pcnt_filter_enable(PCNT_UNIT));

    /* Set threshold 0 and 1 values and enable events to watch */
    // pcnt_set_event_value(PCNT_UNIT, PCNT_EVT_THRES_1, PCNT_THRESH1_VAL);
    // pcnt_event_enable(PCNT_UNIT, PCNT_EVT_THRES_1);
    // pcnt_set_event_value(PCNT_UNIT, PCNT_EVT_THRES_0, PCNT_THRESH0_VAL);
    // pcnt_event_enable(PCNT_UNIT, PCNT_EVT_THRES_0);
    // /* Enable events on zero, maximum and minimum limit values */
    // pcnt_event_enable(PCNT_UNIT, PCNT_EVT_ZERO);
    // pcnt_event_enable(PCNT_UNIT, PCNT_EVT_H_LIM);
    // pcnt_event_enable(PCNT_UNIT, PCNT_EVT_L_LIM);

    ESP_ERROR_CHECK(pcnt_counter_pause(PCNT_UNIT));
    ESP_ERROR_CHECK(pcnt_counter_clear(PCNT_UNIT));

    ESP_ERROR_CHECK(pcnt_counter_resume(PCNT_UNIT));
}

void BlackBox_encoder::initButton() {
    ESP_LOGD(m_tag, "Initializing button.");
    ESP_LOGI(m_tag, "Locking mutex.");
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    ESP_ERROR_CHECK(gpio_config(&gpioConfig));
}

void BlackBox_encoder::init() {
    initEncoder();
    initButton();
}

esp_err_t BlackBox_encoder::readEncoder() {
    ESP_LOGD(m_tag, "Reading encoder.");
    ESP_LOGI(m_tag, "Locking mutex.");
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    int16_t count = 0;
    esp_err_t err = pcnt_get_counter_value(PCNT_UNIT, &count);
    uint8_t value = (count >= 0) ? count : (60 + count);
    if (m_value != value) {
        m_value = value;
    }
    return err;
}

void BlackBox_encoder::readButton() {
    ESP_LOGD(m_tag, "Reading button.");
    ESP_LOGI(m_tag, "Locking mutex.");
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    uint32_t now = esp_log_timestamp();
    uint32_t elapsed = now - m_lastCheck;
    m_rawButtonState = !gpio_get_level(ENC_SW_GPIO);
    }
    //FIXME: Write function for reading button.
    m_lastCheck = now;
}

esp_err_t BlackBox_encoder::read() {
    readButton();
    return readEncoder();
}

}

}

index_t BlackBox_encoder::valueIndex(bool i_renew) {
    if (i_renew)
        ESP_ERROR_CHECK(readEncoder());
    ESP_LOGI(m_tag, "Locking mutex.");
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return index_t(m_value);
}

uint8_t BlackBox_encoder::value(bool i_renew) {
    if (i_renew)
        ESP_ERROR_CHECK(readEncoder());
    ESP_LOGI(m_tag, "Locking mutex.");
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return m_value;
}

button_state_t BlackBox_encoder::buttonState(bool i_renew) {
    if (i_renew)
        readButton();
    ESP_LOGI(m_tag, "Locking mutex.");
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return m_buttonState;
}
void BlackBox_encoder::setReleaseTimeout(uint32_t i_releaseTimeout) {
    ESP_LOGI(m_tag, "Locking mutex.");
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_releaseTimeout = i_releaseTimeout;
}

void BlackBox_encoder::setDebounceTime(uint32_t i_debounceTime) {
    ESP_LOGI(m_tag, "Locking mutex.");
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_debounceTime = i_debounceTime;
}

void BlackBox_encoder::setLongClickTimeout(uint32_t i_longClickTimeout) {
    ESP_LOGI(m_tag, "Locking mutex.");
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_longClickTimeout = i_longClickTimeout;
}

void BlackBox_encoder::setDoubleClickTimeout(uint32_t i_doubleClickTimeout) {
    ESP_LOGI(m_tag, "Locking mutex.");
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_doubleClickTimeout = i_doubleClickTimeout;
}

} // namespace BlackBox