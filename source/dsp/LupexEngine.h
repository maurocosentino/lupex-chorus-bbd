#pragma once

#include "DelayLine.h"
#include "BBDFilter.h"
#include "TapeEmulator.h"
#include "WowFlutter.h"

namespace Lupex
{

    class LupexEngine
    {
    public:
        LupexEngine();

        void prepare (double sampleRate, int samplesPerBlock);
        void setPingPong (bool enabled);
        void reset();

        void process (float* channelL, float* channelR,
                      int    numSamples,
                      float  delayMs,
                      float  feedback,
                      float  mix,
                      float  tone);

    private:
        DelayLine    delayL;
        DelayLine    delayR;
        BBDFilter    filterL;
        BBDFilter    filterR;
        BBDFilter feedbackFilterL;  // filtro dedicado al feedback loop
        BBDFilter feedbackFilterR;
        TapeEmulator tapeL;
        TapeEmulator tapeR;
        WowFlutter wowFlutterL;
        WowFlutter wowFlutterR;

        float smoothedDelayMs { 300.0f };
        // float smoothingCoeff  { 0.9995f };
        static constexpr float stereoSpread { 0.01f };
        bool pingPong { false };

        float applyMix (float dry, float wet, float mix) const;
        float pingPongMix { 0.0f };
        static constexpr float pingPongSlew { 0.005f };  // antes: 0.001f
    };

} // namespace Lupex