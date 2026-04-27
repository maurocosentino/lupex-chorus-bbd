#include "LupexToggle.h"
#include <BinaryData.h>

namespace Lupex
{

    LupexToggle::LupexToggle()
        : juce::Button ("pingpong")
    {
        setClickingTogglesState (true);

        toggleStrip = juce::ImageCache::getFromMemory (
            BinaryData::switch_toggle_png,
            BinaryData::switch_toggle_pngSize);
    }

    void LupexToggle::paintButton (juce::Graphics& g,
                                    bool, bool)
    {
        // Frame 0 = down (off), Frame 1 = up (on)
        int sourceY = getToggleState() ? 0 : frameSize;

        g.drawImage (toggleStrip,
                     0, 0, getWidth(), getHeight(),
                     0, sourceY, frameSize, frameSize);
        g.setColour (juce::Colours::black.withAlpha (0.8f));
        g.setFont (juce::FontOptions ("Arial", 8.0f, juce::Font::bold));
        g.drawFittedText (label,
                  0, -2,
                  getWidth(), 14,
                  juce::Justification::centred, 1);
    }

} // namespace Lupex