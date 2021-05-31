#pragma once

#include <array>
#include <cstdint>

namespace MPU6050Regs {

enum registerAddresses {
    SelfTestX,
    SelfTestY,
    SelfTestZ,
    SelfTestA,
    SampleRateDivider,
    Config,
    GyroscopeConfig,
    AccelerometerConfig,
    FIFOEnable,
    I2CMasterControl,
    I2CSlave0Address,
    I2CSlave0Register,
    I2CSlave0Control,
    I2CSlave1Address,
    I2CSlave1Register,
    I2CSlave1Control,
    I2CSlave2Address,
    I2CSlave2Register,
    I2CSlave2Control,
    I2CSlave3Address,
    I2CSlave3Register,
    I2CSlave3Control,
    I2CSlave4Address,
    I2CSlave4Register,
    I2CSlave4DataOut,
    I2CSlave4Control,
    I2CSlave4DataIn,
    I2CMasterStatus,
    InterruptPinConfig,
    InterruptEnable,
    InterruptStatus,
    AccelerometerXOutH,
    AccelerometerXOutL,
    AccelerometerYOutH,
    AccelerometerYOutL,
    AccelerometerZOutH,
    AccelerometerZOutL,
    TemperatureOutH,
    TemperatureOutL,
    GyroscopeXOutH,
    GyroscopeXOutL,
    GyroscopeYOutH,
    GyroscopeYOutL,
    GyroscopeZOutH,
    GyroscopeZOutL,
    ExternalSensorData0,
    ExternalSensorData1,
    ExternalSensorData2,
    ExternalSensorData3,
    ExternalSensorData4,
    ExternalSensorData5,
    ExternalSensorData6,
    ExternalSensorData7,
    ExternalSensorData8,
    ExternalSensorData9,
    ExternalSensorData10,
    ExternalSensorData11,
    ExternalSensorData12,
    ExternalSensorData13,
    ExternalSensorData14,
    ExternalSensorData15,
    ExternalSensorData16,
    ExternalSensorData17,
    ExternalSensorData18,
    ExternalSensorData19,
    ExternalSensorData20,
    ExternalSensorData21,
    ExternalSensorData22,
    ExternalSensorData23,
    I2CSlave0DataOut,
    I2CSlave1DataOut,
    I2CSlave2DataOut,
    I2CSlave3DataOut,
    I2CMasterDelayControl,
    SignalPathReset,
    UserControl,
    PowerManagement1,
    PowerManagement2,
    FIFOCountH,
    FIFOCountL,
    FIFOReadWrite,
    WhoAmI,
    MaxAddress,
};

static constexpr std::array<std::uint8_t, MaxAddress> hwRegisterAddresses = { {
    0x0D, 0x0E, 0x0F, 0x10,
    0x19, 0x1A, 0x1B, 0x1C,
    0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60,
    0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
    0x6A, 0x6B, 0x6C,
    0x72, 0x73, 0x74, 0x75
    } };

enum ExternalSyncSettings : std::uint8_t {
    InputDisabled = 0,
    TemperatureOutL0,
    GyroscopeXOutL0,
    GyroscopeYOutL0,
    GyroscopeZOutL0,
    AccelerometerXOutL0,
    AccelerometerYOutL0,
    AccelerometerZOutL0,
};

enum AccelerometerFullScaleSelect : std::uint8_t {
    _2G = 0,
    _4G,
    _8G,
    _16G,
};

enum GyroscopeFullScaleSelect : std::uint8_t {
    _250DpS = 0,
    _500DpS,
    _1000DpS,
    _2000DpS,
};

enum I2CMasterClockSpeed {
    _348kHz = 0,
    _333kHz,
    _320kHz,
    _308kHz,
    _296kHz,
    _286kHz,
    _276kHz,
    _267kHz,
    _258kHz,
    _500kHz,
    _471kHz,
    _444kHz,
    _421kHz,
    _400kHz,
    _381kHz,
    _364kHz,
};

enum WakeUpFrequency : std::uint16_t {
    _1Hz25 = 0,
    _5Hz,
    _20Hz,
    _40Hz,
};

enum ClockSelection : std::uint16_t {
    Internal = 0,
    PLLGyroX,
    PLLGyroY,
    PLLGyroZ,
    PLL32k768Hz,
    PLL19M2Hz,
    Reserved,
    Stop,
};

union MPU6050_dev_t {
    struct {
        union {
            struct {
                std::uint8_t gyroTest : 5;
                std::uint8_t accelerometerTest : 3;
            };
            std::uint8_t reg;
        } selfTestX;

