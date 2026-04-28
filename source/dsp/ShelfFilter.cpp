#include "ShelfFilter.h"
#include <algorithm>

namespace Chorus
{

ShelfFilter::ShelfFilter (Type t) : type (t)
{
    // Frecuencia de corte central de cada shelf
    cutoffHz = (type == Type::LowShelf) ? 400.0f : 1000.0f;
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
    // gain 1.0 = flat (sin efecto), gain 0.0 = corte máximo (-12dB low, -6dB high)
    const double pi = 3.14159265358979;
    const double w0 = 2.0 * pi * cutoffHz / sampleRate;
    const double cosW = std::cos (w0);
    const double sinW = std::sin (w0);

    // dBGain: 0.0 a -12dB para low shelf, 0.0 a -6dB para high shelf
    float maxCut = (type == Type::LowShelf) ? -12.0f : -6.0f;
    float dBGain = maxCut * (1.0f - gain);  // gain=1 → 0dB, gain=0 → maxCut
    double A  = std::pow (10.0, dBGain / 40.0);  // amplitud lineal (sqrt para shelf)

    double S  = 1.0;  // slope = 1 (máxima pendiente)
    double alpha = sinW / 2.0 * std::sqrt ((A + 1.0 / A) * (1.0 / S - 1.0) + 2.0);

    if (type == Type::LowShelf)
    {
        double a0 =        (A + 1.0) + (A - 1.0) * cosW + 2.0 * std::sqrt (A) * alpha;
        b0 =  A * ((A + 1.0) - (A - 1.0) * cosW + 2.0 * std::sqrt (A) * alpha) / a0;
        b1 =  2.0 * A * ((A - 1.0) - (A + 1.0) * cosW) / a0;
        b2 =  A * ((A + 1.0) - (A - 1.0) * cosW - 2.0 * std::sqrt (A) * alpha) / a0;
        a1 = -2.0 * ((A - 1.0) + (A + 1.0) * cosW) / a0;
        a2 =        ((A + 1.0) + (A - 1.0) * cosW - 2.0 * std::sqrt (A) * alpha) / a0;
    }
    else // HighShelf
    {
        double a0 =        (A + 1.0) - (A - 1.0) * cosW + 2.0 * std::sqrt (A) * alpha;
        b0 =  A * ((A + 1.0) + (A - 1.0) * cosW + 2.0 * std::sqrt (A) * alpha) / a0;
        b1 = -2.0 * A * ((A - 1.0) + (A + 1.0) * cosW) / a0;
        b2 =  A * ((A + 1.0) + (A - 1.0) * cosW - 2.0 * std::sqrt (A) * alpha) / a0;
        a1 =  2.0 * ((A - 1.0) - (A + 1.0) * cosW) / a0;
        a2 =        ((A + 1.0) - (A - 1.0) * cosW - 2.0 * std::sqrt (A) * alpha) / a0;
    }
}

} // namespace Chorus