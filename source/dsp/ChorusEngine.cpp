#include "ChorusEngine.h"

namespace Chorus
{

ChorusEngine::ChorusEngine() = default;

void ChorusEngine::prepare (double sampleRate, int)
{
    const float maxDelayMs = centerDelayMs + maxDepthMs;

    delayL.prepare (sampleRate, maxDelayMs);
    delayR.prepare (sampleRate, maxDelayMs);

    filterInL.prepare  (sampleRate);
    filterInR.prepare  (sampleRate);
    filterOutL.prepare (sampleRate);
    filterOutR.prepare (sampleRate);

    lfoL.prepare (sampleRate);
    lfoR.prepare (sampleRate);

    lfoL.setPhaseOffset (0.0f);
    lfoR.setPhaseOffset (0.25f);

    // Filtros BBD con cutoff alto y fijo
    filterInL.setTone  (1.0f);
    filterInR.setTone  (1.0f);
    filterOutL.setTone (1.0f);
    filterOutR.setTone (1.0f);

    lowShelfL.prepare  (sampleRate);
    lowShelfR.prepare  (sampleRate);
    highShelfL.prepare (sampleRate);
    highShelfR.prepare (sampleRate);
}

void ChorusEngine::reset()
{
    delayL.reset();     delayR.reset();
    filterInL.reset();  filterInR.reset();
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

    lfoL.setRate  (rate);   lfoR.setRate  (rate);
    lfoL.setDepth (depthMs); lfoR.setDepth (depthMs);

    // Shelf filters — se actualizan una vez por bloque
    lowShelfL.setGain  (low);   lowShelfR.setGain  (low);
    highShelfL.setGain (high);  highShelfR.setGain (high);

    for (int i = 0; i < numSamples; ++i)
    {
        float dryL = channelL[i];
        float dryR = channelR[i];

        float delayL_ms = centerDelayMs + lfoL.process();
        float delayR_ms = centerDelayMs + lfoR.process();

        // Cadena L: filtro in → delay → filtro out → tone shaping
        float inL = filterInL.process (dryL);
        delayL.write (inL);
        float wetL = delayL.readDirect (delayL_ms);
        wetL = filterOutL.process (wetL);
        wetL = lowShelfL.process  (wetL);
        wetL = highShelfL.process (wetL);

        // Cadena R: idéntica con LFO en cuadratura
        float inR = filterInR.process (dryR);
        delayR.write (inR);
        float wetR = delayR.readDirect (delayR_ms);
        wetR = filterOutR.process (wetR);
        wetR = lowShelfR.process  (wetR);
        wetR = highShelfR.process (wetR);

        // dry intacto + wet escalado por level
        channelL[i] = dryL + wetL * level;
        channelR[i] = dryR + wetR * level;
    }
}

} // namespace Chorus