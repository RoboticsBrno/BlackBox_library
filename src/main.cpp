// #ifndef UNIT_TEST
// #include "BlackBox_Manager.hpp"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

// #include "BlackBox_LDC.hpp"
// #include "BlackBox_Touchpad.hpp"

// #include "driver/gpio.h"

// #include <cstdlib>

// using namespace BlackBox;

// extern "C" {

// void app_main() {
//     // BlackBox::Power power;
//     // power.init();
//     // power.turnOn5V();

//     // BlackBox::Lock lock;

//     // lock.init();

//     // gpio_config_t io_conf;
//     // //disable interrupt
//     // io_conf.intr_type = GPIO_INTR_DISABLE;
//     // //set as output mode
//     // io_conf.mode = GPIO_MODE_OUTPUT;
//     // //bit mask of the pins that you want to set,e.g.GPIO18/19
//     // io_conf.pin_bit_mask = (1ULL<<BlackBox::Pins::Power::Power5V.pinNumber);
//     // //disable pull-down mode
//     // io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
//     // //disable pull-up mode
//     // io_conf.pull_up_en = GPIO_PULLUP_DISABLE ;
//     // //configure GPIO with the given settings
//     // gpio_config(&io_conf);
//     // gpio_set_level(BlackBox::Pins::Power::Power5V.pinNumber, 0);
//     // // gpio_set_level(BlackBox::Pins::Lock::Motor, 0);
//     // vTaskDelay(1000 / portTICK_PERIOD_MS);

//     // while (true) {
//     //     lock.lock();
//     //     vTaskDelay(1000 / portTICK_PERIOD_MS);
//     //     lock.unlock();
//     //     vTaskDelay(1000 / portTICK_PERIOD_MS);
//     // }

//     // io_conf.pin_bit_mask = (1ULL<<BlackBox::Pins::Lock::A)|(1ULL<<BlackBox::Pins::Lock::B);
//     // io_conf.mode = GPIO_MODE_INPUT;

//     // gpio_config(&io_conf);

//     // bool state = false;

//     // BlackBox::Lock lock;
//     // lock.init();
//     // ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0b110);
//     // ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
//     // while (true) {
//     //     if(gpio_get_level(BlackBox::Pins::Lock::B) == 0){
//     //         ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0b0);
//     //         ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);

//     //     //     vTaskDelay(500 / portTICK_PERIOD_MS);

//     //     //     ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0b110);
//     //     //     ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
//     //     //     state = !state;
//     //     }
//     //     printf("%i %i\n", gpio_get_level(BlackBox::Pins::Lock::A), gpio_get_level(BlackBox::Pins::Lock::B));
//     //     vTaskDelay(100 / portTICK_PERIOD_MS);
//     //     /* code */
//     // }

//     LDC ldc;
//     ldc.init();

//     Power power;
//     power.init();
//     power.turnOn5V();

//     Ring ring;

//     ring.clear();
//     ring.setDarkModeValue(50);
//     ring.enableDarkMode();

//     float a[4] = { 1, 1, 1, 1 };

//     Touchpad touchpad(16, 0b1110, a);

//     auto regs = ldc.registers();

//     for (int i = 0; i < 4; i++) {
//         regs.settleCount[i] = 10;
//         regs.referenceCount[i] = 1238;
//         regs.driveCurrent[i].reg = 0x9000;
//         // regs.offset[i] = 3500;
//     }

//     // regs.resetDevice.outputGain = 3;

//     ldc.writeRegisters(regs);

//     while (true) {
//         BlackBox::Coords data;
//         ldc.syncChannels();
//         BlackBox::Coords out = touchpad.calculate(ldc);
//         unsigned index = Index(out);
//         ring.clear();
//         ring.show();
//         vTaskDelay(10 / portTICK_PERIOD_MS);
//         ring[out] = Rgb(255, 0, 0);
//         ring.show();

