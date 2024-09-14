/*
  ==============================================================================

    DeckGUI.cpp
    Created: 30 Jan 2024 4:35:38pm
    Author:  Vincent UofL

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _djAudioPlayer, 
                 juce::AudioFormatManager & formatManagerToUse,
                 juce::AudioThumbnailCache & cacheToUse,
                 const juce::Colour _colour) : djAudioPlayer(_djAudioPlayer), waveformdisplay(formatManagerToUse, cacheToUse, _colour)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(reverbSlider);
    addAndMakeVisible(highpassSlider);
    addAndMakeVisible(lowpassSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(waveformdisplay);
    addAndMakeVisible(clearButton);
    
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::steelblue);
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::maroon);
    loadButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkolivegreen);
    loadButton.setButtonText("Load");
    volSlider.setRange(0, 1);
    speedSlider.setRange(0,10);
    speedSlider.setSliderStyle(juce::Slider::Rotary);
    
    //new sliders for reverb(0 to 1, start at 0), highpassSlider(20 to 20000Hz, start at 20), lowpassSlider(20 to 20000Hz, start at 20000)
    reverbSlider.setRange(0,1);
    reverbSlider.setSliderStyle(juce::Slider::Rotary);
    reverbSlider.setValue(0);
    highpassSlider.setRange(20,20000);
    highpassSlider.setSliderStyle(juce::Slider::Rotary);
    lowpassSlider.setRange(20,20000);
    lowpassSlider.setValue(20000);
    lowpassSlider.setSliderStyle(juce::Slider::Rotary);
    posSlider.setRange(0, 1);
    
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    reverbSlider.addListener(this);
    highpassSlider.addListener(this);
    lowpassSlider.addListener(this);
    posSlider.addListener(this);
    clearButton.addListener(this);
//for self callback timer frequency setup (200 = 5 per sec)
    startTimer(200);
    

}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colours::darkgrey);   // clear the background
    
    g.setColour(juce::Colours::black);
    g.drawRect(getLocalBounds(), 1);
    
 

}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    float rowH = getHeight() / 9;
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);
    volSlider.setBounds(0, rowH*2, getWidth(), rowH);
    speedSlider.setBounds(0, rowH*3, getWidth()/2, rowH);
    reverbSlider.setBounds(getWidth()/2, rowH*3, getWidth()/2, rowH);
    highpassSlider.setBounds(0, rowH*4, getWidth()/2, rowH);
    lowpassSlider.setBounds(getWidth()/2, rowH*4, getWidth()/2, rowH);
    waveformdisplay.setBounds(0, rowH*5, getWidth(), rowH*2);
    posSlider.setBounds(0, rowH*7, getWidth() * 3 / 4, rowH);
    clearButton.setBounds(getWidth() * 3 / 4, rowH*7, getWidth() / 4, rowH);
    loadButton.setBounds(0, rowH*8, getWidth(), rowH);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton )
    {
        djAudioPlayer->setPosition(0);
        djAudioPlayer->play();
        stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::firebrick);
    }
    
    if (button == &stopButton )
    {
        djAudioPlayer->stop();
        stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::maroon);
    }
//clear all audio processor values, set to default
    if (button == &clearButton )
    {
        volSlider.setValue(1);
        speedSlider.setValue(1);
        reverbSlider.setValue(0);
        highpassSlider.setValue(20);
        lowpassSlider.setValue(20000);
    }
    
    
    
    if (button == &loadButton)
    {
        auto fileChooserFlags =
        juce::FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
        {
            auto chosenFile = chooser.getResult();
            djAudioPlayer->loadURL(juce::URL{chosenFile});
            waveformdisplay.loadURL(juce::URL{chooser.getResult()});
        });
    }
}


void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        djAudioPlayer->setGain(slider->getValue());
       
    }
    if (slider == &posSlider)
    {
        djAudioPlayer->setPositionRelative(slider->getValue());
    }
    if (slider == &speedSlider)
    {
        
        djAudioPlayer->setSpeed(slider->getValue());
        
    }
    if (slider == &reverbSlider)
    {
        
        djAudioPlayer->setspace(slider->getValue());
    }
    
    if (slider == &highpassSlider)
    {
        djAudioPlayer->setHP(slider->getValue());
    }
    
    if (slider == &lowpassSlider)
    {
        djAudioPlayer->setLP(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped" << std::endl;
    if (files.size() == 1)
    {
        djAudioPlayer->loadURL(juce::URL{juce::File{files[0]}});
    }
}

void DeckGUI::timerCallback()
{
    waveformdisplay.setPositionRelative(djAudioPlayer->getPositionRelative());
}
