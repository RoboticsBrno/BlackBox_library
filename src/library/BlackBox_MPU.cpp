#include "Dsp.hpp"
#ifdef BB_MPU

#include "library/BlackBox_MPU.hpp"

#include "esp_log.h"
#include "library/BlackBox_I2C.hpp"
#include "library/MPU6050_regs.hpp"
#include <cstdint>
#include <mutex>

namespace BlackBox {
void MPU::clearCache() {
    std::scoped_lock l(m_mutex);
    for (int i = 0; i < MPU6050Regs::MaxAddress; i++)
        m_regs = MPU6050Regs::MPU6050_dev_t();
}

void MPU::readRegister(MPU6050Regs::registerAddresses i_address) {
    std::scoped_lock l(m_mutex);
    m_regs.regs[i_address] = readByte(i_address);
}

void MPU::writeRegister(MPU6050Regs::registerAddresses i_address) {
    writeRegister(i_address, m_regs.regs[i_address]);
}

void MPU::writeRegister(MPU6050Regs::registerAddresses i_address, std::uint8_t i_value) {
    std::scoped_lock l(m_mutex);
    writeByte(i_address, i_value);
}

MPU::MPU(std::uint16_t i_address, i2c_port_t i_port)
    : Device(i_address, i_port) {
}

void MPU::init() {
    m_port.init();
    reset();
    configure();
}

void MPU::configure() {
    std::scoped_lock l(m_mutex);
    sleep();
    wake();
}

void MPU::reset() {
    std::scoped_lock l(m_mutex);
    m_regs.powerManagement.deviceReset = true;
    writeRegister(MPU6050Regs::PowerManagement1);
    m_regs.powerManagement.deviceReset = false;
    clearCache();
    syncCache();
}

void MPU::wake() {
    std::scoped_lock l(m_mutex);
    m_regs.powerManagement.sleep = false;
    writeRegister(MPU6050Regs::PowerManagement1);
}

void MPU::sleep() {
    std::scoped_lock l(m_mutex);
    m_regs.powerManagement.sleep = true;
    writeRegister(MPU6050Regs::PowerManagement1);
}

void MPU::syncCache() {
    std::scoped_lock l(m_mutex);
    readBytes(0, m_regs.regs, MPU6050Regs::MaxAddress);
}

void MPU::syncMeasurements() {
    std::scoped_lock l(m_mutex);
    readBytes(MPU6050Regs::AccelerometerXOutH, m_regs.measurements.regs, 14);
}

void MPU::syncAccelerometer() {
    std::scoped_lock l(m_mutex);
    readBytes(MPU6050Regs::AccelerometerXOutH, m_regs.measurements.accelerometer.regs, 6);
}

void MPU::syncGyroscope() {
    std::scoped_lock l(m_mutex);
    readBytes(MPU6050Regs::GyroscopeXOutH, m_regs.measurements.gyroscope.regs, 6);
}

const MPU6050Regs::MPU6050_dev_t& MPU::regs() const {
    return m_regs;
}

MPU6050Regs::MPU6050_dev_t MPU::registers() {
    return m_regs;
}

void MPU::writeRegisters(const MPU6050Regs::MPU6050_dev_t& i_registers) {
    for (const auto& i : MPU6050Regs::writableRegisters)
        writeRegister(i, i_registers.regs[i]);
}

const MPU::RawMeasurements& MPU::raw() const {
    return m_regs.measurements;
}

MPU::Scaled MPU::scale(bool i_update) {
    if (i_update)
        syncMeasurements();
    // _2G = 0,
    // _4G,
    // _8G,
    // _16G,
    Scaled::Number factorAccelerometer;
    switch (m_regs.accelerometerConfig.fullScaleSelect) {
    case MPU6050Regs::_2G:
        factorAccelerometer = { 1 / 16384.0 };
        break;
    case MPU6050Regs::_4G:
        factorAccelerometer = { 1 / 8192.0 };
        break;
    case MPU6050Regs::_8G:
        factorAccelerometer = { 1 / 4096.0 };
        break;
    case MPU6050Regs::_16G:
        factorAccelerometer = { 1 / 2048.0 };
        break;
    }

    // _250DpS = 0,
    // _500DpS,
    // _1000DpS,
    // _2000DpS,
    Scaled::Number factorGyroscope;
    switch (m_regs.gyroscopeConfig.fullScaleSelect) {
    case MPU6050Regs::_250DpS:
        factorGyroscope = { 1 / 131.0 };
        break;
    case MPU6050Regs::_500DpS:
        factorGyroscope = { 1 / 65.5 };
        break;
    case MPU6050Regs::_1000DpS:
        factorGyroscope = { 1 / 32.8 };
        break;
    case MPU6050Regs::_2000DpS:
        factorGyroscope = { 1 / 16.4 };
        break;
    }

    Scaled out;

    out.accelerometer.x = factorAccelerometer * raw().accelerometer.x.value();
    out.accelerometer.y = factorAccelerometer * raw().accelerometer.y.value();
    out.accelerometer.z = factorAccelerometer * raw().accelerometer.z.value();

    out.gyroscope.x = factorGyroscope * raw().gyroscope.x.value();
    out.gyroscope.y = factorGyroscope * raw().gyroscope.y.value();
    out.gyroscope.z = factorGyroscope * raw().gyroscope.z.value();

    return out;
}
} // namespace BlackBox
#endif
