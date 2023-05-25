/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define CRUSHLOW_ID "crushlow"
#define CRUSHLOW_NAME "CrushLow"

#define CRUSHMID_ID "crushmid"
#define CRUSHMID_NAME "CrushMid"

#define CRUSHHIGH_ID "crushhigh"
#define CRUSHHIGH_NAME "CrushHigh"

#define DOWNSAMPLELOW_ID "downsamplelow"
#define DOWNSAMPLELOW_NAME "DownsampleLow"

#define DOWNSAMPLEMID_ID "downsamplemid"
#define DOWNSAMPLEMID_NAME "DownsampleMid"

#define DOWNSAMPLEHIGH_ID "downsamplehigh"
#define DOWNSAMPLEHIGH_NAME "DownsampleHigh"

#define JITTERLOW_ID "jitterlow"
#define JITTERLOW_NAME "JitterLow"

#define JITTERMID_ID "jittermid"
#define JITTERMID_NAME "JitterMid"

#define JITTERHIGH_ID "jitterhigh"
#define JITTERHIGH_NAME "JitterHigh"

#define CLIPLOW_ID "cliplow"
#define CLIPLOW_NAME "ClipLow"

#define CLIPMID_ID "clipmid"
#define CLIPMID_NAME "ClipMid"

#define CLIPHIGH_ID "cliphigh"
#define CLIPHIGH_NAME "ClipHigh"

#define MIX_ID "mix"
#define MIX_NAME "Mix"

#define MONO_ID "mono"
#define MONO_NAME "Mono"

#define FREQ1_ID "frequency1"
#define FREQ1_NAME "Frequency1"

#define FREQ2_ID "frequency2"
#define FREQ2_NAME "Frequency2"

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
    void pushNextSampleIntoFifoIn(float) noexcept;
    void pushNextSampleIntoFifoOut(float) noexcept;

    AudioProcessorValueTreeState treeState;

    enum
    {
        fftOrder = 10,
        fftSize = 1 << fftOrder,
        scopeSize = 512
    };

    float fifoIn[fftSize];
    float fftDataIn[2 * fftSize];
    int fifoInIndex = 0;
    bool nextFFTBlockReadyIn = false;
    float scopeDataIn[scopeSize];

    float fifoOut[fftSize];
    float fftDataOut[2 * fftSize];
    int fifoOutIndex = 0;
    bool nextFFTBlockReadyOut = false;
    float scopeDataOut[scopeSize];

private:
    float dryLow;
    float wetLow;
    float dryMid;
    float wetMid;
    float dryHigh;
    float wetHigh;

    float left;
    float right;

    Random random;

    float crushRawLow;
    float downSampleRawLow;
    float jitterRawLow;
    float clipRawLow;

    float crushRawMid;
    float downSampleRawMid;
    float jitterRawMid;
    float clipRawMid;

    float crushRawHigh;
    float downSampleRawHigh;
    float jitterRawHigh;
    float clipRawHigh;

    float monoRaw;
    float mixRaw;

    IIRFilter lowBandLowPassL1, lowBandLowPassR1;
    IIRFilter midBandHighPassL1, midBandHighPassR1;
    IIRFilter midBandLowPassL1, midBandLowPassR1;
    IIRFilter highBandHighPassL1, highBandHighPassR1;

    IIRFilter lowBandLowPassL2, lowBandLowPassR2;
    IIRFilter midBandHighPassL2, midBandHighPassR2;
    IIRFilter midBandLowPassL2, midBandLowPassR2;
    IIRFilter highBandHighPassL2, highBandHighPassR2;

    IIRCoefficients coefficients;

    float stepFactorLow;
    float stepFactorMid;
    float stepFactorHigh;

    float frequency1;
    float frequency2;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundofmusicAudioProcessor)
};
