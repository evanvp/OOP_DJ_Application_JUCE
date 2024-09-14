/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 17 Jan 2024 7:19:54pm
    Author:  Vincent UofL

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource
{
    public:
        DJAudioPlayer(juce::AudioFormatManager& formatManager);
        ~DJAudioPlayer();
        void loadURL(juce::URL file);
        void play();
        void stop();
        void setPosition(double posInSecs);
        void setSpeed(double ratio);
        void setPositionRelative(double pos);
        void setGain(double gain);
        void setspace(double spaceratio);
       //new fuunction for adjust freqency cutoff for the filters
        void setHP(double frequency);
        void setLP(double frequency);
    
    
        void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;
    
        double getPositionRelative();
    
        juce::Reverb::Parameters reverbParameters;
        

        
    private:
        juce::AudioFormatManager& formatManager;
        std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
        juce::AudioTransportSource transportSource;
        juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
    
    //new AudioSource classes are called here, for the extension features
        juce::ReverbAudioSource reverbSource{&resampleSource, false};
        
        double filterSamplerate;
        double HPfrequency;
        juce::IIRCoefficients HPC;
        juce::IIRFilterAudioSource IIRHighPassSource{&reverbSource, false};
    
        double LPfrequency;
        juce::IIRCoefficients LPC;
        juce::IIRFilterAudioSource IIRLowPassSource{&IIRHighPassSource, false};
    
    

};
