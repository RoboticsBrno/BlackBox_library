#include "library/BlackBox_Manager.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

using namespace BlackBox;

extern "C" {

void app_main() {
    Manager& man = Manager::singleton(); // Get instance of Manager class

    man.init(); // Initialize BlackBox's peripherials

    man.power().turnOn5V(); // Turn on 5V power, meaning LEDs and lock

    Ring& ring = man.ring(); // Get instance of Ring class

    ring.clear(); // Clear display(Ring)
    ring.setDarkModeValue(50); // Set maximum value for darkmode
    ring.enableDarkMode(); // Enable dark mode

    
    while (true) {
        // Draw arcs with given parameters. See documentation for more information 
        ring.drawArc(Rgb(255,0,0), 0, 37, ArcType::Clockwise); // Draw red arc from Index 0 to(including) 37 going Clockwise
        ring.drawArc(Rgb(0, 255,0), 0, 26);
        ring.drawArc(Rgb(0,0, 255), 0, 15);

        ring.show(); // Show changes on display

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
}