#include "ChorusEngine.h"

namespace Chorus
{

ChorusEngine::ChorusEngine() = default;

void ChorusEngine::prepare (double sampleRate, int)
{
    // Máximo delay posible = center + maxDepth = 20ms
    const float maxDelayMs = centerDelayMs + maxDepthMs;

    delayL.prepare (sampleRate, maxDelayMs);
    delayR.prepare (sampleRate, maxDelayMs);

    filterInL.prepare  (sampleRate);
    filterInR.prepare  (sampleRate);
    filterOutL.prepare (sampleRate);
    filterOutR.prepare (sampleRate);

    lfoL.prepare (sampleRate);
    lfoR.prepare (sampleRate);

    // Canal R va 90° desfasado — imagen stereo ancha
    lfoL.setPhaseOffset (0.0f);
    lfoR.setPhaseOffset (0.25f);

    // Drive suave — carácter analógico sin distorsión evidente
    tapeL.setDrive (0.15f);
    tapeR.setDrive (0.15f);
}

void ChorusEngine::reset()
{
    delayL.reset();    delayR.reset();
    filterInL.reset(); filterInR.reset();
    filterOutL.reset();filterOutR.reset();
    tapeL.reset();     tapeR.reset();
    lfoL.reset();      lfoR.reset();
}

void ChorusEngine::process (float* channelL, float* channelR,
                             int    numSamples,
                             float  rate,
                             float  depth,
                             float  mix)
{
    // Actualizar LFOs con parámetros actuales
    // depth viene 0.0–1.0, lo escalamos a ms reales
    const float depthMs = depth * maxDepthMs;

    lfoL.setRate  (rate);
    lfoR.setRate  (rate);
    lfoL.setDepth (depthMs);
    lfoR.setDepth (depthMs);

    for (int i = 0; i < numSamples; ++i)
    {
        float dryL = channelL[i];
        float dryR = channelR[i];

        // LFO modula el delay time alrededor del centro
        float delayL_ms = centerDelayMs + lfoL.process();
        float delayR_ms = centerDelayMs + lfoR.process();

        // Cadena L: filtro in → delay → filtro out → tape
        float inL = filterInL.process (dryL);
        delayL.write (inL);
        float wetL = delayL.readDirect (delayL_ms);
        wetL = filterOutL.process (wetL);
        wetL = tapeL.process (wetL);

        // Cadena R: idéntica con LFO en cuadratura
        float inR = filterInR.process (dryR);
        delayR.write (inR);
        float wetR = delayR.readDirect (delayR_ms);
        wetR = filterOutR.process (wetR);
        wetR = tapeR.process (wetR);

        channelL[i] = applyMix (dryL,  wetL, mix);
        channelR[i] = applyMix (dryR, -wetR, mix);
    }
}

float ChorusEngine::applyMix (float dry, float wet, float mix) const
{
    return dry * (1.0f - mix) + wet * mix;
}

} // namespace Chorus