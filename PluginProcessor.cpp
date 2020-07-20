/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//OPEN SOURCE CODE GENERATING NOISE
// https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform

static Array<float> getSimpleNoise(int numSamples)
{
    Random r = Random::getSystemRandom();
    Array<float> noise;
    
    for (int s=0; s < numSamples; s++)
    {
        noise.add((r.nextFloat() - .5)*2);
    }
    
    return noise;
    
}

static Array<float> getWhiteNoise(int numSamples) {
    
    Array<float> noise;
    
    float z0 = 0;
    float z1 = 0;
    bool generate = false;
    
    float mu = 0; // center (0)
    float sigma = 1; // spread -1 <-> 1
    
    float output = 0;
    float u1 = 0;
    float u2 = 0;
    
    Random r = Random::getSystemRandom();
    r.setSeed(Time::getCurrentTime().getMilliseconds());
    const float epsilon = std::numeric_limits<float>::min();
    
    for (int s=0; s < numSamples; s++)
    {
        
        generate = !generate;
        
        if (!generate)
            output =  z1 * sigma + mu;
        else
        {
            do
            {
                u1 = r.nextFloat();
                u2 = r.nextFloat();
            }
            while ( u1 <= epsilon );
            
            z0 = sqrtf(-2.0 * logf(u1)) * cosf(2*float(double_Pi) * u2);
            z1 = sqrtf(-2.0 * logf(u1)) * sinf(2*float(double_Pi) * u2);
            
            output = z0 * sigma + mu;
        }
        
        // NAN check ...
        jassert(output == output);
        jassert(output > -50 && output < 50);
        
        //
        noise.add(output);
        
    }
    
    return noise;
    
}
//==============================================================================
DelayAudioProcessor::DelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
delayBuffer_ (2,1)
#endif
{
    // Set default values
    noiseAmount = 0;
    bit_ = 32;
    rate_= 1;
    delayLength_ = 0.01;
    dryMix_ = 1.0;
    wetMix_ = 0.5;
    feedback_ = 0.75;
    delayBufferLength_ = 1;
    cutoff_ = 20;

    // Start the circular buffer pointers at the beginning
    delayReadPosition_ = 0;
    delayWritePosition_ = 0;
}

DelayAudioProcessor::~DelayAudioProcessor()
{
}

//==============================================================================
const String DelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayAudioProcessor::setCurrentProgram (int index)
{
}

const String DelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //preparing the Delay implementation
        delayBufferLength_ = (int)(2.0*sampleRate); if(delayBufferLength_ < 1)
        delayBufferLength_ = 1; delayBuffer_.setSize(2, delayBufferLength_); delayBuffer_.clear();

        delayReadPosition_ = (int)(delayWritePosition_ - (delayLength_ * getSampleRate()) + delayBufferLength_) % delayBufferLength_;
    
    //preparing the high-pass filter implementation for stereo
    dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels =2;
    spec.sampleRate = sampleRate;
    highLBuffer.setSize(2,samplesPerBlock);//left channel
    highpassL.prepare(spec);
    
    highRBuffer.setSize(2,samplesPerBlock);//right channel
    highpassR.prepare(spec);
    
    samplerate = sampleRate;


}

int DelayAudioProcessor::getNumParameters() {
    return kNumParameters;
}

float DelayAudioProcessor::getParameter (int index) {
 
    switch (index)
    {
        case kDryMixParam: return dryMix_;
        case kWetMixParam: return wetMix_;
        case kFeedbackParam: return feedback_;
        case kDelayLengthParam: return delayLength_;
        case kNoise: return noiseAmount;
        case kBit: return bit_;
        case kRate: return rate_;
        case kCutOff: return cutoff_;


        default: return 0.0f;
    }
}

const String DelayAudioProcessor::getParameterName (int index) {
    switch (index)
    {
        case kDryMixParam:
        case kWetMixParam:
        case kFeedbackParam:
            return "dry mix";
            return "wet mix";
            return "feedback";
        case kDelayLengthParam:return "delay";
        case kNoise:return "noise";
        case kBit:return "bit";
        case kRate:return "Rate";
        case kCutOff: return "Cut off Frequency";


        default: break; }
    
    return String::empty;
}

const String DelayAudioProcessor::getParameterText (int index)
{
    return String (getParameter (index), 2); }

void DelayAudioProcessor::setParameter (int index, float newValue) {
  
    switch (index)
    {
        case kDryMixParam:
            dryMix_ = newValue;
            break;
        case kWetMixParam:
            wetMix_ = newValue;
            break;
        case kFeedbackParam:
            feedback_ = newValue;
            break;
        case kDelayLengthParam:
            delayLength_ = newValue;
            delayReadPosition_ = (int)(delayWritePosition_ - (delayLength_ * getSampleRate())  + delayBufferLength_) % delayBufferLength_;
                                       break;
        case kNoise:
            noiseAmount = newValue;
            break;
        case kBit:
            bit_ = newValue;
            break;
        case kRate:
            rate_ = newValue;
            break;
        case kCutOff:
            cutoff_ = newValue;
            break;
            
        default:
            break; }
                                       }
                                     

