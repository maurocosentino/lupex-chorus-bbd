#pragma once

namespace Lupex
{

    class TapeEmulator
    {
    public:
        TapeEmulator();

        void reset();

        void setDrive (float drive);

        float process (float input);

    private:
        float drive { 0.3f };
    };

} // namespace Lupex