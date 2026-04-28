#include "ShelfFilter.h"
#include <algorithm>

namespace Chorus
{

ShelfFilter::ShelfFilter (Type t) : type (t)
{
    // Frecuencia inicial — mínimo de cada rango MXR
    cutoffHz = (type == Type::LowShelf) ? 70.0f : 660.0f;
}
void ShelfFilter::setCutoff (float hz)
{
    cutoffHz = hz;
    calculateCoefficients();
}

void ShelfFilter::prepare (double sr)
{
    sampleRate = sr;
    calculateCoefficients();
    reset();
}

void ShelfFilter::reset()
{
    x1 = x2 = y1 = y2 = 0.0;
}

void ShelfFilter::setGain (float g)
{
    gain = std::clamp (g, 0.0f, 1.0f);

    // El knob mueve la frecuencia de corte según rango
    // LOW:  0.0 → 70Hz,  1.0 → 800Hz
    // HIGH: 0.0 → 660Hz, 1.0 → 20000Hz
    if (type == Type::LowShelf)
        cutoffHz = 70.0f + gain * (800.0f - 70.0f);
    else
        cutoffHz = 660.0f + gain * (20000.0f - 660.0f);

    calculateCoefficients();
}

float ShelfFilter::process (float input)
{
    double x0 = static_cast<double> (input);

    double y0 = b0 * x0 + b1 * x1 + b2 * x2
                         - a1 * y1 - a2 * y2;

    x2 = x1; x1 = x0;
    y2 = y1; y1 = y0;

    return static_cast<float> (y0);
}

void ShelfFilter::calculateCoefficients()
{
    const double pi = 3.14159265358979;
    const double w0 = 2.0 * pi * cutoffHz / sampleRate;
    const double cosW = std::cos (w0);
    const double sinW = std::sin (w0);

    // Corte fijo — el knob mueve la frecuencia
    float maxCut = (type == Type::LowShelf) ? -12.0f : -6.0f;
    float dBGain = maxCut;  // siempre al máximo corte
    double A = std::pow (10.0, dBGain / 40.0);

    double S     = 1.0;
    double alpha = sinW / 2.0 * std::sqrt ((A + 1.0 / A) * (1.0 / S - 1.0) + 2.0);

    if (type == Type::LowShelf)
    {
        double a0 =       (A + 1.0) + (A - 1.0) * cosW + 2.0 * std::sqrt (A) * alpha;
        b0 =  A * ((A + 1.0) - (A - 1.0) * cosW + 2.0 * std::sqrt (A) * alpha) / a0;
        b1 =  2.0 * A * ((A - 1.0) - (A + 1.0) * cosW) / a0;
        b2 =  A * ((A + 1.0) - (A - 1.0) * cosW - 2.0 * std::sqrt (A) * alpha) / a0;
        a1 = -2.0 * ((A - 1.0) + (A + 1.0) * cosW) / a0;
        a2 =        ((A + 1.0) + (A - 1.0) * cosW - 2.0 * std::sqrt (A) * alpha) / a0;
    }
    else
    {
        double a0 =       (A + 1.0) - (A - 1.0) * cosW + 2.0 * std::sqrt (A) * alpha;
        b0 =  A * ((A + 1.0) + (A - 1.0) * cosW + 2.0 * std::sqrt (A) * alpha) / a0;
        b1 = -2.0 * A * ((A - 1.0) + (A + 1.0) * cosW) / a0;
        b2 =  A * ((A + 1.0) + (A - 1.0) * cosW - 2.0 * std::sqrt (A) * alpha) / a0;
        a1 =  2.0 * ((A - 1.0) - (A + 1.0) * cosW) / a0;
        a2 =        ((A + 1.0) - (A - 1.0) * cosW - 2.0 * std::sqrt (A) * alpha) / a0;
    }
}
} // namespace Chorus