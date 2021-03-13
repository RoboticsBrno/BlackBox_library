#pragma once

#include "BlackBox_LDC.hpp"
#include <cstdint>

// FIXME: Implement adaptive filter measuring average difference and update threshold based on that

namespace BlackBox
{
struct Coords {
    int x;
    int y;
    unsigned pressure;

    Coords(int x = 0, int y = 0, unsigned pressure = 0);
    Coords& operator=(Coords other);
    Coords operator+(Coords other) const;
    Coords& operator+=(Coords other);
};

class Touchpad
{
private:
    const int m_dataBitsToRemove;
    const std::uint8_t m_protectOverflow;
    const float m_calculationCoefficient[4];
    
    int removeOverflow(int value);
    int prepareData(int value, int channel);

public:
    Touchpad(int dataBitsToRemove, std::uint8_t protectOverflow, float m_calculationCoefficient[4]);
    ~Touchpad() = default;

    Coords calculate(int channel0, int channel1, int channel2, int channel3);
    Coords calculate(int channels[4]);
    Coords calculate(const LDC& ldc);
    Coords calculate(LDC& ldc, bool update);
};
} // namespace BlackBox
