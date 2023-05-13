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

    crushValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, CRUSH_ID, crushSlider);
    downSampleValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, DOWNSAMPLE_ID, downSampleSlider);
    jitterValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, JITTER_ID, jitterSlider);
    clipValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, CLIP_ID, clipSlider);

    monoValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, MONO_ID, monoSlider);
    mixValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, MIX_ID, mixSlider);

    addAndMakeVisible(spectrum);

    crushSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    crushSlider.setRange(0.0, 100.0, 1.0);
    crushSlider.setValue(audioProcessor.getValue(0));
    crushSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    crushName.setText("CRUSH", dontSendNotification);
    crushName.setJustificationType(Justification::centred);
    crushName.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&crushName);
    crushIndicator.setJustificationType(Justification::centred);
    crushIndicator.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&crushIndicator);
    crushLookAndFeel.colourPosition = 75.0 / 30.0 * crushSlider.getValue() - 2.0;
    crushSlider.setLookAndFeel(&crushLookAndFeel);
    crushSlider.addListener(this);
    addAndMakeVisible(&crushSlider);

    downSampleSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    downSampleSlider.setRange(0.0, 100.0, 1.0);
    downSampleSlider.setValue(audioProcessor.getValue(1));
    downSampleSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    downSampleName.setText("DOWNSAMPLE", dontSendNotification);
    downSampleName.setJustificationType(Justification::centred);
    downSampleName.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&downSampleName);
    downSampleIndicator.setJustificationType(Justification::centred);
    downSampleIndicator.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&downSampleIndicator);
    downSampleSlider.setLookAndFeel(&downSampleLookAndFeel);
    downSampleLookAndFeel.colourPosition = 75.0 / 42997.5 * (downSampleSlider.getValue() - 1102.5);
    downSampleSlider.addListener(this);
    addAndMakeVisible(&downSampleSlider);

    jitterSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    jitterSlider.setRange(0.0, 100.0, 1.0);
    jitterSlider.setValue(audioProcessor.getValue(2));
    jitterSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    jitterName.setText("JITTER", dontSendNotification);
    jitterName.setJustificationType(Justification::centred);
    jitterName.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&jitterName);
    jitterIndicator.setJustificationType(Justification::centred);
    jitterIndicator.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&jitterIndicator);
    jitterSlider.setLookAndFeel(&jitterLookAndFeel);
    jitterLookAndFeel.colourPosition = 75.0 / 100.0 * (jitterSlider.getValue());
    jitterSlider.addListener(this);
    addAndMakeVisible(&jitterSlider);

    clipSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    clipSlider.setRange(-15.0, 0.0, 0.1);
    clipSlider.setValue(audioProcessor.getValue(3));
    clipSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    clipName.setText("CLIP", dontSendNotification);
    clipName.setJustificationType(Justification::centred);
    clipName.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&clipName);
    clipIndicator.setJustificationType(Justification::centred);
    clipIndicator.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&clipIndicator);
    clipSlider.setLookAndFeel(&clipLookAndFeel);
    clipLookAndFeel.colourPosition = 75.0 / 15.0 * (clipSlider.getValue() + 15);
    clipSlider.addListener(this);
    addAndMakeVisible(&clipSlider);

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
    spectrum.setBounds(40, 40, 800, 220);

    distortionArea.setBounds(40, 290, 480, 170);

    crushSlider.setBounds(60, 310, 80, 80);
    crushName.setBounds(50, 420, 100, 20);
    crushIndicator.setBounds(50, 420, 100, 20);
    downSampleSlider.setBounds(180, 310, 80, 80);
    downSampleName.setBounds(170, 420, 100, 20);
    downSampleIndicator.setBounds(170, 420, 100, 20);
    jitterSlider.setBounds(300, 310, 80, 80);
    jitterName.setBounds(290, 420, 100, 20);
    jitterIndicator.setBounds(290, 420, 100, 20);
    clipSlider.setBounds(420, 310, 80, 80);
    clipName.setBounds(410, 420, 100, 20);
    clipIndicator.setBounds(410, 420, 100, 20);

    monoSlider.setBounds(560, 300, 100, 100);
    monoLabel.setBounds(560, 440, 100, 20);
    mixSlider.setBounds(720, 300, 100, 100);
    mixLabel.setBounds(720, 440, 100, 20);
}

void SoundofmusicAudioProcessorEditor::sliderValueChanged(Slider* slider) {
    String temp;

    if (slider == &crushSlider) {
        crushLookAndFeel.colourPosition = 90.0 / 100.0 * (crushSlider.getValue());
        temp = String(crushSlider.getValue()) + " %";
        crushIndicator.setText(temp, dontSendNotification);
    }
    else if (slider == &downSampleSlider) {
        downSampleLookAndFeel.colourPosition = 90.0 / 100.0 * (downSampleSlider.getValue());
        temp = String(downSampleSlider.getValue()) + " %";
        downSampleIndicator.setText(temp, dontSendNotification);
    }
    else if (slider == &jitterSlider) {
        jitterLookAndFeel.colourPosition = 90.0 / 100.0 * (jitterSlider.getValue());
        temp = String(jitterSlider.getValue()) + " %";
        jitterIndicator.setText(temp, dontSendNotification);
    }
    else if (slider == &clipSlider) {
        clipLookAndFeel.colourPosition = 90.0 / 15.0 * (clipSlider.getValue() + 15);
        temp = String(clipSlider.getValue()) + " dB";
        clipIndicator.setText(temp, dontSendNotification);
    }
    else if (slider == &monoSlider) {
        monoLookAndFeel.colourPosition = 90.0 / 100.0 * (monoSlider.getValue());
    }
    else if (slider == &mixSlider) {
        mixLookAndFeel.colourPosition = 90.0 / 100.0 * (mixSlider.getValue());
    }
}

void SoundofmusicAudioProcessorEditor::sliderDragStarted(Slider* slider) {
    if (slider == &crushSlider) {
        crushName.setVisible(false);
        crushIndicator.setVisible(true);
    }
    else if (slider == &downSampleSlider) {
        downSampleName.setVisible(false);
        downSampleIndicator.setVisible(true);
    }
    else if (slider == &jitterSlider) {
        jitterName.setVisible(false);
        jitterIndicator.setVisible(true);
    }
    else if (slider == &clipSlider) {
        clipName.setVisible(false);
        clipIndicator.setVisible(true);
    }
    else if (slider == &monoSlider) {
        
    }
    else if (slider == &mixSlider) {
        
    }
}

void SoundofmusicAudioProcessorEditor::sliderDragEnded(Slider* slider) {
    if (slider == &crushSlider) {
        crushName.setVisible(true);
        crushIndicator.setVisible(false);
    }
    else if (slider == &downSampleSlider) {
        downSampleName.setVisible(true);
        downSampleIndicator.setVisible(false);
    }
    else if (slider == &jitterSlider) {
        jitterName.setVisible(true);
        jitterIndicator.setVisible(false);
    }
    else if (slider == &clipSlider) {
        clipName.setVisible(true);
        clipIndicator.setVisible(false);
    }
    else if (slider == &monoSlider) {

    }
    else if (slider == &mixSlider) {

    }
}