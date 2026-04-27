#pragma once

#include "PedalLookAndFeel.h"

namespace Lupex
{

    class LupexKnob : public juce::Slider
    {
    public:
        LupexKnob (const juce::String& labelText, PedalLookAndFeel& laf);

        void paint   (juce::Graphics&) override;
        void resized() override;

    private:
        juce::String label;
        juce::Image  knobStrip;

        static constexpr int totalFrames { 101 };
        static constexpr int frameSize   { 128 };
    };

} // namespace Lupex