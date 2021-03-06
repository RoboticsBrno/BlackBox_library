#pragma once

#include <cstdint>
#include <functional>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// FIXME: Fix this mess
// movability:
//  - What happens with the original function called by trampoline after move?
// detach:
//  - What happens with the original function called by trampoline after detaching the RAII?

namespace BlackBox {

class Task {
private:
    TaskHandle_t m_taskHandle = nullptr;
    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;
    std::function<void()> m_function;

public:
    Task(std::function<void()> i_function,
        const char* const i_name,
        uint32_t i_stackDepth,
        UBaseType_t i_priority);
    Task(std::function<void()> i_function,
        const char* const i_name,
        uint32_t i_stackDepth,
        UBaseType_t i_priority,
        const uint32_t i_coreID);
    explicit Task(TaskHandle_t* const i_taskHnadle);
    Task(Task&&);
    Task& operator=(Task&&);
    ~Task();

    static void trampoline(void* i_functionPtr);

    TaskHandle_t* raw();
    
    // for now this breaks the program
    // void detach();

    UBaseType_t priority();
    static UBaseType_t priority(const TaskHandle_t* i_taskHandle);
    static UBaseType_t IRAM_ATTR priorityFromISR(const TaskHandle_t* i_taskHandle);

    void setPriority(UBaseType_t i_priority);
    static void setPriority(const TaskHandle_t* i_taskHandle, UBaseType_t i_priority);

    eTaskState state();
    static eTaskState state(const TaskHandle_t* i_taskHandle);

    void resume();
    static void resume(const TaskHandle_t* i_taskHandle);
    static BaseType_t IRAM_ATTR resumeFromISR(const TaskHandle_t* i_taskHandle);
    static BaseType_t resumeAll();

    void suspend();
    static void suspend(const TaskHandle_t* i_taskHandle);
    static void suspendAll();

    static void list(char* o_buffer);
};
} // namespace BlackBox
