#include "BlackBox_Touchpad.hpp"

#include <cstdio>
#include <cstdlib>

namespace BlackBox {
Coords::Coords(int x, int y, unsigned pressure)
    : x(x)
    , y(y)
    , pressure(pressure) {
}

Coords& Coords::operator=(Coords other) {
    x = other.x;
    y = other.y;
    pressure = other.pressure;
    return *this;
}

Coords& Coords::operator+=(Coords other) {
    x += other.x;
    y += other.y;
    pressure += other.pressure;
    return *this;
}

Coords Coords::operator+(Coords other) const {
    return Coords(x + other.x, y + other.y, pressure + other.pressure);
}

int Touchpad::removeOverflow(int value) {
    if ((value) & (1 << 26))
        return value;
    else
        return value << 1;
}

int Touchpad::prepareData(int value, int channel) {
    if (m_protectOverflow & (1 << channel))
        return removeOverflow(value) >> m_dataBitsToRemove;
    else
        return value >> m_dataBitsToRemove;
}

Touchpad::Touchpad(int dataBitsToPreserve, std::uint8_t protectOverflow, float calculationCoefficient[4])
    : m_dataBitsToRemove(dataBitsToPreserve)
    , m_protectOverflow(protectOverflow)
    , m_calculationCoefficient { calculationCoefficient[0],
        calculationCoefficient[1],
        calculationCoefficient[2],
        calculationCoefficient[3] } {
}

Coords Touchpad::calculate(int channel0, int channel1, int channel2, int channel3) {
    int array[4] = { channel0, channel1, channel2, channel3 };
    return calculate(array);
}

Coords Touchpad::calculate(int channels[4]) {
    Coords out;
    int data[4];
    for (int i = 0; i < 4; i++) {
        data[i] = m_calculationCoefficient[i] * prepareData(channels[i], i);
        out.pressure += data[i];
    }

    out.x = -data[0]
        - data[1]
        + data[2]
        + data[3]
        - 760; // FIXME: Remove this piece ductape

    out.y = data[0]
        - data[1]
        - data[2]
        + data[3];

    out.pressure /= 4;
    return out;
}

Coords Touchpad::calculate(const LDC& ldc) {
    int array[4];
    for (int i = 0; i < 4; i++)
        array[i] = ldc[i];
    return calculate(array);
}

Coords Touchpad::calculate(LDC& ldc, bool update) {
    if (update)
        ldc.syncChannels();
    return calculate(ldc);
}

} // namespace BlackBox
