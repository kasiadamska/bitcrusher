/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DelayAudioProcessorEditor  : public AudioProcessorEditor, public Slider:: Listener, public Timer
{
public:
    DelayAudioProcessorEditor (DelayAudioProcessor&);
    ~DelayAudioProcessorEditor();
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (Slider*) override;
    void timerCallback() override;
    
    //Declaring Labels and Sliders
private:
    
    Label delayLengthLabel_, feedbackLabel_, dryMixLabel_, wetMixLabel_, gainLabel_, noiseLabel_, bitLabel_, rateLabel_, cutoffLabel_;
    Slider delayLengthSlider_, feedbackSlider_, dryMixSlider_, wetMixSlider_, gainSlider_, noiseSlider_, bitSlider_, rateSlider_, cutoffSlider_;
    
   
    DelayAudioProcessor& processor;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessorEditor)
};