void DelayAudioProcessor::releaseResources()
{
   
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    const int numSamples = buffer.getNumSamples(); // How many samples in the buffer
   
//-------------------------------------------------------------------------------------------
   // BIT CRUSHER IMPLEMENTATION - OPEN SOURCE
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    float noiseAmt = -120 + 120*(noiseAmount/100); // dB
    float bitDepth = bit_;
    int rateDivide = rate_;
    
    
    
    // GET PARAMETERS
    noiseAmt = jlimit<float>(-120, 0, noiseAmt);
    noiseAmt = Decibels::decibelsToGain(noiseAmt);
    
    
    
    // SAFETY CHECK
    if (noiseBuffer.getNumSamples() != numSamples)
    {
        noiseBuffer.setSize(2, numSamples, false, true, true); // clears
        currentOutputBuffer.setSize(2, numSamples, false, true, true); // clears
    }
    
    
    // COPY for processing
    currentOutputBuffer.copyFrom(0, 0, buffer.getReadPointer(0), numSamples);
    if (buffer.getNumChannels() > 1) currentOutputBuffer.copyFrom(1, 0, buffer.getReadPointer(1), numSamples);
    
    
    
    // BUILD NOISE
    {
        noiseBuffer.clear();
        
        Array<float> noise = getWhiteNoise(numSamples);
        
        // range bound
        noiseAmt = jlimit<float>(0, 1, noiseAmt);
        
        FloatVectorOperations::multiply(noise.getRawDataPointer(), noiseAmt, numSamples);
        
        // ADD the noise ...
        FloatVectorOperations::add(noiseBuffer.getWritePointer(0), noise.getRawDataPointer(), numSamples);
        FloatVectorOperations::add(noiseBuffer.getWritePointer(1), noise.getRawDataPointer(), numSamples); // STEREO
        
    }
    
    
    // ADD NOISE to the incoming AUDIO
    currentOutputBuffer.addFrom(0, 0, noiseBuffer.getReadPointer(0), numSamples);
    currentOutputBuffer.addFrom(1, 0, noiseBuffer.getReadPointer(1), numSamples);
    
    
    
    // RESAMPLE AS NEEDED
    for (int chan=0; chan < currentOutputBuffer.getNumChannels(); chan++)
    {
        float* data = currentOutputBuffer.getWritePointer(chan);
        
        for (int i=0; i < numSamples; i++)
        {
            // REDUCE BIT DEPTH :::::
            float totalQLevels = powf(2, bitDepth);
            float val = data[i];
            float remainder = fmodf(val, 1/totalQLevels);
            
            // Quantize ...
            data[i] = val - remainder;
            
            if (rateDivide > 1)
            {
                if (i%rateDivide != 0) data[i] = data[i - i%rateDivide];
            }
        }
    }
    // COPY to the actual output buffer :::
    buffer.copyFrom(0, 0, currentOutputBuffer, 0, 0, numSamples);
    buffer.copyFrom(1, 0, currentOutputBuffer, 1, 0, numSamples);

//END OF BIT CRUSHER IMPLEMENTATION
//-------------------------------------------------------------------------------------------
//GAIN AND DELAY IMPLEMENTATION - BASED ON PREVIOUS LABS
    int  dpr, dpw; // dpr = delay read pointer; dpw = delay write pointer


    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
       
        // Go through each channel and apply the gain
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            
            const float in = channelData[i];
            // Store the output sample back in the buffer
            channelData[i] = in * pow(10, currentGain / 20);    }
        
 
        float* delayData = delayBuffer_.getWritePointer (jmin (channel, delayBuffer_.getNumChannels() - 1));
      
        dpr = delayReadPosition_;
        dpw = delayWritePosition_;
        for (int i = 0; i < numSamples; ++i) {
            const float in = channelData[i]; float out = 0.0;

            out = (dryMix_ * in + wetMix_ * delayData[dpr]);
         
            delayData[dpw] = in + (delayData[dpr] * feedback_);
            if (++dpr >= delayBufferLength_)
                dpr = 0;
            if (++dpw >= delayBufferLength_)
                dpw = 0;
            // Store the output sample in the buffer, replacing the input
            channelData[i] = out;
        } }
    delayReadPosition_ = dpr;
    delayWritePosition_ = dpw;
//END OF GAIN AND DELAY IMPLEMENTATION
//-------------------------------------------------------------------------------------------
//HIGH PASS FILTERS IMPLEMENTATION
    
    highLBuffer.makeCopyOf(buffer);
    highpassL.coefficients = dsp::IIR::Coefficients<float>::makeHighPass(samplerate, cutoff_);
    
    highRBuffer.makeCopyOf(buffer);
    highpassR.coefficients = dsp::IIR::Coefficients<float>::makeHighPass(samplerate, cutoff_);

    dsp::AudioBlock<float> blockH(highLBuffer);//left
    dsp::ProcessContextReplacing<float> HPcontext(blockH);
    highpassL.process(HPcontext);

    dsp::AudioBlock<float> blockH2(highRBuffer);//right
    dsp::ProcessContextReplacing<float> HP2context(blockH);
    highpassR.process(HP2context);
   
    const auto left = blockH.getSingleChannelBlock(0);
    auto right = blockH2.getSingleChannelBlock(1);
    right.copy (left);//copy to the second channel

    buffer.addFrom(0,0, highLBuffer, 0, 0, numSamples);
    buffer.addFrom(1,0, highRBuffer, 1, 0, numSamples);
//END OF HIGH PASS FILTERS
    }


//==============================================================================
bool DelayAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* DelayAudioProcessor::createEditor()
{
    return new DelayAudioProcessorEditor (*this);
}

//==============================================================================
void DelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    
}

void DelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
 
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayAudioProcessor();
}
