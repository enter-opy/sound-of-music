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
            std::make_unique<AudioParameterFloat>(CRUSH_ID, CRUSH_NAME, 0.0, 100.0, 0.0),
            std::make_unique<AudioParameterFloat>(DOWNSAMPLE_ID, DOWNSAMPLE_NAME, 0.0, 100.0, 0.0),
            std::make_unique<AudioParameterFloat>(JITTER_ID, JITTER_NAME, 0.0, 100.0, 0.0),
            std::make_unique<AudioParameterFloat>(CLIP_ID, CLIP_NAME, -15.0, 0.0, 0.0),
            std::make_unique<AudioParameterFloat>(MONO_ID, MONO_NAME, 0.0, 100.0, 0.0),
            std::make_unique<AudioParameterFloat>(MIX_ID, MIX_NAME, 0.0, 100.0, 100.0)
        }
    ),
    crushRaw(0.0),
    downSampleRaw(0.0),
    jitterRaw(0.0),
    clipRaw(0.0),
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
        return crushRaw;
    }
    else if (slider == 1) {
        return downSampleRaw;
    }
    else if (slider == 2) {
        return jitterRaw;
    }
    else if (slider == 3) {
        return clipRaw;
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
    crushRaw = *treeState.getRawParameterValue(CRUSH_ID);
    downSampleRaw = *treeState.getRawParameterValue(DOWNSAMPLE_ID);
    jitterRaw = *treeState.getRawParameterValue(JITTER_ID);
    clipRaw = *treeState.getRawParameterValue(CLIP_ID);

    mono_ = *treeState.getRawParameterValue(MONO_ID);
    mix_ = *treeState.getRawParameterValue(MIX_ID);

    int crushMap = mapToLog10((100.0 - crushRaw) / 100.0, 2.0, 32.0);
    int crush = pow(2, crushMap);
    
    int downSample = mapToLog10((100.0 - downSampleRaw) / 100.0, 441.0, 44101.0);

    float noise = jitterRaw / 100.0;
    int crackle = jitterRaw;

    float clip = Decibels::decibelsToGain(clipRaw);

    float mono = *treeState.getRawParameterValue(MONO_ID) / 200.0;
    float mix = *treeState.getRawParameterValue(MIX_ID) / 100.0;

    int step = samplerate / downSample;

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
            wet = round((dry)*crush) / crush;

            // jitter is added here
            wet += (random.nextInt(3) - 1) * noise * dry;
            if (crackle > 0) {
                if (random.nextInt(100 - crackle + 2) == 0) {
                    if (random.nextInt(10) != 0) {
                        wet = 0.0;
                    }
                }
            }

            // clipping happens here
            if (wet >= clip) {
                wet = clip;
            }
            else if (wet <= -clip) {
                wet = -clip;
            }
            wet *= 1 / clip;

            // downsampling happens here
            for (int i = 0; i < step && sample < buffer.getNumSamples(); i++, sample++) {
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
