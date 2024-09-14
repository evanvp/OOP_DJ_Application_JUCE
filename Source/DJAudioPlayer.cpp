/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 17 Jan 2024 7:19:54pm
    Author:  Vincent UofL

  ==============================================================================
*/

#include "DJAudioPlayer.h"


DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager)
{
    for (int i=0;i<formatManager.getNumKnownFormats(); i++){
        std::string s = formatManager.getKnownFormat(i)->getFormatName().toStdString();
        std::cout << i << " " << s << std::endl;
    }
    
    
//initilize the reverbParameters (only roomSize value can be changed by a slider)
    reverbParameters.roomSize = 0.5f;
    reverbParameters.wetLevel = 0.33f;
    reverbParameters.dryLevel = 0.4f;
    reverbSource.setParameters(reverbParameters);
//initialize a samplerate for high/low pass filter, value  will update when an audio file loaded
    filterSamplerate = 44200;
    
    

    
}

DJAudioPlayer::~DJAudioPlayer()
{
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource
        (new juce::AudioFormatReaderSource (reader, true));
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset (newSource.release());
        
        filterSamplerate = reader->sampleRate;
        HPfrequency = 20;
        HPC = juce::IIRCoefficients::makeHighPass(filterSamplerate, HPfrequency);
        IIRHighPassSource.setCoefficients(HPC);
        
        LPfrequency = 20000;
        LPC = juce::IIRCoefficients::makeLowPass(filterSamplerate, LPfrequency);
        IIRLowPassSource.setCoefficients(LPC);
        
        
    }
    
    std::cout << "DJAudioPlayer::loadURL : Loaded" << std::endl;
}


void DJAudioPlayer::play()
{
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}


void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else {
        transportSource.setGain(gain);
    }
}


void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 10.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setspace(double spaceratio)
{
    if (spaceratio < 0 || spaceratio > 1.0)
    {
        std::cout << "DJAudioPlayer::setspace ratio should be between 0 and 1" << std::endl;
    }
    else {
        reverbParameters.roomSize = spaceratio;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::setHP(double frequency)
{
    if (frequency < 20 || frequency > 20000)
    {
        std::cout << "DJAudioPlayer::setHP ratio should be between 20 and 20000" << std::endl;
    }
    else {
        HPfrequency = frequency;
        HPC = juce::IIRCoefficients::makeHighPass(filterSamplerate, HPfrequency);
        IIRHighPassSource.setCoefficients(HPC);
    }
}

void DJAudioPlayer::setLP(double frequency)
{
    if (frequency < 20 || frequency > 20000)
    {
        std::cout << "DJAudioPlayer::setLP ratio should be between 20 and 20000" << std::endl;
    }
    else {
        LPfrequency = frequency;
        LPC = juce::IIRCoefficients::makeLowPass(filterSamplerate, LPfrequency);
        IIRLowPassSource.setCoefficients(LPC);
    }
}



void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    IIRHighPassSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    IIRLowPassSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
  
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    
    //transportSource.getNextAudioBlock(bufferToFill);
    resampleSource.getNextAudioBlock(bufferToFill);
    reverbSource.getNextAudioBlock(bufferToFill);
    IIRHighPassSource.getNextAudioBlock(bufferToFill);
    IIRLowPassSource.getNextAudioBlock(bufferToFill);
    
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
    reverbSource.releaseResources();
    IIRHighPassSource.releaseResources();
    IIRLowPassSource.releaseResources();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}


