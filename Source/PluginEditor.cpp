/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MetaBugAudioProcessorEditor::MetaBugAudioProcessorEditor (MetaBugAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    startTimerHz(30);
}

MetaBugAudioProcessorEditor::~MetaBugAudioProcessorEditor()
{
}

//==============================================================================
void MetaBugAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    addAndMakeVisible(metaMasterSlider);
    addAndMakeVisible(metaSlaveSlider);
    addAndMakeVisible(metaMasterSlider);
    addAndMakeVisible(metaSlaveSlider);
    metaSlaveSlider.addListener(this);
    metaMasterSlider.addListener(this);
    metaSlaveSlider.setRange(0.f, 1.f);
    metaMasterSlider.setRange(0.f, 1.f);
    metaSlaveSlider.setSliderSnapsToMousePosition(false);
    metaMasterSlider.setSliderSnapsToMousePosition(false);
}

void MetaBugAudioProcessorEditor::resized()
{
    metaMasterSlider.setBounds(10,10,200,40);
    metaSlaveSlider.setBounds(10,50,200,40);
    masterLbl.setBounds(10,110,40,20);
    slaveLbl.setBounds(50,110,40,20);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
void MetaBugAudioProcessorEditor::timerCallback()
{   auto slaveVal = audioProcessor.metaSlaveParam->getValue();
    auto masterVal = audioProcessor.metaMasterParam->getValue();
    if (slaveVal != metaSlaveSlider.getValue())
    {
        metaSlaveSlider.setValue(slaveVal);
    }
    if (masterVal != metaMasterSlider.getValue())
    {
        metaMasterSlider.setValue(masterVal);
    }
}
void MetaBugAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    if (slider == &metaSlaveSlider)
    {
        audioProcessor.metaSlaveParam->setValueNotifyingHost(slider->getValue());
    }
    else if (slider == &metaMasterSlider)
    {
        audioProcessor.metaSlaveParam->setValue(slider->getValue());
        audioProcessor.metaMasterParam->setValueNotifyingHost(slider->getValue());
    }
}
void MetaBugAudioProcessorEditor::sliderDragStarted (Slider* slider)
{
    if (slider == &metaSlaveSlider)
    {
        audioProcessor.metaSlaveParam->beginChangeGesture();
    }
    else if (slider == &metaMasterSlider)
    {
        audioProcessor.metaMasterParam->beginChangeGesture();
    }
}
void MetaBugAudioProcessorEditor::sliderDragEnded (Slider* slider)
{
    if (slider == &metaSlaveSlider)
    {
        audioProcessor.metaSlaveParam->endChangeGesture();
    }
    else if (slider == &metaMasterSlider)
    {
        audioProcessor.metaMasterParam->endChangeGesture();
    }
}
