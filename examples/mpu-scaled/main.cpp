#include "library/BlackBox_MPU.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <iostream>

using namespace BlackBox;
using namespace std;

extern "C" {

void app_main() {
    MPU mpu;
    mpu.init();

    mpu.syncCache();

    for (int i = 0; i < MPU6050Regs::MaxAddress; i++)
        cout << hex << i << " : " << static_cast<int>(mpu.regs().regs[i]) << endl;
        
    cout << "-----------------------------------------------------"
         << endl
         << "acc_x\t"
         << "acc_y\t"
         << "acc_z\t"
         << "gyro_x\t"
         << "gyro_y\t"
         << "gyro_z"
         << endl;

    while (true) {
        auto data = mpu.scale();

        cout << data.accelerometer.y << "\t"
             << data.accelerometer.z << "\t"
             << data.accelerometer.x << "\t"

             << data.gyroscope.x << "\t"
             << data.gyroscope.y << "\t"
             << data.gyroscope.z << endl;

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
}