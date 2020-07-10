#include "BlackBox_task.hpp"

namespace BlackBox {

Task::Task(std::function<void()> i_function,
    const char* const i_name,
    uint32_t i_stackDepth,
    UBaseType_t i_priority) {
    xTaskCreate(trampoline, i_name, i_stackDepth, &i_function, i_priority, &m_taskHandle);
}

Task::Task(std::function<void()> i_function,
    const char* const i_name,
    uint32_t i_stackDepth,
    UBaseType_t i_priority,
    const uint32_t i_coreID) {
    xTaskCreatePinnedToCore(trampoline, i_name, i_stackDepth, &i_function, i_priority, &m_taskHandle, i_coreID);
}

Task::Task(TaskHandle_t* const i_taskHnadle) {
    m_taskHandle = *i_taskHnadle;
}

Task::~Task() {
    vTaskDelete(m_taskHandle);
}

inline void Task::trampoline(void* i_functionPtr) {
    auto function = static_cast<std::function<void()>*>(i_functionPtr);
    (*function)();
}

inline TaskHandle_t* Task::raw() {
    return &m_taskHandle;
}

inline void Task::detach() {
    m_taskHandle = NULL;
}

inline UBaseType_t Task::priority() {
    return uxTaskPriorityGet(m_taskHandle);
}

inline UBaseType_t priority(const TaskHandle_t* i_taskHandle) {
    return uxTaskPriorityGet(*i_taskHandle);
}

inline UBaseType_t IRAM_ATTR priorityFromISR(const TaskHandle_t* i_taskHandle) {
    return uxTaskPriorityGetFromISR(*i_taskHandle);
}

inline void Task::setPriority(UBaseType_t i_priority) {
    vTaskPrioritySet(NULL, i_priority);
}

inline void Task::setPriority(const TaskHandle_t* i_taskHandle, UBaseType_t i_priority) {
    vTaskPrioritySet(*i_taskHandle, i_priority);
}

inline eTaskState Task::state() {
    return eTaskGetState(m_taskHandle);
}

inline eTaskState Task::state(const TaskHandle_t* i_taskHandle) {
    return eTaskGetState(*i_taskHandle);
}

inline void Task::resume() {
    vTaskResume(m_taskHandle);
}

inline void Task::resume(const TaskHandle_t* i_taskHandle) {
    vTaskResume(*i_taskHandle);
}

inline BaseType_t IRAM_ATTR Task::resumeFromISR(const TaskHandle_t* i_taskHandle) {
    return xTaskResumeFromISR(*i_taskHandle);
}

inline BaseType_t Task::resumeAll() {
    return xTaskResumeAll();
}

inline void Task::suspend() {
    vTaskSuspend(m_taskHandle);
}

inline void Task::suspend(const TaskHandle_t* i_taskHandle) {
    vTaskSuspend(*i_taskHandle);
}

inline void Task::suspendAll() {
    vTaskSuspendAll();
}

inline void Task::list(char* o_buffer) {
    vTaskList(o_buffer);
}

} // namespace BlackBox
