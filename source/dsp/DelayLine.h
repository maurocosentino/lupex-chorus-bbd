#pragma once

#include <vector>

namespace Lupex
{
    class DelayLine
    {
    public:
        DelayLine();

        void prepare (double sampleRate, float maxDelayMs);
        void reset();

        float read  (float delayMs);
        void  write (float sample);

    private:
        float msToSamples (float ms) const;

        std::vector<float> buffer;
        int    writeIndex { 0 };
        float  readPos    { 0.0f };

        double sampleRate { 44100.0 };
        float  maxDelayMs { 1200.0f };
    };
}