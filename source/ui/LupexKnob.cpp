#include "LupexKnob.h"
#include <BinaryData.h>

namespace Lupex
{

LupexKnob::LupexKnob (const juce::String& labelText, PedalLookAndFeel& laf)
    : label (labelText)
{
    setLookAndFeel (&laf);
    setSliderStyle (juce::Slider::RotaryVerticalDrag);
    setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    setRotaryParameters (juce::MathConstants<float>::pi * 1.25f,
                         juce::MathConstants<float>::pi * 2.75f,
                         true);

    knobStrip = juce::ImageCache::getFromMemory (
        BinaryData::knob_png,
        BinaryData::knob_pngSize);
}

void LupexKnob::paint (juce::Graphics& g)
{
    float normalizedValue = static_cast<float> (
        (getValue() - getMinimum()) / (getMaximum() - getMinimum()));

    int frameIndex = juce::jlimit (0, totalFrames - 1,
                     static_cast<int> (normalizedValue * (totalFrames - 1)));

    int sourceY = frameIndex * frameSize;
    int size    = juce::jmin (getWidth(), getHeight() - 14);
    int offsetX = 0;

    g.drawImage (knobStrip,
                 offsetX, 0, size, size,
                 0, sourceY, frameSize, frameSize);

    g.setColour (juce::Colours::black.withAlpha (0.8f));
    g.setFont (juce::FontOptions ("Arial", 11.5f, juce::Font::bold));
    g.drawFittedText (label,
                      0, getHeight() - 16,
                      getWidth(), 14,
                      juce::Justification::centred, 1);
}

void LupexKnob::resized()
{
    juce::Slider::resized();
}

} // namespace Lupex