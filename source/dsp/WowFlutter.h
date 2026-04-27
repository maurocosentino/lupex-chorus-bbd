#pragma once
#include <cmath>

namespace Chorus
{
    class WowFlutter
    {
    public:
        void prepare (double sampleRate);
        void reset();

        float process();             // retorna offset en ms

        void setRate        (float hz);    // frecuencia del LFO — controlada por Rate
        void setDepth       (float ms);    // amplitud de modulación en ms
        void setPhaseOffset (float phase); // 0.0–1.0 (0.25 = 90°)

    private:
        double sampleRate   { 44100.0 };
        float  phase        { 0.0f };
        float  phaseOffset  { 0.0f };
        float  rate         { 1.0f };   // Hz
        float  depth        { 3.0f };   // ms de modulación

        static constexpr float twoPi { 6.28318f };
    };

} // namespace Chorus