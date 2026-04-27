#include "TapeEmulator.h"
#include <cmath>

namespace Lupex
{

    TapeEmulator::TapeEmulator() = default;

    void TapeEmulator::reset() {}

    void TapeEmulator::setDrive (float d)
    {
        drive = d;
    }

    float TapeEmulator::process (float input)
    {
        // Drive muy sutil — solo colorea, no distorsiona
        float amplified = input * (1.0f + drive * 0.8f);

        // Curva de Langevin
        float saturated = amplified / (std::abs (amplified) + 1.0f);

        return saturated;
    }

} // namespace Lupex