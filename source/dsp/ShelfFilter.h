#pragma once
#include <cmath>

namespace Chorus
{

    class ShelfFilter
    {
    public:
        enum class Type { LowShelf, HighShelf };

        ShelfFilter (Type type);

        void prepare (double sampleRate);
        void reset();

        // gain: 0.0 = corte máximo, 1.0 = flat
        void setGain (float gain);

        float process (float input);

    private:
        void calculateCoefficients();

        Type   type;
        double sampleRate { 44100.0 };
        float  gain       { 1.0f };

        // Frecuencias de corte fijas según especificación MXR
        // Low shelf:  70Hz (min) — 800Hz (max)
        // High shelf: 660Hz (min) — flat (max)
        float cutoffHz { 0.0f };

        // Coeficientes biquad
        double b0 { 1.0 }, b1 { 0.0 }, b2 { 0.0 };
        double a1 { 0.0 }, a2 { 0.0 };

        // Estado interno
        double x1 { 0.0 }, x2 { 0.0 };
        double y1 { 0.0 }, y2 { 0.0 };
    };

} // namespace Chorus