        union {
            struct {
                std::uint8_t gyroTest : 5;
                std::uint8_t accelerometerTest : 3;
            };
            std::uint8_t reg;
        } selfTestY;

        union {
            struct {
                std::uint8_t gyroTest : 5;
                std::uint8_t accelerometerTest : 3;
            };
            std::uint8_t reg;
        } selfTestZ;

        union {
            struct {
                std::uint8_t zTest : 2;
                std::uint8_t yTest : 2;
                std::uint8_t xTest : 2;
                std::uint8_t reserved : 2;
            };
            std::uint8_t reg;
        } selfTestA;

        std::uint8_t sampleRateDivider;

        union {
            struct {
                std::uint8_t digitalLowPassFilter : 3;
                ExternalSyncSettings externalSyncSet : 3;
                std::uint8_t reserved : 2;
            };
            std::uint8_t reg;
        } config;

        union {
            struct {
                std::uint8_t reserved : 3;
                GyroscopeFullScaleSelect fullScaleSelect : 2;
                std::uint8_t doSelfTestZ : 1;
                std::uint8_t doSelfTestY : 1;
                std::uint8_t doSelfTestX : 1;
            };
            std::uint8_t reg;
        } gyroscopeConfig;

        union {
            struct {
                std::uint8_t reserved : 3;
                AccelerometerFullScaleSelect fullScaleSelect : 2;
                std::uint8_t doSelfTestZ : 1;
                std::uint8_t doSelfTestY : 1;
                std::uint8_t doSelfTestX : 1;
            };
            std::uint8_t reg;
        } accelerometerConfig;

        union {
            struct {
                std::uint8_t slave0 : 1;
                std::uint8_t slave1 : 1;
                std::uint8_t slave2 : 1;
                std::uint8_t accelerometer : 1;
                std::uint8_t gyroscopeZ : 1;
                std::uint8_t gyroscopeY : 1;
                std::uint8_t gyroscopeX : 1;
                std::uint8_t temperature : 1;
            };
            std::uint8_t reg;
        } fifoEnable;

        union {
            struct {
                I2CMasterClockSpeed clock : 4;
                std::uint8_t noRestart : 1;
                std::uint8_t slave3FIFOEnable : 1;
                std::uint8_t waitForExternalSensor : 1;
                std::uint8_t multiMasterEnable : 1;
            };
            std::uint8_t reg;
        } i2cMasterControl;

        union {
            struct {
                struct {
                    std::uint8_t address : 7;
                    std::uint8_t readWrite : 1;
                };
                std::uint8_t registerAddress;
                struct {
                    std::uint8_t length : 4;
                    std::uint8_t groupingOrder : 1;
                    std::uint8_t writeRegisterAddress : 1;
                    std::uint8_t byteSwap : 1;
                    std::uint8_t enable : 1;
                };
            };
            std::uint8_t regs[3];
        } slaveControl[4];

        union {
            struct {
                struct {
                    std::uint8_t address : 7;
                    std::uint8_t readWrite : 1;
                };
                std::uint8_t registerAddress;
                std::uint8_t dataToBeWritten;
                struct {
                    std::uint8_t length : 5;
                    std::uint8_t writeRegisterAddress : 1;
                    std::uint8_t enableInterrupt : 1;
                    std::uint8_t enable : 1;
                };
                std::uint8_t dataRead;
            };
            std::uint8_t regs[5];
        } slaveControl4;

        union {
            struct {
                std::uint8_t slave0NACK : 1;
                std::uint8_t slave1NACK : 1;
                std::uint8_t slave2NACK : 1;
                std::uint8_t slave3NACK : 1;
                std::uint8_t slave4NACK : 1;
                std::uint8_t lostArbitration : 1;
                std::uint8_t slave4Done : 1;
                std::uint8_t passThrough : 1;
            };
            std::uint8_t reg;
        } masterStatus;

