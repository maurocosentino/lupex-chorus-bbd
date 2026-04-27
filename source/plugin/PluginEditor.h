#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "../ui/PedalLookAndFeel.h"
#include "../ui/LupexKnob.h"
#include "../ui/BypassButton.h"
#include "PluginProcessor.h"

namespace Chorus
{

    class ChorusEditor : public juce::AudioProcessorEditor,
                         public juce::AudioProcessorValueTreeState::Listener
    {
    public:
        explicit ChorusEditor (ChorusProcessor&);
        ~ChorusEditor() override;

        void paint   (juce::Graphics&) override;
        void resized() override;
        void parameterChanged (const juce::String& parameterID, float) override;

    private:
        ChorusProcessor&        processor;
        Lupex::PedalLookAndFeel laf;

        Lupex::LupexKnob knobRate  { "RATE",  laf };
        Lupex::LupexKnob knobDepth { "DEPTH", laf };
        Lupex::LupexKnob knobMix   { "MIX",   laf };

        Lupex::BypassButton bypass;

        using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
        using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

        std::unique_ptr<SliderAttachment> attachRate;
        std::unique_ptr<SliderAttachment> attachDepth;
        std::unique_ptr<SliderAttachment> attachMix;
        std::unique_ptr<ButtonAttachment> attachBypass;

        void drawPedalBody  (juce::Graphics&);
        void drawBrandLabel (juce::Graphics&);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChorusEditor)
    };

} // namespace Chorus