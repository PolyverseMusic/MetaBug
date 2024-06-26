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
    masterLbl.setText("master",NotificationType::dontSendNotification);
    slaveLbl.setText("slave",NotificationType::dontSendNotification);
    addAndMakeVisible(metaMasterSlider);
    addAndMakeVisible(metaSlaveSlider);
    addAndMakeVisible(masterLbl);
    addAndMakeVisible(slaveLbl);
    metaSlaveSlider.addListener(this);
    metaMasterSlider.addListener(this);
    metaSlaveSlider.setRange(0.f, 1.f);
    metaMasterSlider.setRange(0.f, 1.f);
    metaSlaveSlider.setSliderSnapsToMousePosition(false);
    metaMasterSlider.setSliderSnapsToMousePosition(false);
    
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
    g.setColour(Colours::white);
    Rectangle<int> r(10,10,getWidth(),20);
    g.drawText("Move the master, and the click - but DONT DRAG the slave.", r, Justification::centredLeft);
    g.drawText("It will pop into the last value reported to the host", r.withY(30), Justification::centredLeft);
    g.drawText("Number of Parameters: "+String(processor.getParameters().size()), 10, getHeight()-40, 200, 20, Justification::centredLeft);
    g.drawText(String(__DATE__)+" "+String(__TIME__), 10, getHeight()-20, 200, 20, Justification::centredLeft);
}

void MetaBugAudioProcessorEditor::resized()
{
    metaMasterSlider.setBounds(10,50,200,40);
    metaSlaveSlider.setBounds(10,90,200,40);
    masterLbl.setBounds(210,50,100,40);
    slaveLbl.setBounds(210,90,100,40);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
void MetaBugAudioProcessorEditor::timerCallback()
{
    auto slaveVal = audioProcessor.metaSlaveParam->getValue();
    auto masterVal = audioProcessor.metaMasterParam->getValue();
    
    if (slaveVal != metaSlaveSlider.getValue())
    {
        metaSlaveSlider.setValue(slaveVal,NotificationType::dontSendNotification);
    }
    if (masterVal != metaMasterSlider.getValue())
    {
        metaMasterSlider.setValue(masterVal,NotificationType::dontSendNotification);
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
        
        audioProcessor.metaMasterParam->setValueNotifyingHost(slider->getValue());
        audioProcessor.metaSlaveParam->setValue(audioProcessor.metaMasterParam->getValue());
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
