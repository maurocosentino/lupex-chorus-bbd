#include "BBDFilter.h"
#include <cmath>

namespace Lupex
{

    BBDFilter::BBDFilter() = default;

    void BBDFilter::prepare (double sr)
    {
        sampleRate = sr;
        reset();
    }

    void BBDFilter::reset()
    {
        lastOutput = 0.0f;
    }

    void BBDFilter::setTone (float tone)
    {
        coefficient = calculateCoefficient (tone);
    }

    float BBDFilter::process (float input)
    {
        // Filtro low-pass de 2 polo — carácter BBD
        lastOutput  = lastOutput  + coefficient * (input       - lastOutput);
        lastOutput2 = lastOutput2 + coefficient * (lastOutput  - lastOutput2);
        return lastOutput2;
    }

    float BBDFilter::calculateCoefficient (float tone) const
    {
        float cutoffHz = 1000.0f + tone * 5000.0f;
        // Fórmula estándar para coeficiente de filtro RC digital
        float rc = 1.0f / (2.0f * 3.14159265f * cutoffHz);
        float dt = 1.0f / static_cast<float> (sampleRate);

        return dt / (rc + dt);
    }

} // namespace Lupex