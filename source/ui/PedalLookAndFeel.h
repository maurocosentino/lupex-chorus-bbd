#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace Lupex
{

    class PedalLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        PedalLookAndFeel();

        void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height,
                               float sliderPosProportional, float rotaryStartAngle,
                               float rotaryEndAngle, juce::Slider&) override;

        void drawToggleButton (juce::Graphics&, juce::ToggleButton&,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override;

        static juce::Colour backgroundColour()  { return juce::Colour (0xfff0eeea); }
        static juce::Colour knobColour()        { return juce::Colour (0xff2e2e2e); }
        static juce::Colour accentColour()      { return juce::Colour (0xffffb040); }
        static juce::Colour ledActiveColour()   { return juce::Colour (0xff4dff88); }
        static juce::Colour ledInactiveColour() { return juce::Colour (0xff1a3320); }
        static juce::Colour textColour()        { return juce::Colour (0xff888888); }
    };

} // namespace Lupex