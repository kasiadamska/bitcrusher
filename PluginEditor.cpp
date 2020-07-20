/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessorEditor::DelayAudioProcessorEditor (DelayAudioProcessor& p)
    : AudioProcessorEditor (&p),
    delayLengthLabel_("", " Delay:"),
    feedbackLabel_("", " Feedback:"),
    dryMixLabel_("", " Dry level:"),
    wetMixLabel_("", " Wet level:"),
    gainLabel_("","  Gain"),
    noiseLabel_("","  Noise"),
    bitLabel_("","  Bit"),
    rateLabel_("","  Rate"),
    cutoffLabel_("", "High-Pass Filter"),
    processor (p)
{
  
//Setting window size
    
    setSize (600, 600);
    
    
    // DELAY SLIDERS
    addAndMakeVisible (&delayLengthSlider_);
    delayLengthSlider_.setSliderStyle (Slider::Rotary);
    delayLengthSlider_.addListener (this);
    delayLengthSlider_.setTextValueSuffix(" s");
    delayLengthSlider_.setRange (0.01, 2.0, 0.01);
    delayLengthSlider_.setValue(processor.getParameter(DelayAudioProcessor::kDelayLengthParam));
    
    addAndMakeVisible (&feedbackSlider_);
    feedbackSlider_.setSliderStyle (Slider::Rotary);
    feedbackSlider_.addListener (this);
    feedbackSlider_.setRange (0.0, 0.995, 0.005);
    feedbackSlider_.setValue(processor.getParameter(DelayAudioProcessor::kFeedbackParam));
    
    addAndMakeVisible (&dryMixSlider_);
    dryMixSlider_.setSliderStyle (Slider::Rotary);
    dryMixSlider_.addListener (this);
    dryMixSlider_.setRange (0.0, 1.0, 0.01);
    dryMixSlider_.setValue(processor.getParameter(DelayAudioProcessor::kDryMixParam));
    
    addAndMakeVisible (&wetMixSlider_);
    wetMixSlider_.setSliderStyle (Slider::Rotary);
    wetMixSlider_.addListener (this);
    wetMixSlider_.setRange (0.0, 1.0, 0.01);
    wetMixSlider_.setValue(processor.getParameter(DelayAudioProcessor::kWetMixParam));
    
    
    //GAIN SLIDER
    addAndMakeVisible(&gainSlider_);
    gainSlider_.setSliderStyle(Slider::LinearVertical);
    gainSlider_.setRange(-35.00, 35.00, 0.01);
    gainSlider_.setColour(Slider::textBoxTextColourId, Colours::white);
    gainSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 200, 20);
    gainSlider_.setTextValueSuffix(" dB");
    gainSlider_.setValue(0.00);
    gainSlider_.addListener(this);
    
    //BIT CRUSHER SLIDERS
    noiseSlider_.setRange(0.0, 100.0,0.01);
    noiseSlider_.setValue(processor.getParameter(DelayAudioProcessor::kNoise));
    noiseSlider_.setSliderStyle(Slider::LinearVertical);
    noiseSlider_.setColour(Slider::textBoxTextColourId, Colours::white);
    noiseSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 200, 20);
    noiseSlider_.setValue(0);
    noiseSlider_.addListener(this);
    addAndMakeVisible(&noiseSlider_);
    
    bitSlider_.setRange(2.0, 32.0, 0.01);
    bitSlider_.setValue(processor.getParameter(DelayAudioProcessor::kBit));
    bitSlider_.setSliderStyle(Slider::LinearVertical);
    bitSlider_.setColour(Slider::textBoxTextColourId, Colours::white);
    bitSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 200, 20);
    bitSlider_.setValue(32);
    bitSlider_.addListener(this);
    addAndMakeVisible(&bitSlider_);
    
    rateSlider_.setRange(1.0, 50.0, 0.01);
    rateSlider_.setValue(processor.getParameter(DelayAudioProcessor::kRate));
    rateSlider_.setSliderStyle(Slider::LinearVertical);
    rateSlider_.setColour(Slider::textBoxTextColourId, Colours::white);
    rateSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 200, 20);
    rateSlider_.setValue(1);
    rateSlider_.addListener(this);
    addAndMakeVisible(&rateSlider_);
    
    //HIGH-PASS FILTER SLIDER
    addAndMakeVisible (&cutoffSlider_);
    cutoffSlider_.setSliderStyle(Slider::LinearHorizontal);
    cutoffSlider_.addListener (this);
    cutoffSlider_.setRange(20, 20000, 1);
    cutoffSlider_.setTextValueSuffix(" Hz");
    cutoffSlider_.setValue(20.00);
    

    //Attaching labels to sliders
    //true = next
    delayLengthLabel_.attachToComponent(&delayLengthSlider_, true);
    feedbackLabel_.attachToComponent(&feedbackSlider_, true);
    dryMixLabel_.attachToComponent(&dryMixSlider_, true);
    wetMixLabel_.attachToComponent(&wetMixSlider_, true);
    
    //false = above
    gainLabel_.attachToComponent(&gainSlider_, false);
    noiseLabel_.attachToComponent(&noiseSlider_, false);
    bitLabel_.attachToComponent(&bitSlider_, false);
    rateLabel_.attachToComponent(&rateSlider_, false);
    cutoffLabel_.attachToComponent(&cutoffSlider_, true);

}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
}


