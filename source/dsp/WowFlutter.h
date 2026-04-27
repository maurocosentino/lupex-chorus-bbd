#pragma once
#include <cmath>

namespace Lupex
{
    class WowFlutter
    {
    public:
        void prepare (double sampleRate);
        void reset();
        float process();  // retorna offset en ms

        void setWowDepth     (float ms);   // 0.0–0.5ms
        void setFlutterDepth (float ms);   //  0.0–0.15ms

    private:
        double sampleRate { 44100.0 };

        // Wow
        float wowPhase      { 0.0f };
        float wowDepth      { 0.3f };
        float wowRate       { 0.8f };   // variable, no constexpr

        // Modulador del wow rate
        float modPhase      { 0.0f };
        static constexpr float modRate  { 0.1f };   // Hz — muy lento
        static constexpr float modDepth { 0.3f };   // cuánto varía el wowRate

        // Flutter
        float flutterPhase  { 0.0f };
        float flutterDepth  { 0.08f };
        static constexpr float flutterRate { 12.0f };

        // Ruido
        float noiseState    { 0.0f };
        static constexpr float noiseSmooth { 0.998f };

        float nextNoise();
    };
}