#include "ChorusEngine.h"

namespace Chorus
{

ChorusEngine::ChorusEngine() = default;

void ChorusEngine::prepare (double sampleRate, int samplesPerBlock)
{
    savedBlockSize = samplesPerBlock;
    const double oversampledRate = sampleRate * 2.0;
    const float maxDelayMs = centerDelayMs + maxDepthMs;

    delayL.prepare (oversampledRate, maxDelayMs);
    delayR.prepare (oversampledRate, maxDelayMs);

    filterOutL.prepare (oversampledRate);
    filterOutR.prepare (oversampledRate);
    filterOutL.setTone (1.0f);
    filterOutR.setTone (1.0f);

    lfoL.prepare (oversampledRate);
    lfoR.prepare (oversampledRate);
    lfoL.setPhaseOffset (0.0f);
    lfoR.setPhaseOffset (0.25f);

    lowShelfL.prepare  (oversampledRate);
    lowShelfR.prepare  (oversampledRate);
    highShelfL.prepare (oversampledRate);
    highShelfR.prepare (oversampledRate);

    oversampler.initProcessing (samplesPerBlock);
}

void ChorusEngine::reset()
{
    delayL.reset();     delayR.reset();
    filterOutL.reset(); filterOutR.reset();
    lfoL.reset();       lfoR.reset();
    lowShelfL.reset();  lowShelfR.reset();
    highShelfL.reset(); highShelfR.reset();
    dcBlockX_L = dcBlockY_L = 0.0f;
    dcBlockX_R = dcBlockY_R = 0.0f;
    oversampler.reset();
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

    // Preparar AudioBlock para el oversampler
    float* channels[2] = { channelL, channelR };
    juce::dsp::AudioBlock<float> block (channels, 2, (size_t)numSamples);
    auto oversampledBlock = oversampler.processSamplesUp (block);

    const int oversampledSize = (int)oversampledBlock.getNumSamples();
    float* outL = oversampledBlock.getChannelPointer (0);
    float* outR = oversampledBlock.getChannelPointer (1);

    for (int i = 0; i < oversampledSize; ++i)
    {
        smoothedLevel += (level - smoothedLevel) * paramSmoothing;
        smoothedLow   += (low   - smoothedLow)   * paramSmoothing;
        smoothedHigh  += (high  - smoothedHigh)  * paramSmoothing;

        lowShelfL.setGain  (smoothedLow);  lowShelfR.setGain  (smoothedLow);
        highShelfL.setGain (smoothedHigh); highShelfR.setGain (smoothedHigh);

        float dryL = outL[i];
        float dryR = outR[i];

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

        // DC blocking
        float newDcL = wetL - dcBlockX_L + dcBlockR * dcBlockY_L;
        dcBlockX_L = wetL; dcBlockY_L = newDcL; wetL = newDcL;

        float newDcR = wetR - dcBlockX_R + dcBlockR * dcBlockY_R;
        dcBlockX_R = wetR; dcBlockY_R = newDcR; wetR = newDcR;

        outL[i] = dryL + wetL * smoothedLevel;
        outR[i] = dryR * 0.7f + wetR * smoothedLevel;
    }

    oversampler.processSamplesDown (block);
}

} // namespace Chorus