void DelayAudioProcessorEditor::timerCallback()
{
    delayLengthSlider_.setValue(processor.delayLength_, dontSendNotification);
    feedbackSlider_.setValue(processor.feedback_, dontSendNotification);
    dryMixSlider_.setValue(processor.dryMix_, dontSendNotification);
    wetMixSlider_.setValue(processor.wetMix_, dontSendNotification);
    
    noiseSlider_.setValue(processor.noiseAmount, dontSendNotification);
    bitSlider_.setValue(processor.bit_, dontSendNotification);
    rateSlider_.setValue(processor.rate_, dontSendNotification);
    cutoffSlider_.setValue(processor.cutoff_, dontSendNotification);


}

void DelayAudioProcessorEditor::sliderValueChanged (Slider* slider) {


    if (slider == &delayLengthSlider_) {
        processor.setParameterNotifyingHost (DelayAudioProcessor::kDelayLengthParam, (float)delayLengthSlider_.getValue());
    }
    else if (slider == &feedbackSlider_) {
        processor.setParameterNotifyingHost (DelayAudioProcessor::kFeedbackParam, (float)feedbackSlider_.getValue());
    }
    else if (slider == &dryMixSlider_) {
        processor.setParameterNotifyingHost (DelayAudioProcessor::kDryMixParam, (float)dryMixSlider_.getValue());
    }
    else if (slider == &wetMixSlider_) {
        processor.setParameterNotifyingHost (DelayAudioProcessor::kWetMixParam, (float)wetMixSlider_.getValue());
    }
    else if (slider == &gainSlider_) {
        processor.currentGain = gainSlider_.getValue();
    }
    else if (slider == &noiseSlider_) {
        processor.setParameterNotifyingHost (DelayAudioProcessor::kNoise, (float)noiseSlider_.getValue());
    }
    else if (slider == &bitSlider_) {
        processor.setParameterNotifyingHost (DelayAudioProcessor::kBit, (float)bitSlider_.getValue());
    }
    else if (slider == &rateSlider_) {
        processor.setParameterNotifyingHost (DelayAudioProcessor::kRate, (float)rateSlider_.getValue());
    }
    else  if (slider == &cutoffSlider_) {
        processor.setParameterNotifyingHost (DelayAudioProcessor::kCutOff, (float)cutoffSlider_.getValue());
    }
}

//==============================================================================
void DelayAudioProcessorEditor::paint (Graphics& g)
{
    //background colour
    g.fillAll (Colours::black);

    //Title text
    g.setColour(Colours::white);
    g.setFont (Font ("Helvetica", 25.0f, Font::bold));
    g.drawText ("BIT CRUSHER", 0, 30, 600, 40, Justification::centred, true);
    
    g.setFont (10.0f);
    g.drawText ("Assignment 2 - Katarzyna Adamska", 0, 0, 600, 40, Justification::right, true);
    
    //Customising sliders - colours
    
    noiseSlider_.setColour (Slider::thumbColourId, Colours::red);
    noiseSlider_.setColour (Slider::backgroundColourId, Colours::lightsalmon);
    noiseSlider_.setColour (Slider::trackColourId, Colours::red);

    bitSlider_.setColour (Slider::thumbColourId, Colours::red);
    bitSlider_.setColour (Slider::backgroundColourId, Colours::lightsalmon);
    bitSlider_.setColour (Slider::trackColourId, Colours::red);

    rateSlider_.setColour (Slider::thumbColourId, Colours::red);
    rateSlider_.setColour (Slider::backgroundColourId, Colours::lightsalmon);
    rateSlider_.setColour (Slider::trackColourId, Colours::red);

    gainSlider_.setColour (Slider::thumbColourId, Colours::yellow);
    gainSlider_.setColour (Slider::backgroundColourId, Colours::lightyellow);
    gainSlider_.setColour (Slider::trackColourId, Colours::yellow);
    
    cutoffSlider_.setColour (Slider::thumbColourId, Colours::greenyellow);
    cutoffSlider_.setColour (Slider::backgroundColourId, Colours::lightgreen);
    cutoffSlider_.setColour (Slider::trackColourId, Colours::greenyellow);
    
    delayLengthSlider_.setColour (Slider::thumbColourId, Colours::blue);
    delayLengthSlider_.setColour (Slider::rotarySliderFillColourId, Colours::blue);
    delayLengthSlider_.setColour (Slider::rotarySliderOutlineColourId, Colours::lightblue);

    feedbackSlider_.setColour (Slider::thumbColourId, Colours::blue);
    feedbackSlider_.setColour (Slider::rotarySliderFillColourId, Colours::blue);
    feedbackSlider_.setColour (Slider::rotarySliderOutlineColourId, Colours::lightblue);

    dryMixSlider_.setColour (Slider::thumbColourId, Colours::blue);
    dryMixSlider_.setColour (Slider::rotarySliderFillColourId, Colours::blue);
    dryMixSlider_.setColour (Slider::rotarySliderOutlineColourId, Colours::lightblue);

    wetMixSlider_.setColour (Slider::thumbColourId, Colours::blue);
    wetMixSlider_.setColour (Slider::rotarySliderFillColourId, Colours::blue);
    wetMixSlider_.setColour (Slider::rotarySliderOutlineColourId, Colours::lightblue);

}

void DelayAudioProcessorEditor::resized()
{
  //positioning the sliders 
    delayLengthSlider_.setBounds (100, 380, 150, 75);
    feedbackSlider_.setBounds (400, 380, 150, 75);
    dryMixSlider_.setBounds(100, 500, 150, 75);
    wetMixSlider_.setBounds(400, 500, 150, 75);

    gainSlider_.setBounds(500, 100, 50, 200);

    
    noiseSlider_.setBounds(50, 100, 50, 200);
    bitSlider_.setBounds(200, 100, 50, 200);
    rateSlider_.setBounds(350, 100, 50, 200);
    
    cutoffSlider_.setBounds(190,310,300,75);
    
    
}
