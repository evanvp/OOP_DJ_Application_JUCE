/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 4 Feb 2024 11:14:17am
    Author:  Vincent UofL

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager & formatManagerToUse,
                                 juce::AudioThumbnailCache & cacheToUse,
                                 const juce::Colour & _colour): audioThumbnail(1000, formatManagerToUse, cacheToUse), fileLoaded(false), colour(_colour), position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    audioThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colours::black); 

    g.setColour (juce::Colours::black);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

//change the printed text to waveform depends on the file loading status
    if (fileLoaded)
    {
        g.setColour(colour);
        audioThumbnail.drawChannel(g, getLocalBounds(), 0, audioThumbnail.getTotalLength(), 0, 1);
        
        g.setColour(juce::Colours::yellow);
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
        
    }
    else
    {
        g.setColour (colour);
        g.setFont (24.0f);
        g.drawText ("Waveform here!!", getLocalBounds(), juce::Justification::centred, true); // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new juce::URLInputSource(audioURL));
    
}

void WaveformDisplay::changeListenerCallback (juce::ChangeBroadcaster *source)
{
    std::cout << "wfd: change received! " << std::endl;
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position and pos > 0)
    {
        position = pos;
        repaint();
    }
}

