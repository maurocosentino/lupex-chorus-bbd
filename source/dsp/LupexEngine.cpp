#include "LupexEngine.h"
#include <juce_core/juce_core.h>

namespace Lupex
{

    LupexEngine::LupexEngine() = default;

    void LupexEngine::prepare (double sampleRate, int)
    {
        delayL.prepare (sampleRate, 1200.0f);
        delayR.prepare (sampleRate, 1200.0f);
        filterL.prepare (sampleRate);
        filterR.prepare (sampleRate);
        feedbackFilterL.prepare (sampleRate);
        feedbackFilterR.prepare (sampleRate);
        wowFlutterL.prepare (sampleRate);
        wowFlutterR.prepare (sampleRate);
        wowFlutterL.setWowDepth     (0.8f);
        wowFlutterL.setFlutterDepth (0.08f);
        wowFlutterR.setWowDepth     (0.8f);
        wowFlutterR.setFlutterDepth (0.08f);
    }

    void LupexEngine::reset()
    {
        delayL.reset();
        delayR.reset();
        filterL.reset();
        filterR.reset();
        feedbackFilterL.reset();
        feedbackFilterR.reset();
        tapeL.reset();
        tapeR.reset();
        // wowFlutterL.reset();
        // wowFlutterR.reset();
    }

    void LupexEngine::process (float* channelL, float* channelR,
                            int    numSamples,
                            float  delayMs,
                            float  feedback,
                            float  mix,
                            float  tone)
    {
        filterL.setTone (tone);
        filterR.setTone (tone);
        feedbackFilterL.setTone (tone * 0.6f);  // feedback más oscuro que el wet
        feedbackFilterR.setTone (tone * 0.6f);
        tapeL.setDrive (0.15f);
        tapeR.setDrive (0.15f);

        for (int i = 0; i < numSamples; ++i)
        {
            // Smoother
            float diff = std::abs (delayMs - smoothedDelayMs);
            float adaptiveCoeff = 0.9995f + (1.0f - 0.9995f) * juce::jlimit (0.0f, 1.0f, 1.0f - diff / 500.0f);
            smoothedDelayMs = smoothedDelayMs * adaptiveCoeff + delayMs * (1.0f - adaptiveCoeff);

            float dryL = channelL[i];
            float dryR = channelR[i];

            float wowL = wowFlutterL.process();
            float wowR = wowFlutterR.process();

            float delayL_ms = smoothedDelayMs * (1.0f + stereoSpread) + wowL;
            float delayR_ms = smoothedDelayMs * (1.0f - stereoSpread) + wowR;

            float wetL = delayL.read (delayL_ms);
            float wetR = delayR.read (delayR_ms);

            // Transición suave entre modos
            float targetPPMix = pingPong ? 1.0f : 0.0f;
            pingPongMix += (targetPPMix - pingPongMix) * pingPongSlew;

            float fbL_normal = wetL * feedback;
            float fbR_normal = wetR * feedback;
            float fbL_cross  = wetR * feedback;
            float fbR_cross  = wetL * feedback;

            float fbL = fbL_normal * (1.0f - pingPongMix) + fbL_cross * pingPongMix;
            float fbR = fbR_normal * (1.0f - pingPongMix) + fbR_cross * pingPongMix;

            // Solo limitá si no esta en self-oscillation
            if (feedback < 0.95f)
            {
                fbL = juce::jlimit (-0.98f, 0.98f, fbL);
                fbR = juce::jlimit (-0.98f, 0.98f, fbR);
            }


            wetL = filterL.process (wetL);
            wetR = filterR.process (wetR);
            fbL = feedbackFilterL.process (fbL);  // feedback se oscurece acumulativamente
            fbR = feedbackFilterR.process (fbR);
            fbL = tapeL.process (fbL);
            fbR = tapeR.process (fbR);

            delayL.write (dryL + fbL);
            delayR.write (
                (dryR + fbR) * (1.0f - pingPongMix) +   // normal
                fbR           *  pingPongMix              // ping-pong
            );

            channelL[i] = applyMix (dryL, wetL, mix);
            channelR[i] = applyMix (dryR, wetR, mix);
        }
    }

    float LupexEngine::applyMix (float dry, float wet, float mix) const
    {
        return dry * (1.0f - mix) + wet * mix;
    }

    void LupexEngine::setPingPong (bool enabled)
    {
        pingPong = enabled;
    }

} // namespace Lupex