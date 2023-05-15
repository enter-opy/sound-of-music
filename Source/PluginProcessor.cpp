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
            std::make_unique<AudioParameterFloat>(CRUSHLOW_ID, CRUSHLOW_NAME, 0.0, 100.0, 0.0),
            std::make_unique<AudioParameterFloat>(DOWNSAMPLELOW_ID, DOWNSAMPLELOW_NAME, 0.0, 100.0, 0.0),
            std::make_unique<AudioParameterFloat>(JITTERLOW_ID, JITTERLOW_NAME, 0.0, 100.0, 0.0),
            std::make_unique<AudioParameterFloat>(CLIPLOW_ID, CLIPLOW_NAME, -15.0, 0.0, 0.0),
            std::make_unique<AudioParameterFloat>(CRUSHMID_ID, CRUSHMID_NAME, 0.0, 100.0, 0.0),
            std::make_unique<AudioParameterFloat>(DOWNSAMPLEMID_ID, DOWNSAMPLEMID_NAME, 0.0, 100.0, 0.0),
            std::make_unique<AudioParameterFloat>(JITTERMID_ID, JITTERMID_NAME, 0.0, 100.0, 0.0),
            std::make_unique<AudioParameterFloat>(CLIPMID_ID, CLIPMID_NAME, -15.0, 0.0, 0.0),
            std::make_unique<AudioParameterFloat>(CRUSHHIGH_ID, CRUSHHIGH_NAME, 0.0, 100.0, 0.0),
            std::make_unique<AudioParameterFloat>(DOWNSAMPLEHIGH_ID, DOWNSAMPLEHIGH_NAME, 0.0, 100.0, 0.0),
            std::make_unique<AudioParameterFloat>(JITTERHIGH_ID, JITTERHIGH_NAME, 0.0, 100.0, 0.0),
            std::make_unique<AudioParameterFloat>(CLIPHIGH_ID, CLIPHIGH_NAME, -15.0, 0.0, 0.0),
            std::make_unique<AudioParameterFloat>(MONO_ID, MONO_NAME, 0.0, 100.0, 0.0),
            std::make_unique<AudioParameterFloat>(MIX_ID, MIX_NAME, 0.0, 100.0, 100.0)
        }
    ),
    crushRawLow(0.0),
    downSampleRawLow(0.0),
    jitterRawLow(0.0),
    clipRawLow(0.0),
    crushRawMid(0.0),
    downSampleRawMid(0.0),
    jitterRawMid(0.0),
    clipRawMid(0.0),
    crushRawHigh(0.0),
    downSampleRawHigh(0.0),
    jitterRawHigh(0.0),
    clipRawHigh(0.0),
    monoRaw(0.0),
    mixRaw(100.0)
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
        return crushRawLow;
    }
    else if (slider == 1) {
        return downSampleRawLow;
    }
    else if (slider == 2) {
        return jitterRawLow;
    }
    else if (slider == 3) {
        return clipRawLow;
    }

    else if (slider == 4) {
        return crushRawMid;
    }
    else if (slider == 5) {
        return downSampleRawMid;
    }
    else if (slider == 6) {
        return jitterRawMid;
    }
    else if (slider == 7) {
        return clipRawMid;
    }

    else if (slider == 8) {
        return crushRawHigh;
    }
    else if (slider == 9) {
        return downSampleRawHigh;
    }
    else if (slider == 10) {
        return jitterRawHigh;
    }
    else if (slider == 11) {
        return clipRawHigh;
    }

    else if (slider == 12) {
        return monoRaw;
    }
    else if (slider == 13) {
        return mixRaw;
    }
}

void SoundofmusicAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    const int totalNumInputChannels = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // getting parameters
    crushRawLow = *treeState.getRawParameterValue(CRUSHLOW_ID);
    downSampleRawLow = *treeState.getRawParameterValue(DOWNSAMPLELOW_ID);
    jitterRawLow = *treeState.getRawParameterValue(JITTERLOW_ID);
    clipRawLow = *treeState.getRawParameterValue(CLIPLOW_ID);

    crushRawMid = *treeState.getRawParameterValue(CRUSHMID_ID);
    downSampleRawMid = *treeState.getRawParameterValue(DOWNSAMPLEMID_ID);
    jitterRawMid = *treeState.getRawParameterValue(JITTERMID_ID);
    clipRawMid = *treeState.getRawParameterValue(CLIPMID_ID);

    crushRawHigh = *treeState.getRawParameterValue(CRUSHHIGH_ID);
    downSampleRawHigh = *treeState.getRawParameterValue(DOWNSAMPLEHIGH_ID);
    jitterRawHigh = *treeState.getRawParameterValue(JITTERHIGH_ID);
    clipRawHigh = *treeState.getRawParameterValue(CLIPHIGH_ID);

    monoRaw = *treeState.getRawParameterValue(MONO_ID);
    mixRaw = *treeState.getRawParameterValue(MIX_ID);

    int crushMapLow = mapToLog10((100.0 - crushRawLow) / 100.0, 2.0, 32.0);
    int crushLow = pow(2, crushMapLow);
    int crushMapMid = mapToLog10((100.0 - crushRawMid) / 100.0, 2.0, 32.0);
    int crushMid = pow(2, crushMapMid);
    int crushMapHigh = mapToLog10((100.0 - crushRawHigh) / 100.0, 2.0, 32.0);
    int crushHigh = pow(2, crushMapHigh);
    
    int downSampleLow = mapToLog10((100.0 - downSampleRawLow) / 100.0, 441.0, 44101.0);
    int downSampleMid = mapToLog10((100.0 - downSampleRawMid) / 100.0, 441.0, 44101.0);
    int downSampleHigh = mapToLog10((100.0 - downSampleRawHigh) / 100.0, 441.0, 44101.0);
    int stepLow = samplerate / downSampleLow;
    int stepMid = samplerate / downSampleMid;
    int stepHigh = samplerate / downSampleHigh;

    float noiseLow = jitterRawLow / 100.0;
    int crackleLow = jitterRawLow;
    float noiseMid = jitterRawMid / 100.0;
    int crackleMid = jitterRawMid;
    float noiseHigh = jitterRawHigh / 100.0;
    int crackleHigh = jitterRawHigh;

    float clipLow = Decibels::decibelsToGain(clipRawLow);
    float clipMid = Decibels::decibelsToGain(clipRawMid);
    float clipHigh = Decibels::decibelsToGain(clipRawHigh);

    float mono = monoRaw / 200.0;
    float mix = mixRaw / 100.0;

    float* leftchannelData = buffer.getWritePointer(0);
    float* rightchannelData = buffer.getWritePointer(1);

    for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
        pushNextSampleIntoFifoIn((leftchannelData[sample] + rightchannelData[sample]) * 0.5);
    }

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ) {
            dry = channelData[sample];

            // bitcrushing happens here
            wet = round((dry)*crushLow) / crushLow;

            // jitter is added here
            wet += (random.nextInt(3) - 1) * noiseLow * dry;
            if (crackleLow > 0) {
                if (random.nextInt(100 - crackleLow + 2) == 0) {
                    if (random.nextInt(10) != 0) {
                        wet = 0.0;
                    }
                }
            }

            // clipping happens here
            if (wet >= clipLow) {
                wet = clipLow;
            }
            else if (wet <= -clipLow) {
                wet = -clipLow;
            }
            wet *= 1 / clipLow;

            // downsampling happens here
            for (int i = 0; i < stepLow && sample < buffer.getNumSamples(); i++, sample++) {
                // mix is applied here
                channelData[sample] = (1 - mix) * dry + mix * wet;
            }
        }
    }

    for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
        pushNextSampleIntoFifoOut((leftchannelData[sample] + rightchannelData[sample]) * 0.5);

        left = (1 - mono) * leftchannelData[sample] + mono * rightchannelData[sample];
        right = mono * leftchannelData[sample] + (1 - mono) * rightchannelData[sample];

        leftchannelData[sample] = (1 - mix) * leftchannelData[sample] + mix * left;
        rightchannelData[sample] = (1 - mix) * rightchannelData[sample] + mix * right;
    }
}

void SoundofmusicAudioProcessor::pushNextSampleIntoFifoIn(float sample) noexcept
{
    if (fifoInIndex == fftSize)
    {
        if (!nextFFTBlockReadyIn)
        {
            juce::zeromem(fftDataIn, sizeof(fftDataIn));
            memcpy(fftDataIn, fifoIn, sizeof(fifoIn));
            nextFFTBlockReadyIn = true;
        }
        fifoInIndex = 0;
    }
    fifoIn[fifoInIndex++] = sample;
}

void SoundofmusicAudioProcessor::pushNextSampleIntoFifoOut(float sample) noexcept
{
    if (fifoOutIndex == fftSize)
    {
        if (!nextFFTBlockReadyOut)
        {
            juce::zeromem(fftDataOut, sizeof(fftDataOut));
            memcpy(fftDataOut, fifoOut, sizeof(fifoOut));
            nextFFTBlockReadyOut = true;
        }
        fifoOutIndex = 0;
    }
    fifoOut[fifoOutIndex++] = sample;
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
