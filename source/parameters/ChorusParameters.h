#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "ParameterIDs.h"

namespace Chorus
{
    class ChorusParameters
    {
    public:
        explicit ChorusParameters (juce::AudioProcessor& processor);

        juce::AudioProcessorValueTreeState apvts;

        float getRate()  const;
        float getDepth() const;
        float getMix()   const;
        bool  getBypass() const;

    private:
        static juce::AudioProcessorValueTreeState::ParameterLayout createLayout();
    };
} // namespace Chorus