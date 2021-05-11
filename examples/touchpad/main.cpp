#include "library/BlackBox_Manager.hpp" // Main BlackBox library header file
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

using namespace BlackBox; // For convenience use whole namespace

extern "C" {

void app_main() {
    Manager& man = Manager::singleton(); // Get instance of Manager class

    man.init(); // Initialize BlackBox's peripherials

    man.power().turnOn5V(); // Turn on 5V power, meaning LEDs and lock

    Ring& ring = man.ring(); // Get instance of Ring class

    ring.clear(); // Clear display(Ring)
    ring.setDarkModeValue(50); // Set maximum value for darkmode
    ring.enableDarkMode(); // Enable dark mode

    Touchpad& touchpad = man.touchpad(); // Get instance of Touchpad class

    // Optional setup for better calibration of LDC

    /* 
    auto regs = man.ldc().registers();
    for (int i = 0; i < 4; i++) {
        regs.settleCount[i] = 10;
        regs.referenceCount[i] = 1238;
        regs.driveCurrent[i].reg = 0x9000;
    }
    man.ldc().writeRegisters(regs);
    */

    while (true) {
        Coords out = man.touchpad().calculate(); // Calculate coordinates of touch
        ring.clear(); // Clear display
        vTaskDelay(10 / portTICK_PERIOD_MS); // Wait before redrawing (isn't strictly necessary)
        ring[out] = Rgb(255, 0, 0); // Implicitly convert cartesian coordinates(Coords) to polar coordinates(ring[Index]) and turn on coresponding LED 
        ring.show(); // Show changes on display

        vTaskDelay(10 / portTICK_PERIOD_MS); // Wait before starting new cycle
    }
}
}