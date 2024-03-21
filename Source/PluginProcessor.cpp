/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PolyJuce/Parameters/PolyParamTypes.h"
/*
class MetaParam:    public AudioParameterFloat
{
public:
    MetaParam(String paramId):
    AudioParameterFloat({paramId,1}, paramId,NormalisableRange<float> (0.f,1.f,0.01f),0.5f, AudioParameterFloatAttributes().withMeta(true))
    {
        
    }
    
    bool isDiscrete() const override
    { 
        return true;
    }
    
    int getNumSteps() const override
    {
        return 101;
    }
};*/


class MetaParam:    public PolyParamSelector
{
public:
    MetaParam(String paramId):
    PolyParamSelector(paramId, 0, 100, paramId)
    {
        setMetaParam();
    }
};


//==============================================================================
MetaBugAudioProcessor::MetaBugAudioProcessor()
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
    addListener((AudioProcessorListener*)this);
    //addParameter(metaSlaveParam = new AudioParameterFloat({"metaSlave",1},"metaSlave",NormalisableRange<float> (0.f,1.f),0.5f));
    addParameter(metaSlaveParam = new PolyParam("metaSlave","metaSlave"));
    addParameter(metaMasterParam = new MetaParam("meta master"));
}

MetaBugAudioProcessor::~MetaBugAudioProcessor()
{
    removeListener((AudioProcessorListener*)this);
}

//==============================================================================
const juce::String MetaBugAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MetaBugAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MetaBugAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MetaBugAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MetaBugAudioProcessor::getTailLengthSeconds() const
{
    return 36.0;
}

int MetaBugAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MetaBugAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MetaBugAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MetaBugAudioProcessor::getProgramName (int index)
{
    return {};
}

void MetaBugAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MetaBugAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MetaBugAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MetaBugAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    bool isSupported = false;
    
    AudioChannelSet mainInputCh = layouts.getMainInputChannelSet();
    AudioChannelSet sideChainCh = layouts.getChannelSet(true, 1);
    AudioChannelSet mainOutputCh = layouts.getMainOutputChannelSet();
    
    //{1,1} channel + stereo/mono side chain
    if (mainInputCh == AudioChannelSet::mono() &&
        (sideChainCh == AudioChannelSet::stereo() || sideChainCh == AudioChannelSet::mono()) &&
        mainOutputCh == AudioChannelSet::mono() )
    {
        isSupported = true;
    }
    
    //{2,2} channel + stereo/mono side chain
    if (mainInputCh == AudioChannelSet::stereo() &&
        (sideChainCh == AudioChannelSet::stereo() || sideChainCh == AudioChannelSet::mono()) &&
        mainOutputCh == AudioChannelSet::stereo() )
    {
        isSupported = true;
    }
    
    //{1,2} channel + stereo/mono side chain
    if (mainInputCh == AudioChannelSet::mono() &&
        (sideChainCh == AudioChannelSet::stereo() || sideChainCh == AudioChannelSet::mono()) &&
        mainOutputCh == AudioChannelSet::stereo() )
    {
        isSupported = true;
    }
    //DBG(mainInputCh.getDescription()+ " " + sideChainCh.getDescription()+ " " + mainOutputCh.getDescription() +" " + String(isSupported));
    
    return isSupported;
    //return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet() && ! layouts.getMainInputChannelSet().isDisabled();
}
#endif

void MetaBugAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool MetaBugAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MetaBugAudioProcessor::createEditor()
{
    return new MetaBugAudioProcessorEditor (*this);
}

//==============================================================================
void MetaBugAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MetaBugAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MetaBugAudioProcessor();
}

void MetaBugAudioProcessor::audioProcessorParameterChanged(AudioProcessor* processor,
                                    int parameterIndex,
                                    float newValue)
{
    auto paramName = processor->getParameters()[parameterIndex]->getName(25);
    DBG("Param Changed: "+paramName+ " val: "+String(newValue));
}
void MetaBugAudioProcessor::audioProcessorChanged (AudioProcessor* processor, const ChangeDetails& details)
{
    DBG("Processor Changed");
}
void MetaBugAudioProcessor::audioProcessorParameterChangeGestureBegin (AudioProcessor* processor,
                                                        int parameterIndex)
{
    auto paramName = processor->getParameters()[parameterIndex]->getName(25);
    DBG("Param Gesture Begin: "+paramName);
}
void MetaBugAudioProcessor::audioProcessorParameterChangeGestureEnd (AudioProcessor* processor,
                                                      int parameterIndex)
{
    auto paramName = processor->getParameters()[parameterIndex]->getName(25);
    DBG("Param Gesture End: "+paramName);
}
