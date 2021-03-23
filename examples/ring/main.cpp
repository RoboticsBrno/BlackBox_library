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
    ring.setDarkModeValue(100);
    ring.enableDarkMode();

    
    while (true) {
        ring.drawArc(Rgb(255,0,0), 0, 37, ArcType::Clockwise);
        ring.drawArc(Rgb(0, 255,0), 0, 26);
        ring.drawArc(Rgb(0,0, 255), 0, 15);

        ring.show();

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
}