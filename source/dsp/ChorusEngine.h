#pragma once
#include "DelayLine.h"
#include "BBDFilter.h"
#include "TapeEmulator.h"
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
        Lupex::BBDFilter    filterInL;
        Lupex::BBDFilter    filterOutL;
        WowFlutter          lfoL;

        Lupex::DelayLine    delayR;
        Lupex::BBDFilter    filterInR;
        Lupex::BBDFilter    filterOutR;
        WowFlutter          lfoR;

        // Shelf filters sobre el wet — emula los tone controls del MXR
        ShelfFilter lowShelfL  { ShelfFilter::Type::LowShelf  };
        ShelfFilter lowShelfR  { ShelfFilter::Type::LowShelf  };
        ShelfFilter highShelfL { ShelfFilter::Type::HighShelf };
        ShelfFilter highShelfR { ShelfFilter::Type::HighShelf };

        static constexpr float centerDelayMs { 10.0f };
        static constexpr float maxDepthMs    {  1.5f };
    };

} // namespace Chorus