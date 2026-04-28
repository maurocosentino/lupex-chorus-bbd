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

    // Level: volumen del wet — dry siempre al 100%
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParameterIDs::LEVEL, 1 },
        "Level",
        juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f),
        50.0f));

    // Low: shelf de graves del wet (0% = corte máximo, 100% = flat)
    // Rango real MXR: 70Hz a 800Hz
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParameterIDs::LOW, 1 },
        "Low",
        juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f),
        50.0f));

    // High: shelf de agudos del wet (0% = corte máximo, 100% = flat)
    // Rango real MXR: 660Hz a flat
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParameterIDs::HIGH, 1 },
        "High",
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
    return apvts.getRawParameterValue (ParameterIDs::DEPTH)->load() / 100.0f;
}

float ChorusParameters::getLevel() const
{
    return apvts.getRawParameterValue (ParameterIDs::LEVEL)->load() / 100.0f;
}

float ChorusParameters::getLow() const
{
    return apvts.getRawParameterValue (ParameterIDs::LOW)->load() / 100.0f;
}

float ChorusParameters::getHigh() const
{
    return apvts.getRawParameterValue (ParameterIDs::HIGH)->load() / 100.0f;
}

bool ChorusParameters::getBypass() const
{
    return apvts.getRawParameterValue (ParameterIDs::BYPASS)->load() > 0.5f;
}

} // namespace Chorus