#include <unity.h>

#include "BlackBox_task.hpp"

using namespace BlackBox;

Task task([&](){

        while(1) {
            printf("main\n");
            TEST_ASSERT(true);
            vTaskDelay(1000 / portTICK_RATE_MS);
        }
    }, "task", 2048, 1);
    
static void test_task() {
    
}

extern "C" {
void app_main() {
    UNITY_BEGIN();
    RUN_TEST(test_task);
    UNITY_END();
    // test_task();
}
}