#pragma once
#include "DelayLine.h"
#include "BBDFilter.h"
#include "WowFlutter.h"
#include "ShelfFilter.h"
#include <juce_dsp/juce_dsp.h>

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
                      float  level,
                      float  low,
                      float  high);

    private:
        Lupex::DelayLine    delayL;
        Lupex::BBDFilter    filterOutL;
        WowFlutter          lfoL;

        Lupex::DelayLine    delayR;
        Lupex::BBDFilter    filterOutR;
        WowFlutter          lfoR;

        ShelfFilter lowShelfL  { ShelfFilter::Type::LowShelf  };
        ShelfFilter lowShelfR  { ShelfFilter::Type::LowShelf  };
        ShelfFilter highShelfL { ShelfFilter::Type::HighShelf };
        ShelfFilter highShelfR { ShelfFilter::Type::HighShelf };

        float smoothedLevel { 0.5f };
        float smoothedLow   { 0.5f };
        float smoothedHigh  { 0.5f };

        float dcBlockX_L { 0.0f }, dcBlockY_L { 0.0f };
        float dcBlockX_R { 0.0f }, dcBlockY_R { 0.0f };

        juce::dsp::Oversampling<float> oversampler {
            2,  // numero de canales
            1,  // factor: 2^1 = 2x
            juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR
        };

        int savedBlockSize { 0 };

        static constexpr float paramSmoothing { 0.002f };
        static constexpr float centerDelayMs  { 12.0f };
        static constexpr float maxDepthMs     {  2.0f };
        static constexpr float dcBlockR { 0.995f };
    };

} // namespace Chorus