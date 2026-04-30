#pragma once
#include "DelayLine.h"
#include "BBDFilter.h"
#include "WowFlutter.h"
#include "ShelfFilter.h"

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

        static constexpr float paramSmoothing { 0.002f };
        static constexpr float centerDelayMs  { 12.0f };
        static constexpr float maxDepthMs     {  2.0f };
    };

} // namespace Chorus