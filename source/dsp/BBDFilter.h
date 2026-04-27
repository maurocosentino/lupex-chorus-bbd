#pragma once

namespace Lupex
{

    class BBDFilter
    {
    public:
        BBDFilter();

        void prepare (double sampleRate);
        void reset();

        // tone: 0.0 = muy oscuro, 1.0 = casi sin filtro
        void setTone (float tone);

        float process (float input);

    private:
        float calculateCoefficient (float tone) const;

        double sampleRate { 44100.0 };
        float  coefficient { 0.8f };
        float  lastOutput  { 0.0f };  // estado interno del filtro
        float lastOutput2 { 0.0f };
    };

} // namespace Lupex