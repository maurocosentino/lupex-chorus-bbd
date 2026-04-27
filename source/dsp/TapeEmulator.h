#pragma once

namespace Lupex
{

    class TapeEmulator
    {
    public:
        TapeEmulator();

        void reset();

        // drive: cuánta saturación aplicamos (0.0 = ninguna, 1.0 = máxima)
        void setDrive (float drive);

        float process (float input);

    private:
        float drive { 0.3f };
    };

} // namespace Lupex