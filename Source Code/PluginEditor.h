/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ZacharyWP04FlangerChorusAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ZacharyWP04FlangerChorusAudioProcessorEditor (ZacharyWP04FlangerChorusAudioProcessor&);
    ~ZacharyWP04FlangerChorusAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ZacharyWP04FlangerChorusAudioProcessor& audioProcessor;
    
    juce::Slider zDryWetSlider;
    juce::Label zDryWetLabel;
    juce::Slider zDepthSlider;
    juce::Label zDepthLabel;
    juce::Slider zRateSlider;
    juce::Label zRateLabel;
    juce::Slider zPhaseOffsetSlider;
    juce::Label zPhaseOffsetLabel;
    juce::Slider zFeedbackSlider;
    juce::Label zFeedbackLabel;
    juce::Slider zDelaySlider;
    juce::Label zDelayLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZacharyWP04FlangerChorusAudioProcessorEditor)
};
