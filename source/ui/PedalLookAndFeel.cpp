#include "PedalLookAndFeel.h"

namespace Lupex
{

PedalLookAndFeel::PedalLookAndFeel()
{
    setColour (juce::Slider::rotarySliderFillColourId, accentColour());
    setColour (juce::Slider::thumbColourId,            accentColour());
}

void PedalLookAndFeel::drawRotarySlider (juce::Graphics& g,
                                          int x, int y, int width, int height,
                                          float sliderPos,
                                          float rotaryStartAngle,
                                          float rotaryEndAngle,
                                          juce::Slider&)
{
    float radius  = (float) juce::jmin (width, height) * 0.4f;
    float centreX = (float) x + (float) width  * 0.5f;
    float centreY = (float) y + (float) height * 0.5f;
    float angle   = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // Sombra exterior
    g.setColour (juce::Colour (0xff0a0a0a));
    g.fillEllipse (centreX - radius - 2.0f, centreY - radius - 2.0f,
                   (radius + 2.0f) * 2.0f,  (radius + 2.0f) * 2.0f);

    // Cuerpo del knob
    juce::ColourGradient gradient (juce::Colour (0xff4a4a4a),
                                   centreX - radius * 0.3f,
                                   centreY - radius * 0.3f,
                                   juce::Colour (0xff1e1e1e),
                                   centreX + radius * 0.5f,
                                   centreY + radius * 0.5f,
                                   true);
    g.setGradientFill (gradient);
    g.fillEllipse (centreX - radius, centreY - radius,
                   radius * 2.0f,    radius * 2.0f);

    // Borde
    g.setColour (juce::Colour (0xff555555));
    g.drawEllipse (centreX - radius, centreY - radius,
                   radius * 2.0f,    radius * 2.0f, 1.0f);

    // Indicador
    float indicatorLength = radius * 0.65f;
    float indicatorX = centreX + indicatorLength * std::sin (angle);
    float indicatorY = centreY - indicatorLength * std::cos (angle);

    g.setColour (accentColour());
    g.drawLine (centreX, centreY, indicatorX, indicatorY, 2.5f);
    g.fillEllipse (indicatorX - 2.5f, indicatorY - 2.5f, 5.0f, 5.0f);
}

void PedalLookAndFeel::drawToggleButton (juce::Graphics& g,
                                          juce::ToggleButton& button,
                                          bool, bool)
{
    auto  bounds = button.getLocalBounds().toFloat();
    bool  isOn   = button.getToggleState();

    g.setColour (juce::Colour (0xff1a1a1a));
    g.fillRoundedRectangle (bounds, 4.0f);

    g.setColour (isOn ? accentColour().withAlpha (0.6f)
                      : juce::Colour (0xff333333));
    g.drawRoundedRectangle (bounds.reduced (0.5f), 4.0f, 1.0f);

    float ledSize = 6.0f;
    float ledX    = bounds.getCentreX() - ledSize * 0.5f;
    float ledY    = bounds.getY() + 6.0f;

    g.setColour (isOn ? ledActiveColour() : ledInactiveColour());
    g.fillEllipse (ledX, ledY, ledSize, ledSize);

    if (isOn)
    {
        g.setColour (ledActiveColour().withAlpha (0.2f));
        g.fillEllipse (ledX - 3.0f, ledY - 3.0f,
                       ledSize + 6.0f, ledSize + 6.0f);
    }

    g.setColour (isOn ? accentColour() : textColour());
    g.setFont (juce::FontOptions (9.0f));
    g.drawFittedText (button.getButtonText(),
                      bounds.withTrimmedTop (ledY + ledSize + 2.0f).toNearestInt(),
                      juce::Justification::centred, 1);
}

} // namespace Lupex