#pragma once

#include "Dsp.hpp"
#ifdef BB_MPU

#include "driver/i2c.h"
#include "library/BlackBox_I2C.hpp"
#include "library/MPU6050_regs.hpp"
#include <cnl/fixed_point.h>
#include <cstdint>
#include <mutex>

namespace BlackBox {
class MPU : public I2C::Device {
private:
    MPU6050Regs::MPU6050_dev_t m_regs;

    using RawMeasurements = decltype(m_regs.measurements);

    mutable std::recursive_mutex m_mutex;

    const char* m_tag = "MPU";

    struct Scaled {
        using Number = cnl::scaled_integer<std::int32_t, cnl::power<-16>>;

        struct Accelerometer {
            Number x, y, z;
        } accelerometer;

        struct Gyroscope {
            Number x, y, z;
        } gyroscope;
    };

    void clearCache();

    void readRegister(MPU6050Regs::registerAddresses address); // Read register from MPU

    void writeRegister(MPU6050Regs::registerAddresses address); // Write register to MPU

public:
    void writeRegister(MPU6050Regs::registerAddresses address, std::uint8_t value); // Write register to MPU
    MPU(std::uint16_t address = 0x69, i2c_port_t = 0);
    ~MPU() = default;

    virtual void init() override;

    void configure();
    void reset();

    void wake();

    void sleep();

    void syncCache();
    void syncMeasurements();
    void syncAccelerometer();
    void syncGyroscope();

    MPU6050Regs::MPU6050_dev_t registers();
    const MPU6050Regs::MPU6050_dev_t& regs() const;

    void writeRegisters(const MPU6050Regs::MPU6050_dev_t&);

    const RawMeasurements& raw() const;

    Scaled scale(bool update = false);
};
} // namespace BlackBox
#endif
