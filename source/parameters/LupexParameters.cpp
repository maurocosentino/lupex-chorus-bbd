#include "LupexParameters.h"

namespace Lupex
{

LupexParameters::LupexParameters (juce::AudioProcessor& processor)
    : apvts (processor, nullptr, "LupexState", createLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout LupexParameters::createLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParameterIDs::TIME, 1 },
        "Time",
        juce::NormalisableRange<float> (1.0f, 1200.0f, 1.0f),
        300.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParameterIDs::FEEDBACK, 1 },
        "Feedback",
        juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f),
        40.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParameterIDs::MIX, 1 },
        "Mix",
        juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f),
        30.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParameterIDs::TONE, 1 },
        "Tone",
        juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f),
        70.0f));

    params.push_back (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParameterIDs::TOGGLE, 1 },
        "Ping-Pong",
        false));

    params.push_back (std::make_unique<juce::AudioParameterBool> (
    juce::ParameterID { ParameterIDs::BYPASS, 1 },
    "Bypass",
    false));

    return { params.begin(), params.end() };
}

float LupexParameters::getTime() const
{
    return apvts.getRawParameterValue (ParameterIDs::TIME)->load();
}
    
    float LupexParameters::getFeedback() const
{
    float raw = apvts.getRawParameterValue (ParameterIDs::FEEDBACK)->load();
    return raw / 100.0f;  // 0.0 a 1.0
}

float LupexParameters::getMix() const
{
    return apvts.getRawParameterValue (ParameterIDs::MIX)->load() / 100.0f;
}

float LupexParameters::getTone() const
{
    return apvts.getRawParameterValue (ParameterIDs::TONE)->load() / 100.0f;
}

bool LupexParameters::getToggle() const
{
    return apvts.getRawParameterValue (ParameterIDs::TOGGLE)->load() > 0.5f;
}
    bool LupexParameters::getBypass() const
{
    return apvts.getRawParameterValue (ParameterIDs::BYPASS)->load() > 0.5f;
}

} // namespace Lupex