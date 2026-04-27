#include "BypassButton.h"
#include <BinaryData.h>

namespace Lupex
{

    BypassButton::BypassButton()
        : juce::Button ("bypass")
    {
        setClickingTogglesState (true);

        imgUp   = juce::ImageCache::getFromMemory (
            BinaryData::footswitch_up_png,
            BinaryData::footswitch_up_pngSize);

        imgDown = juce::ImageCache::getFromMemory (
            BinaryData::footswitch_down_png,
            BinaryData::footswitch_down_pngSize);
    }

    void BypassButton::paintButton (juce::Graphics& g,
                                     bool,
                                     bool isDown)
    {

        auto& img = isDown ? imgDown : imgUp;

        g.drawImage (img,
                     0, 0, getWidth(), getHeight(),
                     0, 0, img.getWidth(), img.getHeight());
    }

} // namespace Lupex