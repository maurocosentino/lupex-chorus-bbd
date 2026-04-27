#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "ParameterIDs.h"

namespace Lupex
{

    class LupexParameters
    {
    public:
        explicit LupexParameters (juce::AudioProcessor& processor);

        juce::AudioProcessorValueTreeState apvts;

        float getTime()     const;
        float getFeedback() const;
        float getMix()      const;
        float getTone()     const;
        bool  getToggle()   const;
        bool getBypass() const;

    private:
        static juce::AudioProcessorValueTreeState::ParameterLayout createLayout();
    };

} // namespace Lupex