#include "DelayLine.h"
#include <cmath>
#include <juce_core/juce_core.h>

namespace Lupex
{

    DelayLine::DelayLine() = default;

    void DelayLine::prepare (double sr, float maxMs)
    {
        sampleRate = sr;
        maxDelayMs = maxMs;

        int maxSamples = (int)(msToSamples(maxMs)) + 2;
        buffer.assign (maxSamples, 0.0f);

        writeIndex = 0;
        readPos = 0.0f;
    }

    void DelayLine::reset()
    {
        std::fill(buffer.begin(), buffer.end(), 0.0f);
        writeIndex = 0;
        readPos = 0.0f;
    }

    float DelayLine::msToSamples (float ms) const
    {
        return (float)(sampleRate * ms / 1000.0);
    }

    float DelayLine::read (float delayMs)
    {
        int bufferSize = (int)buffer.size();

        float targetDelaySamples = msToSamples(delayMs);
        float targetReadPos = (float)writeIndex - targetDelaySamples;
        // wrap target
        while (targetReadPos < 0.0f)
            targetReadPos += (float)bufferSize;

        // 🔥 smoothing del read pointer (clave para analog feel)
        float diff = targetReadPos - readPos;

        // wrap diff
        if (diff >  bufferSize * 0.5f) diff -= bufferSize;
        if (diff < -bufferSize * 0.5f) diff += bufferSize;

        // velocidad de adaptación
        float smoothing = 0.001f;
        float maxSpeed  = 2.5f;

        float step = diff * smoothing;

        // soft limiter
        step = maxSpeed * std::tanh(step / maxSpeed);

        readPos += step;

        // wrap readPos
        if (readPos < 0.0f) readPos += bufferSize;
        if (readPos >= bufferSize) readPos -= bufferSize;

        // 🔊 interpolación lineal
        int index0 = (int)readPos;
        int index1 = (index0 + 1) % bufferSize;

        float frac = readPos - (float)index0;

        float sample = buffer[index0] + frac * (buffer[index1] - buffer[index0]);

        return sample;
    }

    void DelayLine::write (float sample)
    {
        buffer[(size_t)writeIndex] = sample;

        writeIndex++;
        if (writeIndex >= (int)buffer.size())
            writeIndex = 0;
    }

} // namespace Lupex