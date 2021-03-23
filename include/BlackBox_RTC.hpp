/**
 * @file BlackBox_RTC.hpp
 * @author Tomáš Rohlínek (haberturdeur)
 * @brief Library for interfacing with RTC and updating time
 */

#pragma once

#include "BlackBox_I2C.hpp"
#include "M41T62_regs.hpp"
#include "driver/i2c.h"
#include <ctime>
#include <mutex>

namespace BlackBox {
/**
 * @brief Class for interfacing with RTC
 */
class RTC : public I2C::Device {
private:
    M41T62Regs::M41T62_dev_t m_regs;

    mutable std::recursive_mutex m_mutex;

    const char* m_tag = "RTC";

    void clearCache(); // Reset cache to default state

    void readRegister(M41T62Regs::registerAddresses address); // Read register from RTC

    void writeRegister(M41T62Regs::registerAddresses address); // Write register to RTC
    void writeRegister(M41T62Regs::registerAddresses address, std::uint8_t value); // Write register to RTC
public:
    RTC(std::uint16_t address = 0x68, i2c_port_t = 0);
    ~RTC() = default;

    virtual void init() final;

    void adjustRTC(std::tm newTime);

    void adjustESP();

    void reset();

    std::tm now();

    /**
     * @brief Retrieve read-only copy of cached registers
     * 
     * @return copy of cached registers
     */
    M41T62Regs::M41T62_dev_t registers() const;

    /**
     * @brief Retrieve const reference to cached registers
     * 
     * @return copy of cached registers
     */
    const M41T62Regs::M41T62_dev_t& regs() const;

    /**
     * @brief Write registers
     */
    void writeRegisters(const M41T62Regs::M41T62_dev_t&);
};
} // namespace BlackBox
