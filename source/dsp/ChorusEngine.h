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
        Lupex::DelayLine    delayL;
        Lupex::BBDFilter    filterInL;
        Lupex::BBDFilter    filterOutL;
        Lupex::TapeEmulator tapeL;
        WowFlutter          lfoL;

        Lupex::DelayLine    delayR;
        Lupex::BBDFilter    filterInR;
        Lupex::BBDFilter    filterOutR;
        Lupex::TapeEmulator tapeR;
        WowFlutter          lfoR;

        static constexpr float centerDelayMs { 15.0f };
        static constexpr float maxDepthMs    {  5.0f };

        float applyMix (float dry, float wet, float mix) const;
    };

} // namespace Chorus