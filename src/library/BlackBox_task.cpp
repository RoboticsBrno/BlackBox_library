#include "library/BlackBox_task.hpp"

namespace BlackBox {

Task::Task(std::function<void()> i_function,
    const char* const i_name,
    uint32_t i_stackDepth,
    UBaseType_t i_priority) {
    m_function = i_function;
    xTaskCreate(&trampoline, i_name, i_stackDepth, static_cast<void*>(&m_function), i_priority, &m_taskHandle);
}

Task::Task(std::function<void()> i_function,
    const char* const i_name,
    uint32_t i_stackDepth,
    UBaseType_t i_priority,
    const uint32_t i_coreID) {
    m_function = i_function;
    xTaskCreatePinnedToCore(&trampoline, i_name, i_stackDepth, static_cast<void*>(&m_function), i_priority, &m_taskHandle, i_coreID);
}

Task::Task(TaskHandle_t* const i_taskHnadle) {
    m_taskHandle = *i_taskHnadle;
}

Task::Task(Task&& other)
    : m_taskHandle(std::exchange(other.m_taskHandle, nullptr)) {
}

Task& Task::operator=(Task&& other) {
    if (this != &other) {
        if (m_taskHandle == nullptr)
            vTaskDelete(m_taskHandle);
        m_taskHandle = std::exchange(other.m_taskHandle, nullptr);
    }
    return *this;
}

Task::~Task() {
    if(m_taskHandle != nullptr)
        vTaskDelete(m_taskHandle);
}

void Task::trampoline(void* i_functionPtr) {
    auto function = *static_cast<std::function<void()>*>(i_functionPtr);
    function();
}

TaskHandle_t* Task::raw() {
    return &m_taskHandle;
}

// for now this breaks the program
// void Task::detach() { 
//     m_taskHandle = nullptr;
// }

UBaseType_t Task::priority() {
    return uxTaskPriorityGet(m_taskHandle);
}

UBaseType_t priority(const TaskHandle_t* i_taskHandle) {
    return uxTaskPriorityGet(*i_taskHandle);
}

UBaseType_t IRAM_ATTR priorityFromISR(const TaskHandle_t* i_taskHandle) {
    return uxTaskPriorityGetFromISR(*i_taskHandle);
}

void Task::setPriority(UBaseType_t i_priority) {
    vTaskPrioritySet(nullptr, i_priority);
}

void Task::setPriority(const TaskHandle_t* i_taskHandle, UBaseType_t i_priority) {
    vTaskPrioritySet(*i_taskHandle, i_priority);
}

eTaskState Task::state() {
    return eTaskGetState(m_taskHandle);
}

eTaskState Task::state(const TaskHandle_t* i_taskHandle) {
    return eTaskGetState(*i_taskHandle);
}

void Task::resume() {
    vTaskResume(m_taskHandle);
}

void Task::resume(const TaskHandle_t* i_taskHandle) {
    vTaskResume(*i_taskHandle);
}

BaseType_t IRAM_ATTR Task::resumeFromISR(const TaskHandle_t* i_taskHandle) {
    return xTaskResumeFromISR(*i_taskHandle);
}

BaseType_t Task::resumeAll() {
    return xTaskResumeAll();
}

void Task::suspend() {
    vTaskSuspend(m_taskHandle);
}

void Task::suspend(const TaskHandle_t* i_taskHandle) {
    vTaskSuspend(*i_taskHandle);
}

void Task::suspendAll() {
    vTaskSuspendAll();
}

void Task::list(char* o_buffer) {
    vTaskList(o_buffer);
}

} // namespace BlackBox
