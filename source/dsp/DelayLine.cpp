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

    float DelayLine::readDirect (float delayMs)
    {
    int bufferSize = (int)buffer.size();

    float delaySamples = msToSamples (delayMs);
    float pos = (float)writeIndex - delaySamples;

    while (pos < 0.0f)
        pos += (float)bufferSize;

    // Interpolación cúbica de Hermite — 4 samples
    int i1 = (int)pos % bufferSize;
    int i0 = (i1 - 1 + bufferSize) % bufferSize;
    int i2 = (i1 + 1) % bufferSize;
    int i3 = (i1 + 2) % bufferSize;

    float t  = pos - std::floor (pos);  // fracción entre 0 y 1

    float p0 = buffer[i0];
    float p1 = buffer[i1];
    float p2 = buffer[i2];
    float p3 = buffer[i3];

    // Coeficientes de Hermite
    float a = -0.5f*p0 + 1.5f*p1 - 1.5f*p2 + 0.5f*p3;
    float b =       p0 - 2.5f*p1 + 2.0f*p2 - 0.5f*p3;
    float c = -0.5f*p0            + 0.5f*p2;
    float d =                 p1;

    return ((a * t + b) * t + c) * t + d;
    }

    void DelayLine::write (float sample)
    {
        buffer[(size_t)writeIndex] = sample;

        writeIndex++;
        if (writeIndex >= (int)buffer.size())
            writeIndex = 0;
    }

} // namespace Lupex