#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace Chorus
{

ChorusProcessor::ChorusProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      parameters (*this)
{
}

ChorusProcessor::~ChorusProcessor() {}

void ChorusProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    engine.prepare (sampleRate, samplesPerBlock);
}

void ChorusProcessor::releaseResources()
{
    engine.reset();
}

void ChorusProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                    juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    if (parameters.getBypass())
        return;

    engine.process (buffer.getWritePointer (0),
                    buffer.getWritePointer (1),
                    buffer.getNumSamples(),
                    parameters.getRate(),
                    parameters.getDepth(),
                    parameters.getMix());
}

juce::AudioProcessorEditor* ChorusProcessor::createEditor()
{
    return new ChorusEditor (*this);
}

bool ChorusProcessor::hasEditor() const { return true; }
const juce::String ChorusProcessor::getName() const { return JucePlugin_Name; }
bool ChorusProcessor::acceptsMidi() const { return false; }
bool ChorusProcessor::producesMidi() const { return false; }
double ChorusProcessor::getTailLengthSeconds() const { return 0.5; }
int ChorusProcessor::getNumPrograms() { return 1; }
int ChorusProcessor::getCurrentProgram() { return 0; }
void ChorusProcessor::setCurrentProgram (int) {}
const juce::String ChorusProcessor::getProgramName (int) { return {}; }
void ChorusProcessor::changeProgramName (int, const juce::String&) {}

void ChorusProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void ChorusProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary (data, sizeInBytes));
    if (xml && xml->hasTagName (parameters.apvts.state.getType()))
        parameters.apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

} // namespace Chorus

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Chorus::ChorusProcessor();
}