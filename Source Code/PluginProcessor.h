/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StkLite-4.6.2/Delay.h"
#include "StkLite-4.6.2/BiQuad.h"
#include "Mu45FilterCalc/Mu45FilterCalc.h"
#include "Mu45LFO/Mu45LFO.h"


//==============================================================================
/**
*/
class ZacharyWP04FlangerChorusAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    ZacharyWP04FlangerChorusAudioProcessor();
    ~ZacharyWP04FlangerChorusAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    
    // Functions created:
    void calcAlgorithmParams();
    int calcMsecToSamps(float msec);
    float calcHzToMidi(float Hz);
    float calcMidiToHz(float midiNote);
    
    Mu45LFO zLFO;
    stk::BiQuad zFilterL, zFilterR;
        
    juce::AudioParameterFloat* zWetDryMixParam;
    juce::AudioParameterFloat* zFeedbackParameter;
    juce::AudioParameterFloat* zDelayMsecParam;
    juce::AudioParameterFloat* zCenterFreqParam;
    juce::AudioParameterFloat* zLFODepthParam;
    juce::AudioParameterFloat* zLFOSpeedParam;
    
    
        
    float zWetGain, zDryGain;
    float zFeedbackGainLin;
    float zFilterQ = 2;
    float zMinFc;
    float zMaxFc;
    
    long zControlCounter = 0; // The control rate counter
    int zControlN = 100; // How often cntrl rate variables get updated
        
    stk::Delay zDelay1L, zDelay1R;
    float zFs;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZacharyWP04FlangerChorusAudioProcessor)
};
