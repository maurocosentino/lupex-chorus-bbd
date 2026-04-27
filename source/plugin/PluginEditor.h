#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "../ui/PedalLookAndFeel.h"
#include "../ui/LupexKnob.h"
#include "../ui/LupexToggle.h"
#include "../ui/BypassButton.h"
#include "PluginProcessor.h"

namespace Lupex
{

    class LupexEditor : public juce::AudioProcessorEditor,
                        public juce::AudioProcessorValueTreeState::Listener
    {
    public:
        explicit LupexEditor (LupexProcessor&);
        ~LupexEditor() override;

        void paint   (juce::Graphics&) override;
        void resized() override;
        void parameterChanged (const juce::String& parameterID, float) override;


    private:
        LupexProcessor&   processor;
        PedalLookAndFeel  laf;

        LupexKnob knobTime     { "TIME",     laf };
        LupexKnob knobFeedback { "FEEDBACK", laf };
        LupexKnob knobTone     { "TONE",     laf };
        LupexKnob knobMix      { "MIX",      laf };

        LupexToggle toggle;
        BypassButton bypass;

        using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
        using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

        std::unique_ptr<SliderAttachment> attachTime;
        std::unique_ptr<SliderAttachment> attachFeedback;
        std::unique_ptr<SliderAttachment> attachTone;
        std::unique_ptr<SliderAttachment> attachMix;
        std::unique_ptr<ButtonAttachment> attachToggle;
        std::unique_ptr<ButtonAttachment> attachBypass;


        void drawPedalBody  (juce::Graphics&);
        void drawBypassLed  (juce::Graphics&);
        void drawBrandLabel (juce::Graphics&);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LupexEditor)
    };

} // namespace Lupex