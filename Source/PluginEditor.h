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
class MetaBugAudioProcessorEditor  : public juce::AudioProcessorEditor, public Timer, public Slider::Listener
{
    Slider metaMasterSlider;
    Slider metaSlaveSlider;
    Label masterLbl;
    Label slaveLbl;
public:
    MetaBugAudioProcessorEditor (MetaBugAudioProcessor& p);
    ~MetaBugAudioProcessorEditor() override;

    void timerCallback() override;
    void sliderValueChanged (Slider* slider) override;
    void sliderDragStarted (Slider*) override;
    void sliderDragEnded (Slider*) override;
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MetaBugAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MetaBugAudioProcessorEditor)
};
