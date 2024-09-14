/*
  ==============================================================================

    WaveformDisplay.h
    Created: 4 Feb 2024 11:14:17am
    Author:  Vincent UofL

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component, public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager & formatManagerToUse,
                    juce::AudioThumbnailCache & cacheToUse,
                    const juce::Colour & colour);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void changeListenerCallback (juce::ChangeBroadcaster *source) override;
    
//set resource to the AudioThumbnail
    void loadURL(juce::URL audioURL);
//set the playhead position according to current position given by "DeckGUI.h"
    void setPositionRelative(double pos);

private:
    juce::AudioThumbnail audioThumbnail;
    bool fileLoaded;
    juce::Colour colour;
    
    double position;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
