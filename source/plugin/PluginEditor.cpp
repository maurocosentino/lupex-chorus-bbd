#include "PluginEditor.h"
#include <BinaryData.h>
namespace Lupex
{

    LupexEditor::LupexEditor (LupexProcessor& p)
        : AudioProcessorEditor (&p), processor (p)
    {
        setLookAndFeel (&laf);
        setSize (260, 470);

        addAndMakeVisible (knobTime);
        addAndMakeVisible (knobFeedback);
        addAndMakeVisible (knobTone);
        addAndMakeVisible (knobMix);
        addAndMakeVisible (toggle);
        addAndMakeVisible (bypass);

        auto& apvts = processor.parameters.apvts;
        attachTime     = std::make_unique<SliderAttachment> (apvts, "time",     knobTime);
        attachFeedback = std::make_unique<SliderAttachment> (apvts, "feedback", knobFeedback);
        attachTone     = std::make_unique<SliderAttachment> (apvts, "tone",     knobTone);
        attachMix      = std::make_unique<SliderAttachment> (apvts, "mix",      knobMix);
        attachToggle = std::make_unique<ButtonAttachment> (apvts, "pingpong", toggle);
        attachBypass = std::make_unique<ButtonAttachment> (apvts, "bypass", bypass);
        processor.parameters.apvts.addParameterListener ("bypass", this);
    }

    LupexEditor::~LupexEditor()
    {
        processor.parameters.apvts.removeParameterListener ("bypass", this);
        setLookAndFeel (nullptr);
    }

    void LupexEditor::paint (juce::Graphics& g)
    {
        drawPedalBody (g);
        // drawBrandLabel (g); //Titulos
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
            // Glow
            g.setColour (juce::Colour (0x22dd4466));
            g.fillEllipse (lx - ls * 0.5f, ly - ls * 0.5f, ls * 2.0f, ls * 2.0f);
        }

        // Housing — borde oscuro exterior
        g.setColour (juce::Colour (0xff111111));
        g.fillEllipse (lx - 1.0f, ly - 1.0f, ls + 2.0f, ls + 2.0f);

        // Cuerpo del LED
        g.setColour (ledOn ? juce::Colour (0xffdd4466)
                   : juce::Colour (0xff1a2a2a));
        g.fillEllipse (lx, ly, ls, ls);

        // Capa de profundidad — mitad inferior más oscura
        g.setColour (ledOn ? juce::Colour (0x44990022)
                   : juce::Colour (0x66000000));
        g.fillEllipse (lx, ly + ls * 0.4f, ls, ls * 0.6f);

        // Reflejo especular principal — óvalo arriba a la izquierda
        g.setColour (juce::Colours::white.withAlpha (ledOn ? 0.85f : 0.2f));
        g.fillEllipse (lx + ls * 0.15f, ly + ls * 0.1f, ls * 0.4f, ls * 0.28f);

        // Reflejo secundario pequeño
        g.setColour (juce::Colours::white.withAlpha (ledOn ? 0.4f : 0.08f));
        g.fillEllipse (lx + ls * 0.5f, ly + ls * 0.55f, ls * 0.2f, ls * 0.15f);
    }

    void LupexEditor::resized()
    {
        const int knobS   = 100;
        const int topY1 = 10;   // fila 1
        const int topY2 = 120;  // fila 2
        const int colLeft  = 5;
        const int colRight = getWidth() - 5 - knobS;

        // ── Fila 1: TIME (izq) y FEEDBACK (der) ──────────────
        knobTime    .setBounds (colLeft,  topY1, knobS, knobS + 14);
        knobFeedback.setBounds (colRight, topY1, knobS, knobS + 14);

        // ── Fila 2: TONE (izq) y MIX (der) ───────────────────
        knobTone.setBounds (colLeft,  topY2, knobS, knobS + 14);
        knobMix .setBounds (colRight, topY2, knobS, knobS + 14);

        // ── Toggle centrado entre las 4 filas ─────────────────
        const int toggleSz = 52 + 14;  // +14 para el label
        const int toggleX = getWidth() / 2 - toggleSz / 2;
        const int toggleY = (topY1 + knobS + 14 + topY2) / 2 - toggleSz / 2;
        toggle.setBounds (toggleX, toggleY, toggleSz, toggleSz);

        // ── Bypass centrado abajo ──────────────────────────────
        const int fsSize = 130;
        bypass.setBounds (getWidth() / 2 - fsSize / 2 + 10,
                          getHeight() - 5 - fsSize,
                          fsSize, fsSize);
    }

    void LupexEditor::drawPedalBody (juce::Graphics& g)
{
    auto bg = juce::ImageCache::getFromMemory (
        BinaryData::background_png,
        BinaryData::background_pngSize);

    g.drawImageWithin (bg, 0, 0, getWidth(), getHeight(),
                       juce::RectanglePlacement::stretchToFit);
}

void LupexEditor::drawBrandLabel (juce::Graphics& g)
{
    g.setColour (juce::Colour (0xff333333));
    g.setFont (juce::FontOptions (22.0f).withStyle ("Bold"));
    g.drawText ("LUPEX",
                getWidth() / 2 - 60, 14, 120, 28,
                juce::Justification::centred);

    g.setColour (juce::Colour (0xffaaaaaa));
    g.setFont (juce::FontOptions (9.0f));
    g.drawText ("FAUZ AUDIO",
                getWidth() / 2 - 50, 42, 100, 14,
                juce::Justification::centred);
}

    void LupexEditor::parameterChanged (const juce::String&, float)
    {
        // Este callback llega desde el audio thread → hay que redibujar en el mensaje thread
        juce::MessageManager::callAsync ([this] { repaint(); });
    }

} // namespace Lupex