//         // printf("X:%i\tY:%i\tP:%u\tI:%u\n", out.x, out.y, out.pressure, index);
//         //525, 830, 907, 860
//         // data = touchpad.calculate(ldc[0]>>16, ldc[1]>>16, ldc[2]>>16, ldc[3]>>16);
//         // printf("Loop0:%u,\tLoop1:%u,\tLoop2:%u,\tLoop3:%u,\tX:%i,\tY:%i,\tP:%i\n",
//         //     ldc[0] >> 18,
//         //     ((ldc[1] >> 18) & (1 << 8))
//         //         ? (ldc[1] >> 18)
//         //         : (ldc[1] >> 17),
//         //     ((ldc[2] >> 18) & (1 << 8))
//         //         ? (ldc[2] >> 18)
//         //         : (ldc[2] >> 17),
//         //     ((ldc[3] >> 18) & (1 << 8))
//         //         ? (ldc[3] >> 18)
//         //         : (ldc[3] >> 17),
//         //         out.x,
//         //         out.y,
//         //         out.pressure);
//         // printf("Loop0:%u,\tLoop1:%u,\tLoop2:%u,\tLoop3:%u,\tX:%i,\tY:%i,\tP:%u\n", ldc[0]>>16, ldc[1]>>16, ldc[2]>>16, ldc[3]>>16, data.x, data.y, data.pressure);
//         // printf("\n");

//         vTaskDelay(10 / portTICK_PERIOD_MS);
//     }

//     // unsigned m_average[4] = { 0, 0, 0, 0 };

//     // float m_calculationCoefficient[4] = { 1.0, 1.0, 1.0, 1.0 };

//     // unsigned m_data[4] = { 0, 0, 0, 0 };
//     //     vTaskDelay(500 / portTICK_PERIOD_MS);

//     // ldc.syncChannels();

//     // for (int i = 0; i < 4; i++) {
//     //     m_average[i] = ldc[i]>>14;
//     // }

//     // // while (true) {
//     // //     ldc.syncChannels();
//     // //     for (int i = 0; i < 4; i++) {
//     // //         if(true)
//     // //         // if((((ldc[i]>>14) - m_average[i])<=30)||(((ldc[i]>>14) - m_average[i])>=-30))
//     // //             m_data[i] = 0;
//     // //         else
//     // //             m_data[i] = ((ldc[i]>>14) - m_average[i]) ;
//     // //     }
//     // //     printf("Loop0:%i,\tLoop1:%i,\tLoop2:%i,\tLoop3:%i\n", m_data[0], m_data[1], m_data[2], m_data[3]);
//     // //     // printf("LoopA0:%i,\tLoopA1:%i,\tLoopA2:%i,\tLoopA3:%i\n", m_average[0], m_average[1], m_average[2], m_average[3]);
//     // //     vTaskDelay(10 / portTICK_PERIOD_MS);
//     // // }

//     // unsigned b[4];
//     // while (true) {
//     //     for (int i = 0; i < 1; i++) {
//     //         ldc.syncChannels();
//     //         for (int j = 0; j < 4; j++) {
//     //             b[j] += ((ldc[j]>>18));
//     //         }
//     //     }
//     //     for (int j = 0; j < 4; j++) {
//     //             printf("Data%i:%u\t", j, b[j]/1);
//     //             b[j] = 0;
//     //         }
//     //     printf("\n");
//     //     vTaskDelay(10 / portTICK_PERIOD_MS);
//     // }

//     // BlackBox::RTC rtc;

//     // rtc.init();

//     // while (true) {
//     //     printf("Second: %i\n", rtc.now().tm_sec);
//     //     vTaskDelay(1000 / portTICK_PERIOD_MS);
//     // }
// }
// }
// #endif

#include "BlackBox_Manager.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

using namespace BlackBox;

extern "C" {

void app_main() {
    Manager& man = Manager::singleton();

    man.init();

    man.power().turnOn5V();

    Ring& ring = man.ring();

    ring.clear();
    ring.setDarkModeValue(50);
    ring.enableDarkMode();

    float a[4] = { 1, 1, 1, 1 };

    Touchpad touchpad(16, 0b1110, a);

    auto regs = man.ldc().registers();

    for (int i = 0; i < 4; i++) {
        regs.settleCount[i] = 10;
        regs.referenceCount[i] = 1238;
        regs.driveCurrent[i].reg = 0x9000;
    }

    man.ldc().writeRegisters(regs);

    while (true) {
        BlackBox::Coords data;
        BlackBox::Coords out = man.touchpad().calculate();;
        ring.clear();
        vTaskDelay(10 / portTICK_PERIOD_MS);
        ring[out] = Rgb(255, 0, 0);
        ring.show();

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
}