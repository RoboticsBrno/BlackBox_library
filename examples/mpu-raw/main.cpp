#include "library/BlackBox_MPU.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <iostream>

#ifdef BB_MPU
using namespace BlackBox;
#endif
using namespace std;

extern "C" {

void app_main() {
    #ifdef BB_MPU
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
        mpu.syncMeasurements();

        cout << mpu.raw().accelerometer.y << "\t"
             << mpu.raw().accelerometer.z << "\t"
             << mpu.raw().accelerometer.x << "\t"

             << mpu.raw().gyroscope.x << "\t"
             << mpu.raw().gyroscope.y << "\t"
             << mpu.raw().gyroscope.z << endl;

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    #endif
}
}