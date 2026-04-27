#include "ChorusParameters.h"

namespace Chorus
{

    ChorusParameters::ChorusParameters (juce::AudioProcessor& processor)
        : apvts (processor, nullptr, "ChorusState", createLayout())
    {
    }

    juce::AudioProcessorValueTreeState::ParameterLayout ChorusParameters::createLayout()
    {
        std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

        // Rate: velocidad del LFO en Hz
        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { ParameterIDs::RATE, 1 },
            "Rate",
            juce::NormalisableRange<float> (0.1f, 5.0f, 0.01f),
            1.0f));

        // Depth: profundidad de modulación del delay time
        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { ParameterIDs::DEPTH, 1 },
            "Depth",
            juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f),
            50.0f));

        // Mix: balance wet/dry
        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { ParameterIDs::MIX, 1 },
            "Mix",
            juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f),
            50.0f));

        // Bypass
        params.push_back (std::make_unique<juce::AudioParameterBool> (
            juce::ParameterID { ParameterIDs::BYPASS, 1 },
            "Bypass",
            false));

        return { params.begin(), params.end() };
    }

    float ChorusParameters::getRate() const
    {
        return apvts.getRawParameterValue (ParameterIDs::RATE)->load();
    }

    float ChorusParameters::getDepth() const
    {
        // 0.0 a 1.0 — el engine lo escala a ms de modulación
        return apvts.getRawParameterValue (ParameterIDs::DEPTH)->load() / 100.0f;
    }

    float ChorusParameters::getMix() const
    {
        return apvts.getRawParameterValue (ParameterIDs::MIX)->load() / 100.0f;
    }

    bool ChorusParameters::getBypass() const
    {
        return apvts.getRawParameterValue (ParameterIDs::BYPASS)->load() > 0.5f;
    }

} // namespace Chorus