/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ZacharyWP04FlangerChorusAudioProcessor::ZacharyWP04FlangerChorusAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(zWetDryMixParam = new juce::AudioParameterFloat("drywet",
                                                                      "Dry/Wet",
                                                                      0.0f,
                                                                      100.0f,
                                                                      50.0f));
    addParameter(zLFODepthParam = new juce::AudioParameterFloat("depth",
                                                                     "Depth",
                                                                     0.0f,
                                                                     24.0f,
                                                                     0.5f));
    addParameter(zLFOSpeedParam = new juce::AudioParameterFloat("rate",
                                                                     "Rate",
                                                                     0.1f,
                                                                     10.0f,
                                                                     0.5f));
    addParameter(zCenterFreqParam = new juce::AudioParameterFloat("phaseoffset",
                                                                     "Phase Offset",
                                                                     49.0f,
                                                                     81.0f,
                                                                     69.0f));
    addParameter(zFeedbackParameter = new juce::AudioParameterFloat("feedback",
                                                                        "Feedback",
                                                                        -20.0f,
                                                                        20.0f,
                                                                        0.0f));
    // Msec Delay Param
    addParameter(zDelayMsecParam = new juce::AudioParameterFloat(
                                                                 "Delay Msec", // paramID,
                                                                 "DelayMsec", // param name,
                                                                 5.0f, // min value,
                                                                 10000.0f, // max value,
                                                                 500.0f // default value,
                                                                       ));
        

}

ZacharyWP04FlangerChorusAudioProcessor::~ZacharyWP04FlangerChorusAudioProcessor()
{
}

//==============================================================================
const juce::String ZacharyWP04FlangerChorusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ZacharyWP04FlangerChorusAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ZacharyWP04FlangerChorusAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ZacharyWP04FlangerChorusAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ZacharyWP04FlangerChorusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ZacharyWP04FlangerChorusAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ZacharyWP04FlangerChorusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ZacharyWP04FlangerChorusAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ZacharyWP04FlangerChorusAudioProcessor::getProgramName (int index)
{
    return {};
}

void ZacharyWP04FlangerChorusAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ZacharyWP04FlangerChorusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    
    // Get system sampling rate
    zFs = sampleRate;
    
    zMinFc = 10.0;
    zMaxFc = zFs*0.5*0.95;
    
    // Initialize Delay objects w/ maximum length
    int maxSamps = calcMsecToSamps(10000);
    zDelay1L.setMaximumDelay(maxSamps);
    zDelay1R.setMaximumDelay(maxSamps);

}

void ZacharyWP04FlangerChorusAudioProcessor::calcAlgorithmParams()
{
    
    // Wet and Dry gains
    zWetGain = zWetDryMixParam->get()/100.0;
    zDryGain = 1.0 - zWetGain;
    
    // Set Delay times
    int samps = calcMsecToSamps(zDelayMsecParam->get());
    zDelay1L.setDelay(samps);
    zDelay1R.setDelay(samps);
    
    // set feedback gain
    zFeedbackGainLin = powf(10.0, zFeedbackParameter->get()*0.05);
    
    // set rate of LFO
    zLFO.setFreq(zLFOSpeedParam->get(), zFs/zControlN);
    
}

int ZacharyWP04FlangerChorusAudioProcessor::calcMsecToSamps(float msec)
{
    // msec = mDelayMsecParam->get();
    float numSamps = (msec / 1000) * zFs;
    return numSamps;
}

float ZacharyWP04FlangerChorusAudioProcessor::calcMidiToHz(float midiNote){
    return 440.0*powf(2, (midiNote-60.0)/12.0);
    
}

float ZacharyWP04FlangerChorusAudioProcessor::calcHzToMidi(float Hz){
    return 12*log2f(Hz/440.0)+60.0;
    
}
void ZacharyWP04FlangerChorusAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ZacharyWP04FlangerChorusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void ZacharyWP04FlangerChorusAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    float* channelDataLeft = buffer.getWritePointer(0);
    float* channelDataRight = buffer.getWritePointer(1);
    
    // update param values
    calcAlgorithmParams();
    float lfoOut, centerMidi, centerFreq;
    float coeffs[5];
    
    float varL, varR;
    
    const int numSamples = buffer.getNumSamples();
    
    for (int samps = 0; samps < numSamples; ++samps)
    {
        if (zControlCounter % zControlN == 0) {
            
            // update LFO and use output to calc center freq
            lfoOut = zLFO.tick();
            centerMidi = zCenterFreqParam->get() + zLFODepthParam->get() * lfoOut;
            centerFreq = calcMidiToHz(centerMidi);
            
            // ensure freq is not out of range
            centerFreq = (centerFreq < zMinFc) ? zMinFc : centerFreq;
            centerFreq = (centerFreq > zMaxFc) ? zMaxFc : centerFreq;
            
            // Calc & then set the filter coeffs
            Mu45FilterCalc::calcCoeffsBPF(coeffs, centerFreq, zFilterQ, zFs);
            zFilterL.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);
            zFilterR.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);
            
            zControlCounter = 0;
        }
        zControlCounter++;
        
        
        // filter audio
        channelDataLeft[samps] = zFilterL.tick(channelDataLeft[samps]);
        channelDataRight[samps] = zFilterR.tick(channelDataRight[samps]);
        

    
        // get next out
        varL = zDelay1L.nextOut();
        varR = zDelay1R.nextOut();
        
        // compute next input to filter with feedback and tick
        zDelay1L.tick(zWetGain*channelDataLeft[samps] + zFeedbackGainLin);
        zDelay1R.tick(zWetGain*channelDataRight[samps] + zFeedbackGainLin);
        
        // mix wet and dry and send out
        channelDataLeft[samps] = zDryGain * channelDataLeft[samps] + zWetGain * varL;
        channelDataRight[samps] = zDryGain * channelDataRight[samps] + zWetGain * varR;
        //}
    }
}

//==============================================================================
bool ZacharyWP04FlangerChorusAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ZacharyWP04FlangerChorusAudioProcessor::createEditor()
{
    return new ZacharyWP04FlangerChorusAudioProcessorEditor (*this);
}

//==============================================================================
void ZacharyWP04FlangerChorusAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ZacharyWP04FlangerChorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ZacharyWP04FlangerChorusAudioProcessor();
}
