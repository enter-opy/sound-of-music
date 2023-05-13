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

    bitdepthValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, CRUSH_ID, bitdepthSlider);
    samplerateValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, DOWNSAMPLE_ID, samplerateSlider);
    jitterValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, JITTER_ID, jitterSlider);
    clipCeilingValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, CLIP_ID, clipCeilingSlider);
    monoValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, MONO_ID, monoSlider);
    mixValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, MIX_ID, mixSlider);

    addAndMakeVisible(spectrum);

    bitdepthSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    bitdepthSlider.setRange(0.0, 100.0, 1.0);
    bitdepthSlider.setValue(audioProcessor.getValue(0));
    bitdepthSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    bitdepthLabel.setText("CRUSH", dontSendNotification);
    bitdepthLabel.setJustificationType(Justification::centred);
    bitdepthLabel.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&bitdepthLabel);
    bitdepthSlider.setLookAndFeel(&bitdepthLookAndFeel);
    bitdepthLookAndFeel.colourPosition = 75.0 / 30.0 * (bitdepthSlider.getValue() - 2.0);
    bitdepthSlider.addListener(this);
    addAndMakeVisible(&bitdepthSlider);

    samplerateSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    samplerateSlider.setRange(0.0, 100.0, 1.0);
    samplerateSlider.setValue(audioProcessor.getValue(1));
    samplerateSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    samplerateLabel.setText("DOWNSAMPLE", dontSendNotification);
    samplerateLabel.setJustificationType(Justification::centred);
    samplerateLabel.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&samplerateLabel);
    samplerateSlider.setLookAndFeel(&samplerateLookAndFeel);
    samplerateLookAndFeel.colourPosition = 75.0 / 42997.5 * (samplerateSlider.getValue() - 1102.5);
    samplerateSlider.addListener(this);
    addAndMakeVisible(&samplerateSlider);

    jitterSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    jitterSlider.setRange(0.0, 100.0, 1.0);
    jitterSlider.setValue(audioProcessor.getValue(2));
    jitterSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    jitterLabel.setText("JITTER", dontSendNotification);
    jitterLabel.setJustificationType(Justification::centred);
    jitterLabel.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&jitterLabel);
    jitterSlider.setLookAndFeel(&jitterLookAndFeel);
    jitterLookAndFeel.colourPosition = 75.0 / 100.0 * (jitterSlider.getValue());
    jitterSlider.addListener(this);
    addAndMakeVisible(&jitterSlider);

    clipCeilingSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    clipCeilingSlider.setRange(-15.0, 0.0, 0.1);
    clipCeilingSlider.setValue(audioProcessor.getValue(3));
    clipCeilingSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    clipCeilingLabel.setText("CLIP", dontSendNotification);
    clipCeilingLabel.setJustificationType(Justification::centred);
    clipCeilingLabel.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&clipCeilingLabel);
    clipCeilingSlider.setLookAndFeel(&clipCeilingLookAndFeel);
    clipCeilingLookAndFeel.colourPosition = 75.0 / 15.0 * (clipCeilingSlider.getValue() + 15);
    clipCeilingSlider.addListener(this);
    addAndMakeVisible(&clipCeilingSlider);

    monoSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    monoSlider.setRange(0.0, 100.0, 1.0);
    monoSlider.setValue(audioProcessor.getValue(5));
    monoSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    monoLabel.setText("STEREO/MONO", dontSendNotification);
    monoLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(&monoLabel);
    monoSlider.setLookAndFeel(&monoLookAndFeel);
    monoLookAndFeel.colourPosition = 90.0 / 100.0 * (monoSlider.getValue());
    monoSlider.addListener(this);
    addAndMakeVisible(&monoSlider);

    mixSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    mixSlider.setRange(0.0, 100.0, 0.0);
    mixSlider.setValue(audioProcessor.getValue(6));
    mixSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    mixLabel.setText("DRY/WET", dontSendNotification);
    mixLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(&mixLabel);
    mixSlider.setLookAndFeel(&mixLookAndFeel);
    mixLookAndFeel.colourPosition = 90.0 / 100.0 * (mixSlider.getValue());
    mixSlider.addListener(this);
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

    ColourGradient gradient(Colour::fromRGB(0x37, 0x9B, 0xE3), 40, 290, Colour::fromRGB(0xCB, 0x16, 0x6D), 40, 460, false);

    g.setGradientFill(gradient);
    g.drawRoundedRectangle(distortionArea, 10, 2);
}

void SoundofmusicAudioProcessorEditor::resized()
{
    spectrum.setBounds(40, 40, 800, 200);
    distortionArea.setBounds(40, 290, 480, 170);

    bitdepthSlider.setBounds(60, 310, 80, 80);
    bitdepthLabel.setBounds(50, 420, 100, 20);
    samplerateSlider.setBounds(180, 310, 80, 80);
    samplerateLabel.setBounds(170, 420, 100, 20);
    jitterSlider.setBounds(300, 310, 80, 80);
    jitterLabel.setBounds(290, 420, 100, 20);
    clipCeilingSlider.setBounds(420, 310, 80, 80);
    clipCeilingLabel.setBounds(410, 420, 100, 20);

    monoSlider.setBounds(560, 300, 100, 100);
    monoLabel.setBounds(560, 440, 100, 20);
    mixSlider.setBounds(720, 300, 100, 100);
    mixLabel.setBounds(720, 440, 100, 20);
}

void SoundofmusicAudioProcessorEditor::sliderValueChanged(Slider* slider) {
    if (slider == &bitdepthSlider) {
        bitdepthLookAndFeel.colourPosition = 90.0 / 100.0 * (bitdepthSlider.getValue());
    }
    else if (slider == &samplerateSlider) {
        samplerateLookAndFeel.colourPosition = 90.0 / 100.0 * (samplerateSlider.getValue());
    }
    else if (slider == &jitterSlider) {
        jitterLookAndFeel.colourPosition = 90.0 / 100.0 * (jitterSlider.getValue());
    }
    else if (slider == &clipCeilingSlider) {
        clipCeilingLookAndFeel.colourPosition = 90.0 / 15.0 * (clipCeilingSlider.getValue() + 15);
    }
    else if (slider == &monoSlider) {
        monoLookAndFeel.colourPosition = 90.0 / 100.0 * (monoSlider.getValue());
    }
    else if (slider == &mixSlider) {
        mixLookAndFeel.colourPosition = 90.0 / 100.0 * (mixSlider.getValue());
    }
}