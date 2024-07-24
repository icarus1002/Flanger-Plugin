/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ZacharyWP04FlangerChorusAudioProcessorEditor::ZacharyWP04FlangerChorusAudioProcessorEditor (ZacharyWP04FlangerChorusAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    auto& params = processor.getParameters();
        
    juce::AudioParameterFloat* dryWetParameter = (juce::AudioParameterFloat*)params.getUnchecked(0);
        
    zDryWetSlider.setBounds(0, 50, 100, 100);
    zDryWetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    zDryWetSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 25);
    zDryWetSlider.setColour(zDryWetSlider.textBoxTextColourId, juce::Colours::black);
    zDryWetSlider.setRange(dryWetParameter->range.start, dryWetParameter->range.end, 0.01);
    zDryWetSlider.setValue(*dryWetParameter);
    addAndMakeVisible(zDryWetSlider);
        
    zDryWetLabel.setText("Dry/Wet", juce::dontSendNotification);
    zDryWetLabel.attachToComponent(&zDryWetSlider, false);
    zDryWetLabel.setJustificationType(juce::Justification::centredTop);
    zDryWetLabel.setColour(zDryWetLabel.textColourId, juce::Colours::black);
    addAndMakeVisible(zDryWetLabel);
        
    zDryWetSlider.onValueChange = [this, dryWetParameter] { *dryWetParameter = zDryWetSlider.getValue();};
    zDryWetSlider.onDragStart = [dryWetParameter] { dryWetParameter->beginChangeGesture(); };
    zDryWetSlider.onDragEnd = [dryWetParameter] { dryWetParameter->endChangeGesture(); };
        
    juce::AudioParameterFloat* depthParameter = (juce::AudioParameterFloat*)params.getUnchecked(1);
        
    zDepthSlider.setBounds(0, 200, 100, 100);
    zDepthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    zDepthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 25);
    zDepthSlider.setColour(zDepthSlider.textBoxTextColourId, juce::Colours::black);
    zDepthSlider.setRange(depthParameter->range.start, depthParameter->range.end, 0.01);
    zDepthSlider.setValue(*depthParameter);
    addAndMakeVisible(zDepthSlider);
        
    zDepthLabel.setText("Depth", juce::dontSendNotification);
    zDepthLabel.attachToComponent(&zDepthSlider, false);
    zDepthLabel.setJustificationType(juce::Justification::centredTop);
    zDepthLabel.setColour(zDepthLabel.textColourId, juce::Colours::black);
    addAndMakeVisible(zDepthLabel);
        
    zDepthSlider.onValueChange = [this, depthParameter] { *depthParameter = zDepthSlider.getValue();};
    zDepthSlider.onDragStart = [depthParameter] { depthParameter->beginChangeGesture(); };
    zDepthSlider.onDragEnd = [depthParameter] { depthParameter->endChangeGesture(); };
        
    juce::AudioParameterFloat* rateParameter = (juce::AudioParameterFloat*)params.getUnchecked(2);
        
    zRateSlider.setBounds(300, 200, 100, 100);
    zRateSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    zRateSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 25);
    zRateSlider.setColour(zRateSlider.textBoxTextColourId, juce::Colours::black);
    zRateSlider.setRange(rateParameter->range.start, rateParameter->range.end, 0.01);
    zRateSlider.setValue(*rateParameter);
    addAndMakeVisible(zRateSlider);
        
    zRateLabel.setText("Rate", juce::dontSendNotification);
    zRateLabel.attachToComponent(&zRateSlider, false);
    zRateLabel.setJustificationType(juce::Justification::centredTop);
    zRateLabel.setColour(zRateLabel.textColourId, juce::Colours::black);
    addAndMakeVisible(zRateLabel);
        
    zRateSlider.onValueChange = [this, rateParameter] { *rateParameter = zRateSlider.getValue();};
    zRateSlider.onDragStart = [rateParameter] { rateParameter->beginChangeGesture(); };
    zRateSlider.onDragEnd = [rateParameter] { rateParameter->endChangeGesture(); };
        
    juce::AudioParameterFloat* phaseParameter = (juce::AudioParameterFloat*)params.getUnchecked(3);
        
    zPhaseOffsetSlider.setBounds(150, 50, 100, 100);
    zPhaseOffsetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    zPhaseOffsetSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 25);
    zPhaseOffsetSlider.setColour(zPhaseOffsetSlider.textBoxTextColourId, juce::Colours::black);
    zPhaseOffsetSlider.setRange(phaseParameter->range.start, phaseParameter->range.end);
    zPhaseOffsetSlider.setValue(*phaseParameter);
    addAndMakeVisible(zPhaseOffsetSlider);
    
    zPhaseOffsetLabel.setText("Phase", juce::dontSendNotification);
    zPhaseOffsetLabel.attachToComponent(&zPhaseOffsetSlider, false);
    zPhaseOffsetLabel.setJustificationType(juce::Justification::centredTop);
    zPhaseOffsetLabel.setColour(zPhaseOffsetLabel.textColourId, juce::Colours::black);
    addAndMakeVisible(zPhaseOffsetLabel);
        
    zPhaseOffsetSlider.onValueChange = [this, phaseParameter] { *phaseParameter = zPhaseOffsetSlider.getValue();};
    zPhaseOffsetSlider.onDragStart = [phaseParameter] { phaseParameter->beginChangeGesture(); };
    zPhaseOffsetSlider.onDragEnd = [phaseParameter] { phaseParameter->endChangeGesture(); };
        
    juce::AudioParameterFloat* feedbackParameter = (juce::AudioParameterFloat*)params.getUnchecked(4);
        
    zFeedbackSlider.setBounds(300, 50, 100, 100);
    zFeedbackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    zFeedbackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 25);
    zFeedbackSlider.setColour(zFeedbackSlider.textBoxTextColourId, juce::Colours::black);
    zFeedbackSlider.setRange(feedbackParameter->range.start, feedbackParameter->range.end, 0.01);
    zFeedbackSlider.setValue(*feedbackParameter);
    addAndMakeVisible(zFeedbackSlider);
        
    zFeedbackLabel.setText("Feedback", juce::dontSendNotification);
    zFeedbackLabel.attachToComponent(&zFeedbackSlider, false);
    zFeedbackLabel.setJustificationType(juce::Justification::centredTop);
    zFeedbackLabel.setColour(zFeedbackLabel.textColourId, juce::Colours::black);
    addAndMakeVisible(zFeedbackLabel);
        
    zFeedbackSlider.onValueChange = [this, feedbackParameter] { *feedbackParameter = zFeedbackSlider.getValue(); };
    zFeedbackSlider.onDragStart = [feedbackParameter] { feedbackParameter->beginChangeGesture(); };
    zFeedbackSlider.onDragEnd = [feedbackParameter] { feedbackParameter->endChangeGesture(); };
        
    juce::AudioParameterFloat* delayParameter = (juce::AudioParameterFloat*)params.getUnchecked(2);
    
    zDelaySlider.setBounds(150, 200, 100, 100);
    zDelaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    zDelaySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 25);
    zDelaySlider.setColour(zDelaySlider.textBoxTextColourId, juce::Colours::black);
    zDelaySlider.setRange(delayParameter->range.start, delayParameter->range.end, 0.01);
    zDelaySlider.setValue(*delayParameter);
    addAndMakeVisible(zDelaySlider);
        
    zDelayLabel.setText("Delay", juce::dontSendNotification);
    zDelayLabel.attachToComponent(&zDelaySlider, false);
    zDelayLabel.setJustificationType(juce::Justification::centredTop);
    zDelayLabel.setColour(zDelayLabel.textColourId, juce::Colours::black);
    addAndMakeVisible(zDelayLabel);
        
    zDelaySlider.onValueChange = [this, delayParameter] { *delayParameter = zDelaySlider.getValue(); };
    zDelaySlider.onDragStart = [delayParameter] { delayParameter->beginChangeGesture(); };
    zDelaySlider.onDragEnd = [delayParameter] { delayParameter->endChangeGesture(); };

        
}

ZacharyWP04FlangerChorusAudioProcessorEditor::~ZacharyWP04FlangerChorusAudioProcessorEditor()
{
}

//==============================================================================
void ZacharyWP04FlangerChorusAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::white);

    g.setColour (juce::Colours::black);
    g.setFont (25.0f);
    // g.drawFittedText ("Flanger", getLocalBounds(), juce::Justification::centredBottom, 1);
}

void ZacharyWP04FlangerChorusAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
