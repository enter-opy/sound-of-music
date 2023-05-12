/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoundofmusicAudioProcessor::SoundofmusicAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    treeState(*this, nullptr, "PARAMETER",
        {
            std::make_unique<AudioParameterFloat>(BITDEPTH_ID, BITDEPTH_NAME, 2.0, 32.0, 32.0),
            std::make_unique<AudioParameterFloat>(SAMPLERATE_ID, SAMPLERATE_NAME, 1102.5, 44100.0, 44100.0),
            std::make_unique<AudioParameterFloat>(JITTER_ID, JITTER_NAME, 0.0, 100.0, 0.0),
            std::make_unique<AudioParameterFloat>(CLIPCELING_ID, CLIPCELING_NAME, -15.0, 0.0, 0.0),
            std::make_unique<AudioParameterFloat>(CRACKLE_ID, CRACKLE_NAME, 0.0, 100.0, 0.0),
            std::make_unique<AudioParameterFloat>(MONO_ID, MONO_NAME, 0.0, 100.0, 0.0),
            std::make_unique<AudioParameterFloat>(MIX_ID, MIX_NAME, 0.0, 100.0, 100.0)
        }
    ),
    bitdepth_(32.0),
    samplerate_(44100.0),
    jitter_(0.0),
    clip_(0.0),
    crackle_(0.0),
    mono_(0.0),
    mix_(100.0)
#endif
{
    treeState.state = ValueTree("savedParams");
}

SoundofmusicAudioProcessor::~SoundofmusicAudioProcessor()
{
}

//==============================================================================
const juce::String SoundofmusicAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SoundofmusicAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SoundofmusicAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SoundofmusicAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SoundofmusicAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SoundofmusicAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SoundofmusicAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SoundofmusicAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SoundofmusicAudioProcessor::getProgramName (int index)
{
    return {};
}

void SoundofmusicAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SoundofmusicAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    samplerate = sampleRate;
}

void SoundofmusicAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SoundofmusicAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

double SoundofmusicAudioProcessor::getValue(int slider) {
    if (slider == 0) {
        return bitdepth_;
    }
    else if (slider == 1) {
        return samplerate_;
    }
    else if (slider == 2) {
        return jitter_;
    }
    else if (slider == 3) {
        return clip_;
    }
    else if (slider == 4) {
        return crackle_;
    }
    else if (slider == 5) {
        return mono_;
    }
    else if (slider == 6) {
        return mix_;
    }
}

void SoundofmusicAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    const int totalNumInputChannels = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // getting parameters
    bitdepth_ = *treeState.getRawParameterValue(BITDEPTH_ID);
    samplerate_ = *treeState.getRawParameterValue(SAMPLERATE_ID);
    jitter_ = *treeState.getRawParameterValue(JITTER_ID);
    clip_ = *treeState.getRawParameterValue(CLIPCELING_ID);
    crackle_ = *treeState.getRawParameterValue(CRACKLE_ID);
    mono_ = *treeState.getRawParameterValue(MONO_ID);
    mix_ = *treeState.getRawParameterValue(MIX_ID);

    int bitdepth = pow(2, *treeState.getRawParameterValue(BITDEPTH_ID)) / 2;
    int newSamplerate = *treeState.getRawParameterValue(SAMPLERATE_ID);
    float jitter = *treeState.getRawParameterValue(JITTER_ID) / 100.0;
    float clipCeiling = Decibels::decibelsToGain((float)*treeState.getRawParameterValue(CLIPCELING_ID));
    int crackle = *treeState.getRawParameterValue(CRACKLE_ID);
    float mono = *treeState.getRawParameterValue(MONO_ID) / 200.0;
    float mix = *treeState.getRawParameterValue(MIX_ID) / 100.0;

    int step = samplerate / newSamplerate;

    float* leftchannelData = buffer.getWritePointer(0);
    float* rightchannelData = buffer.getWritePointer(1);

    // stereo width is reduced here
    for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
        leftSample = (1 - mono) * leftchannelData[sample] + mono * rightchannelData[sample];
        rightSample = mono * leftchannelData[sample] + (1 - mono) * rightchannelData[sample];

        leftchannelData[sample] = (1 - mix) * leftchannelData[sample] + mix * leftSample;
        rightchannelData[sample] = (1 - mix) * rightchannelData[sample] + mix * rightSample;
    }

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ) {
            drySample = channelData[sample];

            // bitdepth reduction happens here
            wetSample = round((drySample)*bitdepth) / bitdepth;

            // jitter is added here
            wetSample += (random.nextInt(3) - 1) * jitter * drySample;

            // hard clipping happens here
            if (wetSample >= clipCeiling) {
                wetSample = clipCeiling;
            }
            else if (wetSample <= -clipCeiling) {
                wetSample = -clipCeiling;
            }
            wetSample *= 1 / clipCeiling;

            // crackles are added here
            if (crackle > 0) {
                if (random.nextInt(100 - crackle + 2) == 0) {
                    if (random.nextInt(10) == 0) {
                        wetSample = -wetSample;
                    }
                    else {
                        wetSample = 0.0;
                    }
                }
            }

            // sample rate reduction happens here
            for (int i = 0; i < step && sample < buffer.getNumSamples(); i++, sample++) {
                // mix is applied here
                channelData[sample] = (1 - mix) * channelData[sample] + mix * wetSample;
            }
        }
    }
}

//==============================================================================
bool SoundofmusicAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SoundofmusicAudioProcessor::createEditor()
{
    return new SoundofmusicAudioProcessorEditor (*this);
}

//==============================================================================
void SoundofmusicAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    std::unique_ptr <XmlElement> xml(treeState.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SoundofmusicAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr <XmlElement> params(getXmlFromBinary(data, sizeInBytes));

    if (params != nullptr) {
        if (params->hasTagName(treeState.state.getType())) {
            treeState.state = ValueTree::fromXml(*params);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SoundofmusicAudioProcessor();
}
