/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class DelayAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DelayAudioProcessor();
    ~DelayAudioProcessor();
    float currentGain; //gain value


    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //PARAMETERS
    int getNumParameters() override;
    float getParameter (int index) override;
    void setParameter (int index, float newValue) override; const String getParameterName (int index) override;
    const String getParameterText (int index) override;
    
    //DECLARING PARAMETERS
    enum Parameters
    {
        kDelayLengthParam = 0,
        kDryMixParam,
        kWetMixParam,
        kFeedbackParam,
        kGain,
        kNoise,
        kBit,
        kRate,
        kCutOff,
        kNumParameters
    };
   
    // ADJUSTABLE PARAMETERS
    float delayLength_;
    float dryMix_;
    float wetMix_;
    float feedback_;
    float gain_;
    float noiseAmount;
    float bit_;
    float rate_;
    float cutoff_;


    
private:
    // Circular buffer variables for implementing delay, noise, bit & rate reduction and the highpass filter
    AudioSampleBuffer delayBuffer_, noiseBuffer, currentOutputBuffer, highLBuffer, highRBuffer;
    int delayBufferLength_;
    int delayReadPosition_, delayWritePosition_;
    
    //declaring highpass filters. Since the effect is in stereo, there are two highpass filters(normally in mono) to implement stereo
    dsp::IIR::Filter<float> highpassL;
    dsp::IIR::Filter<float> highpassR;

    float samplerate;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessor)
};
