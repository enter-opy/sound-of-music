/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define CRUSH_ID "crush"
#define CRUSH_NAME "Crush"

#define DOWNSAMPLE_ID "downsample"
#define DOWNSAMPLE_NAME "Downsample"

#define JITTER_ID "jitter"
#define JITTER_NAME "Jitter"

#define CLIP_ID "clip"
#define CLIP_NAME "Clip"

#define MIX_ID "mix"
#define MIX_NAME "Mix"

#define MONO_ID "mono"
#define MONO_NAME "Mono"

//==============================================================================
/**
*/
class SoundofmusicAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    SoundofmusicAudioProcessor();
    ~SoundofmusicAudioProcessor() override;

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
    void getStateInformation (juce::MemoryBlock&) override;
    void setStateInformation (const void*, int) override;

    double getValue(int);
    void pushNextSampleIntoFifo(float) noexcept;

    AudioProcessorValueTreeState treeState;

    enum
    {
        fftOrder = 11,
        fftSize = 1 << fftOrder,
        scopeSize = 512
    };

    float fifo[fftSize];
    float fftData[2 * fftSize];
    int fifoIndex = 0;
    bool nextFFTBlockReady = false;
    float scopeData[scopeSize];

private:
    float dry;
    float wet;
    float left;
    float right;

    float samplerate;

    Random random;

    float crushRaw;
    float downSampleRaw;
    float jitterRaw;
    float clipRaw;

    float mono_;
    float mix_;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundofmusicAudioProcessor)
};
