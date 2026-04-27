#pragma once

#include "DelayLine.h"
#include "BBDFilter.h"
#include "TapeEmulator.h"
#include "WowFlutter.h"

namespace Chorus
{

    class ChorusEngine
    {
    public:
        ChorusEngine();

        void prepare (double sampleRate, int samplesPerBlock);
        void reset();

        void process (float* channelL, float* channelR,
                      int    numSamples,
                      float  rate,
                      float  depth,
                      float  mix);

    private:
        // Canal L
        DelayLine    delayL;
        BBDFilter    filterInL;    // anti-aliasing
        BBDFilter    filterOutL;   // reconstrucción
        TapeEmulator tapeL;
        WowFlutter   lfoL;

        // Canal R
        DelayLine    delayR;
        BBDFilter    filterInR;
        BBDFilter    filterOutR;
        TapeEmulator tapeR;
        WowFlutter   lfoR;

        static constexpr float centerDelayMs { 15.0f };  // centro del delay BBD
        static constexpr float maxDepthMs    {  5.0f };  // Depth 100% = ±5ms

        float applyMix (float dry, float wet, float mix) const;
    };

} // namespace Chorus