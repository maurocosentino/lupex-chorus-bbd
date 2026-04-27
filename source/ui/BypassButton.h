#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace Lupex
{

    class BypassButton : public juce::Button
    {
    public:
        BypassButton();

        void paintButton (juce::Graphics&,
                          bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown) override;

    private:
        juce::Image imgUp;
        juce::Image imgDown;
    };

} // namespace Lupex