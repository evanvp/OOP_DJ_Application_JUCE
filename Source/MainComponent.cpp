
#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    

    formatManager.registerBasicFormats();
    
    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);
    

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    // note that this will call prepareToPlay
    // automatically on the two players
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
}


void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}


//BELOW is the synthesis part (for record)

//void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
//{
//    if(!playing)
//    {
//        bufferToFill.clearActiveBufferRegion();
//        return;
//    }
//    
//    auto* leftChannel = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
//    auto* rightChannel = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
//    for (auto i=0;i<bufferToFill.numSamples; ++i)
//    {
//        auto sample = fmod(phase, 1.0f);
//        phase += fmod(dphase, 0.01f);
//        dphase += 0.0000005f;
//        leftChannel[i] = sample * 0.125 * gain;
//        rightChannel[i] = sample * 0.125 * gain;
//    }
//    //bufferToFill.clearActiveBufferRegion();
//}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
    g.setFont (20.0f);
}

void MainComponent::resized()
{
 
    deck1.setBounds(0, 0, getWidth()/2, getHeight());
    deck2.setBounds(getWidth()/2, 0, getWidth()/2, getHeight());

}

void MainComponent::buttonClicked(juce::Button* button)
{
    
}

void MainComponent::sliderValueChanged(juce
                                       ::Slider *slider)
{

}

