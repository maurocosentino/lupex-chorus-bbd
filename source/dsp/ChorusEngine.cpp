#include "ChorusEngine.h"

namespace Chorus
{

ChorusEngine::ChorusEngine() = default;

void ChorusEngine::prepare (double sampleRate, int)
{
    const float maxDelayMs = centerDelayMs + maxDepthMs;

    delayL.prepare (sampleRate, maxDelayMs);
    delayR.prepare (sampleRate, maxDelayMs);

    filterOutL.prepare (sampleRate);
    filterOutR.prepare (sampleRate);
    filterOutL.setTone (1.0f);
    filterOutR.setTone (1.0f);

    lfoL.prepare (sampleRate);
    lfoR.prepare (sampleRate);
    lfoL.setPhaseOffset (0.0f);
    lfoR.setPhaseOffset (0.25f);

    lowShelfL.prepare  (sampleRate);
    lowShelfR.prepare  (sampleRate);
    highShelfL.prepare (sampleRate);
    highShelfR.prepare (sampleRate);
}

void ChorusEngine::reset()
{
    delayL.reset();     delayR.reset();
    filterOutL.reset(); filterOutR.reset();
    lfoL.reset();       lfoR.reset();
    lowShelfL.reset();  lowShelfR.reset();
    highShelfL.reset(); highShelfR.reset();
}

void ChorusEngine::process (float* channelL, float* channelR,
                             int    numSamples,
                             float  rate,
                             float  depth,
                             float  level,
                             float  low,
                             float  high)
{
    const float depthMs = depth * maxDepthMs;

    lfoL.setRate  (rate);    lfoR.setRate  (rate);
    lfoL.setDepth (depthMs); lfoR.setDepth (depthMs);

    for (int i = 0; i < numSamples; ++i)
    {
        smoothedLevel += (level - smoothedLevel) * paramSmoothing;
        smoothedLow   += (low   - smoothedLow)   * paramSmoothing;
        smoothedHigh  += (high  - smoothedHigh)  * paramSmoothing;

        lowShelfL.setGain  (smoothedLow);  lowShelfR.setGain  (smoothedLow);
        highShelfL.setGain (smoothedHigh); highShelfR.setGain (smoothedHigh);

        float dryL = channelL[i];
        float dryR = channelR[i];

        float delayL_ms = centerDelayMs + lfoL.process();
        float delayR_ms = centerDelayMs + lfoR.process();

        delayL.write (dryL);
        float wetL = delayL.readDirect (delayL_ms);
        wetL = filterOutL.process (wetL);
        wetL = lowShelfL.process  (wetL);
        wetL = highShelfL.process (wetL);

        delayR.write (dryR);
        float wetR = delayR.readDirect (delayR_ms);
        wetR = filterOutR.process (wetR);
        wetR = lowShelfR.process  (wetR);
        wetR = highShelfR.process (wetR);

        channelL[i] = dryL + wetL * smoothedLevel;
        channelR[i] = dryR * 0.7f + wetR * smoothedLevel;
    }
}

} // namespace Chorus