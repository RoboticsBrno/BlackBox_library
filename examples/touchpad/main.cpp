#include "library/BlackBox_Manager.hpp"
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