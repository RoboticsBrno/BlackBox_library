#pragma once

#include "BlackBox_I2C.hpp"
#include "LDC16XX_regs.h"
#include "driver/i2c.h"
#include <cstdint>
#include <mutex>

namespace BlackBox {
class LDC : public I2C::Device {
private:
    LDCRegs::LDC16XX_dev_t m_regs;

    mutable std::recursive_mutex m_mutex;

    const char* m_tag = "LDC";

    void clearCache(); // Reset cache to default state

    void readRegister(LDCRegs::registerAddresses address); // Read register from LDC 

    void writeRegister(LDCRegs::registerAddresses address); // Write register to LDC
    void writeRegister(LDCRegs::registerAddresses address, std::uint16_t value); // Write register to LDC

    void readChannel(int channel); // Read channel from LDC
public:

    /**
     * @brief Construct a new LDC object
     * 
     * @param address Address of LDC (0x2A or 0x2B)
     * @param port ESP32s i2c port on which LDC is connected
     */
    LDC(std::uint16_t address = 0x2A, i2c_port_t = 0);
    ~LDC() = default;

    /**
     * @brief Initialize LDC
     */
    virtual void init() final;

    /**
     * @brief Configure LDC to its default operation state
     */
    void configure(); // FIXME: this should have parameters for configuration

    /**
     * @brief Read any pending errors from LDC
     */
    void readErrors();

    /**
     * @brief Wake up LDC from sleep mode
     */
    void wake();

    /**
     * @brief Put LDC into sleep mode
     */
    void sleep();

    /**
     * @brief Synchronize (Read Only) all cached registers with their physical counterparts
     */
    void syncCache();

    /**
     * @brief Sync channel values in cache with their physical counterparts
     */
    void syncChannels();

    /**
     * @brief Reset LDC using SW reset
     */
    void reset();

    /**
     * @brief Retrieve read-only copy of cached registers
     * 
     * @return copy of cached registers
     */
    LDCRegs::LDC16XX_dev_t registers() const;

    /**
     * @brief Retrieve const reference to cached registers
     * 
     * @return copy of cached registers
     */
    const LDCRegs::LDC16XX_dev_t& regs() const;

    /**
     * @brief Write registers
     */
    void writeRegisters(const LDCRegs::LDC16XX_dev_t&); // FIXME: probably should use pass by value for thread safety?

    /**
     * @brief Returns value on channel.
     * 
     * @param channel(0-3) - channel of LDC whose value is to be returned
     * @return value 
     */
    std::uint32_t operator[](int channel) const;
};
} // namespace BlackBox