        union {
            struct {
                std::uint8_t reserved : 1;
                std::uint8_t i2cBypassEnable : 1;
                std::uint8_t fsyncInterruptEnable : 1;
                std::uint8_t fsyncInterruptLevel : 1;
                std::uint8_t clearOnReadMode : 1;
                std::uint8_t latchInterrupt : 1;
                std::uint8_t openDrainEnable : 1;
                std::uint8_t activeLevel : 1;
            };
            std::uint8_t reg;
        } interruptPinConfig;

        union {
            struct {
                std::uint8_t dataReady : 1;
                std::uint8_t reserved : 2;
                std::uint8_t masterI2CInterrupt : 1;
                std::uint8_t fifoOverflow : 1;
                std::uint8_t reserved1 : 3;
            };
            std::uint8_t reg;
        } interruptEnable;

        union {
            struct {
                std::uint8_t dataReady : 1;
                std::uint8_t reserved : 2;
                std::uint8_t masterI2CInterrupt : 1;
                std::uint8_t fifoOverflow : 1;
                std::uint8_t reserved1 : 3;
            };
            std::uint8_t reg;
        } interruptStatus;

        union {
            struct {
                union {
                    struct {
                        union {
                            uint16_t value;
                            uint8_t regs[2];
                        } x;

                        union {
                            uint16_t value;
                            uint8_t regs[2];
                        } y;

                        union {
                            uint16_t value;
                            uint8_t regs[2];
                        } z;
                    };
                } accelerometer;

                union {
                    uint16_t value;
                    uint8_t regs[2];
                } temperature;

                union {
                    struct {
                        union {
                            uint16_t value;
                            uint8_t regs[2];
                        } x;

                        union {
                            uint16_t value;
                            uint8_t regs[2];
                        } y;

                        union {
                            uint16_t value;
                            uint8_t regs[2];
                        } z;
                    };
                } gyroscope;
            };
            std::uint8_t regs[14];
        } measurements;

        std::uint8_t externalSensorData[24];

        std::uint8_t slaveDataOut[4];

        union {
            struct {
                std::uint8_t slave0DelayEnable : 1;
                std::uint8_t slave1DelayEnable : 1;
                std::uint8_t slave2DelayEnable : 1;
                std::uint8_t slave3DelayEnable : 1;
                std::uint8_t slave4DelayEnable : 1;
                std::uint8_t reserved : 2;
                std::uint8_t externalSensorShadowEnable : 1;
            };
            std::uint8_t reg;
        } i2cMasterDelayControl;

        union {
            struct {
                std::uint8_t temperatureReset : 1;
                std::uint8_t accelerometerReset : 1;
                std::uint8_t gyroscopeReset : 1;
                std::uint8_t reset : 5;
            };
            std::uint8_t reg;
        } signalPathReset;

        union {
            struct {
                std::uint8_t signalCondReset : 1;
                std::uint8_t i2cMasterReset : 1;
                std::uint8_t fifoReset : 1;
                std::uint8_t reserved : 1;
                std::uint8_t i2cInterfaceDisable : 1; /*!< On MPU-6050 always write 0 */
                std::uint8_t i2cMasterEnable : 1;
                std::uint8_t fifoEnable : 1;
                std::uint8_t reserved1 : 1;
            };
            std::uint8_t reg;
        } userControl;

        union {
            std::uint8_t regs[2];
            struct {
                std::uint16_t standbyZGyroscope : 1;
                std::uint16_t standbyYGyroscope : 1;
                std::uint16_t standbyXGyroscope : 1;
                std::uint16_t standbyZAccelerometer : 1;
                std::uint16_t standbyYAccelerometer : 1;
                std::uint16_t standbyXAccelerometer : 1;
                WakeUpFrequency LowPowerWakeControl : 1;
                ClockSelection clockSelection : 3;
                std::uint16_t temperatureDisable : 1;
                std::uint16_t reserved : 1;
                std::uint16_t cycle : 1;
                std::uint16_t sleep : 1;
                std::uint16_t deviceReset : 1;
            };
        } powerManagement;

        std::uint8_t fifoReadWrite;

        union {
            std::uint8_t reg;
            struct {
                std::uint8_t reserved : 1;
                std::uint8_t data : 6;
                std::uint8_t reserved1 : 1;
            };
        } whoAmI;
    };
    std::uint8_t regs[MaxAddress];

    constexpr MPU6050_dev_t() : powerManagement{0x40, 0}, whoAmI{0x68} {}
};
} // namespace MPU6050Regs
