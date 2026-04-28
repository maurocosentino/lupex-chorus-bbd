#include "WowFlutter.h"
#include <algorithm>

namespace Chorus
{

    void WowFlutter::prepare (double sr)
    {
        sampleRate = sr;
        phase      = 0.0f;
    }

    void WowFlutter::reset()
    {
        phase = 0.0f;
    }

    float WowFlutter::process()
    {
        smoothedRate += (rate - smoothedRate) * rateSmoothing;
        phase += static_cast<float> (smoothedRate / sampleRate);
        if (phase > 1.0f) phase -= 1.0f;

        float modulatedPhase = phase + phaseOffset;
        if (modulatedPhase > 1.0f) modulatedPhase -= 1.0f;

        return std::sin (modulatedPhase * twoPi) * depth;
    }

    void WowFlutter::setRate (float hz)
    {
        rate = std::max (0.1f, hz);
    }

    void WowFlutter::setDepth (float ms)
    {
        depth = std::max (0.0f, ms);
    }

    void WowFlutter::setPhaseOffset (float p)
    {
        // Clampear entre 0.0 y 1.0
        phaseOffset = p - std::floor (p);
    }

} // namespace Chorus