/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoundofmusicAudioProcessorEditor::SoundofmusicAudioProcessorEditor (SoundofmusicAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize(880, 520);

    bitdepthValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, BITDEPTH_ID, bitdepthSlider);
    samplerateValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, SAMPLERATE_ID, samplerateSlider);
    jitterValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, JITTER_ID, jitterSlider);
    clipCeilingValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, CLIPCELING_ID, clipCeilingSlider);
    crackleValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, CRACKLE_ID, crackleSlider);
    monoValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, MONO_ID, monoSlider);
    mixValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, MIX_ID, mixSlider);

    addAndMakeVisible(spectrum);

    bitdepthSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    bitdepthSlider.setRange(2.0, 32.0, 1.0);
    bitdepthSlider.setValue(audioProcessor.getValue(0));
    bitdepthSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    bitdepthLabel.setText("BITDEPTH", dontSendNotification);
    bitdepthLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(&bitdepthLabel);
    bitdepthSlider.setLookAndFeel(&sliderLookAndFeel);
    addAndMakeVisible(&bitdepthSlider);

    samplerateSlider.setSliderStyle(Slider::LinearBarVertical);
    samplerateSlider.setRange(1102.5, 44100.0, 1.0);
    samplerateSlider.setValue(audioProcessor.getValue(1));
    samplerateSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    samplerateLabel.setText("SAMPLERATE", dontSendNotification);
    samplerateLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(&samplerateLabel);
    samplerateSlider.setLookAndFeel(&sliderLookAndFeel);
    addAndMakeVisible(&samplerateSlider);

    jitterSlider.setSliderStyle(Slider::LinearBarVertical);
    jitterSlider.setRange(0.0, 100.0, 1.0);
    jitterSlider.setValue(audioProcessor.getValue(2));
    jitterSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    jitterLabel.setText("JITTER", dontSendNotification);
    jitterLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(&jitterLabel);
    jitterSlider.setLookAndFeel(&sliderLookAndFeel);
    addAndMakeVisible(&jitterSlider);

    clipCeilingSlider.setSliderStyle(Slider::LinearBarVertical);
    clipCeilingSlider.setRange(-15.0, 0.0, 0.1);
    clipCeilingSlider.setValue(audioProcessor.getValue(3));
    clipCeilingSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    clipCeilingLabel.setText("CLIP", dontSendNotification);
    clipCeilingLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(&clipCeilingLabel);
    clipCeilingSlider.setLookAndFeel(&sliderLookAndFeel);
    addAndMakeVisible(&clipCeilingSlider);

    crackleSlider.setSliderStyle(Slider::LinearBarVertical);
    crackleSlider.setRange(0.0, 100.0, 1.0);
    crackleSlider.setValue(audioProcessor.getValue(4));
    crackleSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    crackleLabel.setText("CRACKLE", dontSendNotification);
    crackleLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(&crackleLabel);
    crackleSlider.setLookAndFeel(&sliderLookAndFeel);
    addAndMakeVisible(&crackleSlider);

    monoSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    monoSlider.setRange(0.0, 100.0, 1.0);
    monoSlider.setValue(audioProcessor.getValue(5));
    monoSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    monoLabel.setText("STEREO/MONO", dontSendNotification);
    monoLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(&monoLabel);
    monoSlider.setLookAndFeel(&sliderLookAndFeel);
    addAndMakeVisible(&monoSlider);

    mixSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    mixSlider.setRange(0.0, 100.0, 0.0);
    mixSlider.setValue(audioProcessor.getValue(6));
    mixSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    mixLabel.setText("DRY/WET", dontSendNotification);
    mixLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(&mixLabel);
    mixSlider.setLookAndFeel(&sliderLookAndFeel);
    addAndMakeVisible(&mixSlider);
}

SoundofmusicAudioProcessorEditor::~SoundofmusicAudioProcessorEditor()
{
}

//==============================================================================
void SoundofmusicAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setColour(Colour::fromRGB(0x0D, 0x10, 0x15));
    g.fillAll();
}

void SoundofmusicAudioProcessorEditor::resized()
{
    spectrum.setBounds(getLocalBounds());

    bitdepthSlider.setBounds(60, 275, 20, 150);
    bitdepthLabel.setBounds(20, 440, 100, 20);
    samplerateSlider.setBounds(160, 275, 20, 150);
    samplerateLabel.setBounds(120, 440, 100, 20);
    jitterSlider.setBounds(260, 275, 20, 150);
    jitterLabel.setBounds(220, 440, 100, 20);
    clipCeilingSlider.setBounds(360, 275, 20, 150);
    clipCeilingLabel.setBounds(320, 440, 100, 20);
    crackleSlider.setBounds(460, 275, 20, 150);
    crackleLabel.setBounds(420, 440, 100, 20);

    monoSlider.setBounds(560, 300, 100, 100);
    monoLabel.setBounds(560, 440, 100, 20);
    mixSlider.setBounds(720, 300, 100, 100);
    mixLabel.setBounds(720, 440, 100, 20);
}