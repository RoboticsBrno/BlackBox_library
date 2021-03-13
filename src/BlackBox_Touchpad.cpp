#include "BlackBox_Touchpad.hpp"

#include <cstdio>
#include <cstdlib>

namespace BlackBox {
Touchpad::Touchpad(int dataBitsToPreserve, std::uint8_t protectOverflow, float calculationCoefficient[4])
    : m_dataBitsToRemove(dataBitsToPreserve)
    , m_protectOverflow(protectOverflow)
    , m_calculationCoefficient { calculationCoefficient[0],
        calculationCoefficient[1],
        calculationCoefficient[2],
        calculationCoefficient[3] } {
}

Cords Touchpad::calculate(int channel0, int channel1, int channel2, int channel3) {
    channel0 = m_calculationCoefficient[0]
            * (((m_protectOverflow) & (1 << 0))
        ? (((channel0 >> m_dataBitsToRemove) & (1 << 8))
            ? (channel0 >> m_dataBitsToRemove)
            : (channel0 >> (m_dataBitsToRemove - 1)))
        : (channel0 >> m_dataBitsToRemove));

    channel1 = m_calculationCoefficient[1]
            * (((m_protectOverflow) & (1 << 1))
        ? (((channel1 >> m_dataBitsToRemove) & (1 << 8))
            ? (channel1 >> m_dataBitsToRemove)
            : (channel1 >> (m_dataBitsToRemove - 1)))
        : channel1);

    channel2 = m_calculationCoefficient[2]
            * (((m_protectOverflow) & (1 << 2))
        ? (((channel2 >> m_dataBitsToRemove) & (1 << 8))
            ? (channel2 >> m_dataBitsToRemove)
            : (channel2 >> (m_dataBitsToRemove - 1)))
        : channel2);

    channel3 = m_calculationCoefficient[3]
            * (((m_protectOverflow) & (1 << 3))
        ? (((channel3 >> m_dataBitsToRemove) & (1 << 8))
            ? (channel3 >> m_dataBitsToRemove)
            : (channel3 >> (m_dataBitsToRemove - 1)))
        : channel3);

    // printf("CH0:%u\tCH1:%u\tCH2:%u\tCH3:%u\t", channel0, channel1, channel2, channel3);

    Cords out;
    out.x = -channel0
        - channel1
        + channel2
        + channel3;

    out.y = channel0
        - channel1
        - channel2
        + channel3;

    out.pressure = (channel0 + channel1 + channel2 + channel3) / 4;
    return out;
}

} // namespace BlackBox
