#pragma once

#include <cstdint>

// FIXME: Implement adaptive filter measuring average difference and update threshold based on that

namespace BlackBox
{
struct Cords {
    int x = 0;
    int y = 0;
    unsigned pressure = 0;
};

class Touchpad
{
private:
    const int m_dataBitsToRemove;
    const std::uint8_t m_protectOverflow;
    const float m_calculationCoefficient[4];
    
public:
    Touchpad(int dataBitsToPreserve, std::uint8_t protectOverflow, float m_calculationCoefficient[4]);
    ~Touchpad() = default;

    Cords calculate(int channel0, int channel1, int channel2, int channel3);
};
} // namespace BlackBox
