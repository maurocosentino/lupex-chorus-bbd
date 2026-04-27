#include "WowFlutter.h"
#include <cmath>
#include <algorithm>
#include <cstdlib>

namespace Lupex
{
    void WowFlutter::prepare (double sr)
    {
        sampleRate = sr;
        // Fases aleatorias — L y R arrancan distintos
        wowPhase     = static_cast<float> (std::rand()) / RAND_MAX;
        flutterPhase = static_cast<float> (std::rand()) / RAND_MAX;
        modPhase     = static_cast<float> (std::rand()) / RAND_MAX;
        noiseState   = 0.0f;
    }

    void WowFlutter::reset()
    {
        wowPhase    = 0.0f;
        flutterPhase = 0.0f;
        noiseState  = 0.0f;
    }

    float WowFlutter::process()
    {
        // Modulador lento del wowRate
        modPhase += static_cast<float> (modRate / sampleRate);
        if (modPhase > 1.0f) modPhase -= 1.0f;

        float currentWowRate = wowRate + std::sin (modPhase * 6.28318f) * modDepth;
        currentWowRate = std::max (0.1f, currentWowRate);

        // Wow con rate modulado
        wowPhase += static_cast<float> (currentWowRate / sampleRate);
        if (wowPhase > 1.0f) wowPhase -= 1.0f;

        // Flutter
        flutterPhase += static_cast<float> (flutterRate / sampleRate);
        if (flutterPhase > 1.0f) flutterPhase -= 1.0f;

        float wow     = std::sin (wowPhase     * 6.28318f) * wowDepth;
        float flutter = std::sin (flutterPhase * 6.28318f) * flutterDepth;

        // Ruido orgánico mezclado con wow
        float noise = nextNoise();
        wow += noise * wowDepth * 0.3f;

        return wow + flutter;
    }

    float WowFlutter::nextNoise()
    {
        float raw = (static_cast<float> (std::rand()) / RAND_MAX) * 2.0f - 1.0f;
        noiseState = noiseState * noiseSmooth + raw * (1.0f - noiseSmooth);
        return noiseState;
    }

    void WowFlutter::setWowDepth (float ms)     { wowDepth     = ms; }
    void WowFlutter::setFlutterDepth (float ms) { flutterDepth = ms; }
}