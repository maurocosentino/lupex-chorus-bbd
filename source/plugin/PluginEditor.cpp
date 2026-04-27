#include "PluginEditor.h"
#include <BinaryData.h>

namespace Chorus
{

ChorusEditor::ChorusEditor (ChorusProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setLookAndFeel (&laf);
    setSize (260, 470);

    addAndMakeVisible (knobRate);
    addAndMakeVisible (knobDepth);
    addAndMakeVisible (knobMix);
    addAndMakeVisible (bypass);

    auto& apvts = processor.parameters.apvts;
    attachRate  = std::make_unique<SliderAttachment> (apvts, "rate",  knobRate);
    attachDepth = std::make_unique<SliderAttachment> (apvts, "depth", knobDepth);
    attachMix   = std::make_unique<SliderAttachment> (apvts, "mix",   knobMix);
    attachBypass = std::make_unique<ButtonAttachment> (apvts, "bypass", bypass);

    processor.parameters.apvts.addParameterListener ("bypass", this);
}

ChorusEditor::~ChorusEditor()
{
    processor.parameters.apvts.removeParameterListener ("bypass", this);
    setLookAndFeel (nullptr);
}

void ChorusEditor::paint (juce::Graphics& g)
{
    drawPedalBody (g);

    bool bypassed = processor.parameters.getBypass();
    bool ledOn    = !bypassed;

    const int fsSize   = 92;
    const int fsOffset = 10;
    const int fsX      = getWidth() / 2 - fsSize / 2 + fsOffset;
    const int fsY      = getHeight() - 5 - fsSize;

    const int ledSize  = 18;
    const int ledX = fsX + fsSize + 8 - 20 + 10;
    const int ledY = fsY + fsSize / 2 - ledSize / 2 - 8 - 15;

    const float lx = (float)ledX;
    const float ly = (float)ledY;
    const float ls = (float)ledSize;

    if (ledOn)
    {
        g.setColour (juce::Colour (0x22dd4466));
        g.fillEllipse (lx - ls * 0.5f, ly - ls * 0.5f, ls * 2.0f, ls * 2.0f);
    }

    g.setColour (juce::Colour (0xff111111));
    g.fillEllipse (lx - 1.0f, ly - 1.0f, ls + 2.0f, ls + 2.0f);

    g.setColour (ledOn ? juce::Colour (0xffdd4466)
                       : juce::Colour (0xff1a2a2a));
    g.fillEllipse (lx, ly, ls, ls);

    g.setColour (ledOn ? juce::Colour (0x44990022)
                       : juce::Colour (0x66000000));
    g.fillEllipse (lx, ly + ls * 0.4f, ls, ls * 0.6f);

    g.setColour (juce::Colours::white.withAlpha (ledOn ? 0.85f : 0.2f));
    g.fillEllipse (lx + ls * 0.15f, ly + ls * 0.1f, ls * 0.4f, ls * 0.28f);

    g.setColour (juce::Colours::white.withAlpha (ledOn ? 0.4f : 0.08f));
    g.fillEllipse (lx + ls * 0.5f, ly + ls * 0.55f, ls * 0.2f, ls * 0.15f);
}

void ChorusEditor::resized()
{
    const int knobS    = 100;
    const int colLeft  = 5;
    const int colRight = getWidth() - 5 - knobS;

    // Rate (izq) y Depth (der) — fila 1
    knobRate .setBounds (colLeft,  20, knobS, knobS + 14);
    knobDepth.setBounds (colRight, 20, knobS, knobS + 14);

    // Mix — centrado — fila 2
    knobMix.setBounds (getWidth() / 2 - knobS / 2, 150, knobS, knobS + 14);

    // Bypass centrado abajo
    const int fsSize = 130;
    bypass.setBounds (getWidth() / 2 - fsSize / 2 + 10,
                      getHeight() - 5 - fsSize,
                      fsSize, fsSize);
}

void ChorusEditor::drawPedalBody (juce::Graphics& g)
{
    auto bg = juce::ImageCache::getFromMemory (
        BinaryData::background_png,
        BinaryData::background_pngSize);

    g.drawImageWithin (bg, 0, 0, getWidth(), getHeight(),
                       juce::RectanglePlacement::stretchToFit);
}

void ChorusEditor::drawBrandLabel (juce::Graphics& g)
{
    g.setColour (juce::Colour (0xff333333));
    g.setFont (juce::FontOptions (22.0f).withStyle ("Bold"));
    g.drawText ("LUPEX CHORUS",
                getWidth() / 2 - 80, 14, 160, 28,
                juce::Justification::centred);

    g.setColour (juce::Colour (0xffaaaaaa));
    g.setFont (juce::FontOptions (9.0f));
    g.drawText ("FAUZ AUDIO",
                getWidth() / 2 - 50, 42, 100, 14,
                juce::Justification::centred);
}

void ChorusEditor::parameterChanged (const juce::String&, float)
{
    juce::MessageManager::callAsync ([this] { repaint(); });
}

} // namespace Chorus