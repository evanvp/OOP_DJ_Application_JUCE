/*
  ==============================================================================

    DeckGUI.h
    Created: 30 Jan 2024 4:35:38pm
    Author:  Vincent UofL

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component, public juce::Button::Listener, public juce::Slider::Listener, public juce::FileDragAndDropTarget, public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* _djAudioPlayer, 
            juce::AudioFormatManager & formatManagerToUse,
            juce::AudioThumbnailCache & cacheToUse,
            const juce::Colour colour);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged (juce::Slider *slider) override;
    
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    
    void timerCallback() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
    
    DJAudioPlayer* djAudioPlayer;
    WaveformDisplay waveformdisplay;
    
    juce::TextButton playButton{"Play"};
    juce::TextButton stopButton{"Stop"};
    juce::TextButton loadButton;
    juce::FileChooser fChooser{"Select a file..."};
    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider reverbSlider;
    juce::Slider highpassSlider;
    juce::Slider lowpassSlider;
    juce::Slider posSlider;
    juce::TextButton clearButton{"Clear"};
    

    
};
