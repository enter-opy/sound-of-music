/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoundofmusicAudioProcessorEditor::SoundofmusicAudioProcessorEditor (SoundofmusicAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), forwardFFTIn(10), forwardFFTOut(10),
    window(1 << 10, juce::dsp::WindowingFunction<float>::hann)
{
    setSize(880, 520);
    startTimerHz(24);

    crushValueLow = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, CRUSHLOW_ID, crushSliderLow);
    downSampleValueLow = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, DOWNSAMPLELOW_ID, downSampleSliderLow);
    jitterValueLow = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, JITTERLOW_ID, jitterSliderLow);
    clipValueLow = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, CLIPLOW_ID, clipSliderLow);

    crushValueMid = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, CRUSHMID_ID, crushSliderMid);
    downSampleValueMid = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, DOWNSAMPLEMID_ID, downSampleSliderMid);
    jitterValueMid = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, JITTERMID_ID, jitterSliderMid);
    clipValueMid = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, CLIPMID_ID, clipSliderMid);

    crushValueHigh = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, CRUSHHIGH_ID, crushSliderHigh);
    downSampleValueHigh = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, DOWNSAMPLEHIGH_ID, downSampleSliderHigh);
    jitterValueHigh = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, JITTERHIGH_ID, jitterSliderHigh);
    clipValueHigh = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, CLIPHIGH_ID, clipSliderHigh);

    monoValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, MONO_ID, monoSlider);
    mixValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, MIX_ID, mixSlider);

    spectrum.addMouseListener(this, false);
    addAndMakeVisible(spectrum);

    crushSliderLow.setSliderStyle(Slider::RotaryVerticalDrag);
    crushSliderLow.setRange(0.0, 100.0, 1.0);
    crushSliderLow.setValue(audioProcessor.getValue(0));
    crushSliderLow.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    crushName.setText("CRUSH", dontSendNotification);
    crushName.setJustificationType(Justification::centred);
    crushName.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&crushName);
    crushIndicatorLow.setJustificationType(Justification::centred);
    crushIndicatorLow.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    crushIndicatorLow.setVisible(false);
    addAndMakeVisible(&crushIndicatorLow);
    crushSliderLow.setLookAndFeel(&crushLookAndFeelLow);
    crushSliderLow.addListener(this);
    addAndMakeVisible(&crushSliderLow);

    crushSliderMid.setSliderStyle(Slider::RotaryVerticalDrag);
    crushSliderMid.setRange(0.0, 100.0, 1.0);
    crushSliderMid.setValue(audioProcessor.getValue(4));
    crushSliderMid.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    crushIndicatorMid.setJustificationType(Justification::centred);
    crushIndicatorMid.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    crushIndicatorMid.setVisible(false);
    addAndMakeVisible(&crushIndicatorMid);
    crushSliderMid.setLookAndFeel(&crushLookAndFeelMid);
    crushSliderMid.addListener(this);
    addAndMakeVisible(&crushSliderMid);
    crushSliderMid.setVisible(false);

    crushSliderHigh.setSliderStyle(Slider::RotaryVerticalDrag);
    crushSliderHigh.setRange(0.0, 100.0, 1.0);
    crushSliderHigh.setValue(audioProcessor.getValue(8));
    crushSliderHigh.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    crushIndicatorHigh.setJustificationType(Justification::centred);
    crushIndicatorHigh.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&crushIndicatorHigh);
    crushIndicatorHigh.setVisible(false);
    crushSliderHigh.setLookAndFeel(&crushLookAndFeelHigh);
    crushSliderHigh.addListener(this);
    addAndMakeVisible(&crushSliderHigh);
    crushSliderHigh.setVisible(false);

    downSampleSliderLow.setSliderStyle(Slider::RotaryVerticalDrag);
    downSampleSliderLow.setRange(0.0, 100.0, 1.0);
    downSampleSliderLow.setValue(audioProcessor.getValue(1));
    downSampleSliderLow.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    downSampleName.setText("DOWNSAMPLE", dontSendNotification);
    downSampleName.setJustificationType(Justification::centred);
    downSampleName.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&downSampleName);
    downSampleIndicatorLow.setJustificationType(Justification::centred);
    downSampleIndicatorLow.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&downSampleIndicatorLow);
    downSampleSliderLow.setLookAndFeel(&downSampleLookAndFeelLow);
    downSampleSliderLow.addListener(this);
    addAndMakeVisible(&downSampleSliderLow);

    downSampleSliderMid.setSliderStyle(Slider::RotaryVerticalDrag);
    downSampleSliderMid.setRange(0.0, 100.0, 1.0);
    downSampleSliderMid.setValue(audioProcessor.getValue(5));
    downSampleSliderMid.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    downSampleIndicatorMid.setJustificationType(Justification::centred);
    downSampleIndicatorMid.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&downSampleIndicatorMid);
    downSampleSliderMid.setLookAndFeel(&downSampleLookAndFeelMid);
    downSampleSliderMid.addListener(this);
    addAndMakeVisible(&downSampleSliderMid);
    downSampleSliderMid.setVisible(false);

    downSampleSliderHigh.setSliderStyle(Slider::RotaryVerticalDrag);
    downSampleSliderHigh.setRange(0.0, 100.0, 1.0);
    downSampleSliderHigh.setValue(audioProcessor.getValue(9));
    downSampleSliderHigh.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    downSampleIndicatorHigh.setJustificationType(Justification::centred);
    downSampleIndicatorHigh.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&downSampleIndicatorHigh);
    downSampleSliderHigh.setLookAndFeel(&downSampleLookAndFeelHigh);
    downSampleSliderHigh.addListener(this);
    addAndMakeVisible(&downSampleSliderHigh);
    downSampleSliderHigh.setVisible(false);

    jitterSliderLow.setSliderStyle(Slider::RotaryVerticalDrag);
    jitterSliderLow.setRange(0.0, 100.0, 1.0);
    jitterSliderLow.setValue(audioProcessor.getValue(2));
    jitterSliderLow.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    jitterName.setText("JITTER", dontSendNotification);
    jitterName.setJustificationType(Justification::centred);
    jitterName.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&jitterName);
    jitterIndicatorLow.setJustificationType(Justification::centred);
    jitterIndicatorLow.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&jitterIndicatorLow);
    jitterSliderLow.setLookAndFeel(&jitterLookAndFeelLow);
    jitterSliderLow.addListener(this);
    addAndMakeVisible(&jitterSliderLow);

    jitterSliderMid.setSliderStyle(Slider::RotaryVerticalDrag);
    jitterSliderMid.setRange(0.0, 100.0, 1.0);
    jitterSliderMid.setValue(audioProcessor.getValue(6));
    jitterSliderMid.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    jitterIndicatorMid.setJustificationType(Justification::centred);
    jitterIndicatorMid.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&jitterIndicatorMid);
    jitterSliderMid.setLookAndFeel(&jitterLookAndFeelMid);
    jitterSliderMid.addListener(this);
    addAndMakeVisible(&jitterSliderMid);
    jitterSliderMid.setVisible(false);

    jitterSliderHigh.setSliderStyle(Slider::RotaryVerticalDrag);
    jitterSliderHigh.setRange(0.0, 100.0, 1.0);
    jitterSliderHigh.setValue(audioProcessor.getValue(10));
    jitterSliderHigh.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    jitterIndicatorHigh.setJustificationType(Justification::centred);
    jitterIndicatorHigh.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&jitterIndicatorHigh);
    jitterSliderHigh.setLookAndFeel(&jitterLookAndFeelHigh);
    jitterSliderHigh.addListener(this);
    addAndMakeVisible(&jitterSliderHigh);
    jitterSliderHigh.setVisible(false);

    clipSliderLow.setSliderStyle(Slider::RotaryVerticalDrag);
    clipSliderLow.setRange(-15.0, 0.0, 0.1);
    clipSliderLow.setValue(audioProcessor.getValue(3));
    clipSliderLow.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    clipName.setText("CLIP", dontSendNotification);
    clipName.setJustificationType(Justification::centred);
    clipName.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&clipName);
    clipIndicatorLow.setJustificationType(Justification::centred);
    clipIndicatorLow.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&clipIndicatorLow);
    clipSliderLow.setLookAndFeel(&clipLookAndFeelLow);
    clipSliderLow.addListener(this);
    addAndMakeVisible(&clipSliderLow);

    clipSliderMid.setSliderStyle(Slider::RotaryVerticalDrag);
    clipSliderMid.setRange(-15.0, 0.0, 0.1);
    clipSliderMid.setValue(audioProcessor.getValue(7));
    clipSliderMid.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    clipIndicatorMid.setJustificationType(Justification::centred);
    clipIndicatorMid.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&clipIndicatorMid);
    clipSliderMid.setLookAndFeel(&clipLookAndFeelMid);
    clipSliderMid.addListener(this);
    addAndMakeVisible(&clipSliderMid);
    clipSliderMid.setVisible(false);

    clipSliderHigh.setSliderStyle(Slider::RotaryVerticalDrag);
    clipSliderHigh.setRange(-15.0, 0.0, 0.1);
    clipSliderHigh.setValue(audioProcessor.getValue(11));
    clipSliderHigh.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    clipIndicatorHigh.setJustificationType(Justification::centred);
    clipIndicatorHigh.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&clipIndicatorHigh);
    clipSliderHigh.setLookAndFeel(&clipLookAndFeelHigh);
    clipSliderHigh.addListener(this);
    addAndMakeVisible(&clipSliderHigh);
    clipSliderHigh.setVisible(false);

    monoSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    monoSlider.setRange(0.0, 100.0, 1.0);
    monoSlider.setValue(audioProcessor.getValue(12));
    monoSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    monoName.setText("STEREO/MONO", dontSendNotification);
    monoName.setJustificationType(Justification::centred);
    monoName.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&monoName);
    monoIndicator.setJustificationType(Justification::centred);
    monoIndicator.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&monoIndicator);
    monoSlider.setLookAndFeel(&monoLookAndFeel);
    monoSlider.addListener(this);
    addAndMakeVisible(&monoSlider);

    mixSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    mixSlider.setRange(0.0, 100.0, 1.0);
    mixSlider.setValue(audioProcessor.getValue(13));
    mixSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    mixName.setText("DRY/WET", dontSendNotification);
    mixName.setJustificationType(Justification::centred);
    mixName.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&mixName);
    mixIndicator.setJustificationType(Justification::centred);
    mixIndicator.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&mixIndicator);
    mixSlider.setLookAndFeel(&mixLookAndFeel);
    mixSlider.addListener(this);
    addAndMakeVisible(&mixSlider);

    crushIndicatorLow.setVisible(false);
    crushIndicatorMid.setVisible(false);
    crushIndicatorHigh.setVisible(false);

    downSampleIndicatorLow.setVisible(false);
    downSampleIndicatorMid.setVisible(false);
    downSampleIndicatorHigh.setVisible(false);

    jitterIndicatorLow.setVisible(false);
    jitterIndicatorMid.setVisible(false);
    jitterIndicatorHigh.setVisible(false);

    clipIndicatorLow.setVisible(false);
    clipIndicatorMid.setVisible(false);
    clipIndicatorHigh.setVisible(false);

    monoLookAndFeel.colourPosition = 90.0 / 100.0 * (monoSlider.getValue());
    mixLookAndFeel.colourPosition = 90.0 / 100.0 * (mixSlider.getValue());

    crushLookAndFeelLow.colourPosition = 90.0 / 100.0 * (crushSliderLow.getValue());
    downSampleLookAndFeelLow.colourPosition = 90.0 / 100.0 * (downSampleSliderLow.getValue());
    jitterLookAndFeelLow.colourPosition = 90.0 / 100.0 * (jitterSliderLow.getValue());
    clipLookAndFeelLow.colourPosition = 90.0 / 15.0 * (clipSliderLow.getValue() + 15);

    crushLookAndFeelMid.colourPosition = 90.0 / 100.0 * (crushSliderMid.getValue());
    downSampleLookAndFeelMid.colourPosition = 90.0 / 100.0 * (downSampleSliderMid.getValue());
    jitterLookAndFeelMid.colourPosition = 90.0 / 100.0 * (jitterSliderMid.getValue());
    clipLookAndFeelMid.colourPosition = 90.0 / 15.0 * (clipSliderMid.getValue() + 15);

    crushLookAndFeelHigh.colourPosition = 90.0 / 100.0 * (crushSliderHigh.getValue());
    downSampleLookAndFeelHigh.colourPosition = 90.0 / 100.0 * (downSampleSliderHigh.getValue());
    jitterLookAndFeelHigh.colourPosition = 90.0 / 100.0 * (jitterSliderHigh.getValue());
    clipLookAndFeelHigh.colourPosition = 90.0 / 15.0 * (clipSliderHigh.getValue() + 15);

    label20Hz.setText("20 Hz", dontSendNotification);
    label20Hz.setJustificationType(Justification::left);
    label20Hz.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&label20Hz);

    label200Hz.setText("200 Hz", dontSendNotification);
    label200Hz.setJustificationType(Justification::centred);
    label200Hz.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&label200Hz);

    label2kHz.setText("2 kHz", dontSendNotification);
    label2kHz.setJustificationType(Justification::centred);
    label2kHz.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&label2kHz);

    label20kHz.setText("20 kHz", dontSendNotification);
    label20kHz.setJustificationType(Justification::right);
    label20kHz.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&label20kHz);
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
    g.drawRoundedRectangle(outputArea, 10, 2);

    drawFrame(g);

    g.setColour(Colours::white);
    g.fillRect(marker200Hz);
    g.fillRect(marker2kHz);
    g.fillRect(divider200Hz);
    g.fillRect(divider2kHz);

    g.setColour(Colour::fromRGBA(0x37, 0x9B, 0xE3, 0x44));
    g.fillRect(bandSelectionArea);
}

void SoundofmusicAudioProcessorEditor::resized()
{
    spectrum.setBounds(40, 40, 800, 180);

    distortionArea.setBounds(40, 290, 500, 190);
    outputArea.setBounds(580, 290, 260, 190);

    crushSliderLow.setBounds(70, 320, 80, 80);
    crushName.setBounds(60, 430, 100, 20);
    crushIndicatorLow.setBounds(60, 430, 100, 20);
    downSampleSliderLow.setBounds(190, 320, 80, 80);
    downSampleName.setBounds(180, 430, 100, 20);
    downSampleIndicatorLow.setBounds(180, 430, 100, 20);
    jitterSliderLow.setBounds(310, 320, 80, 80);
    jitterName.setBounds(300, 430, 100, 20);
    jitterIndicatorLow.setBounds(300, 430, 100, 20);
    clipSliderLow.setBounds(430, 320, 80, 80);
    clipName.setBounds(420, 430, 100, 20);
    clipIndicatorLow.setBounds(420, 430, 100, 20);

    crushSliderMid.setBounds(70, 320, 80, 80);
    crushIndicatorMid.setBounds(60, 430, 100, 20);
    downSampleSliderMid.setBounds(190, 320, 80, 80);
    downSampleIndicatorMid.setBounds(180, 430, 100, 20);
    jitterSliderMid.setBounds(310, 320, 80, 80);
    jitterIndicatorMid.setBounds(300, 430, 100, 20);
    clipSliderMid.setBounds(430, 320, 80, 80);
    clipIndicatorMid.setBounds(420, 430, 100, 20);

    crushSliderHigh.setBounds(70, 320, 80, 80);
    crushIndicatorHigh.setBounds(60, 430, 100, 20);
    downSampleSliderHigh.setBounds(190, 320, 80, 80);
    downSampleIndicatorHigh.setBounds(180, 430, 100, 20);
    jitterSliderHigh.setBounds(310, 320, 80, 80);
    jitterIndicatorHigh.setBounds(300, 430, 100, 20);
    clipSliderHigh.setBounds(430, 320, 80, 80);
    clipIndicatorHigh.setBounds(420, 430, 100, 20);

    monoSlider.setBounds(610, 320, 80, 80);
    monoName.setBounds(600, 430, 100, 20);
    monoIndicator.setBounds(600, 430, 100, 20);
    mixSlider.setBounds(730, 320, 80, 80);
    mixName.setBounds(720, 430, 100, 20);
    mixIndicator.setBounds(720, 430, 100, 20);

    marker200Hz.setBounds(307, 215, 4, 10);
    marker2kHz.setBounds(573, 215, 4, 10);

    divider200Hz.setBounds(308, 40, 2, 180);
    divider2kHz.setBounds(574, 40, 2, 180);

    label20Hz.setBounds(35, 230, 60, 20);
    label200Hz.setBounds(277, 230, 60, 20);
    label2kHz.setBounds(543, 230, 60, 20);
    label20kHz.setBounds(785, 230, 60, 20);

    bandSelectionArea.setBounds(40, 40, 268, 180);
}

void SoundofmusicAudioProcessorEditor::sliderValueChanged(Slider* slider) {
    String temp;

    if (slider == &crushSliderLow) {
        crushLookAndFeelLow.colourPosition = 90.0 / 100.0 * (crushSliderLow.getValue());
        temp = String(crushSliderLow.getValue()) + " %";
        crushIndicatorLow.setText(temp, dontSendNotification);
    }
    else if (slider == &crushSliderMid) {
        crushLookAndFeelMid.colourPosition = 90.0 / 100.0 * (crushSliderMid.getValue());
        temp = String(crushSliderMid.getValue()) + " %";
        crushIndicatorMid.setText(temp, dontSendNotification);
    }
    else if (slider == &crushSliderHigh) {
        crushLookAndFeelHigh.colourPosition = 90.0 / 100.0 * (crushSliderHigh.getValue());
        temp = String(crushSliderHigh.getValue()) + " %";
        crushIndicatorHigh.setText(temp, dontSendNotification);
    }

    else if (slider == &downSampleSliderLow) {
        downSampleLookAndFeelLow.colourPosition = 90.0 / 100.0 * (downSampleSliderLow.getValue());
        temp = String(downSampleSliderLow.getValue()) + " %";
        downSampleIndicatorLow.setText(temp, dontSendNotification);
    }
    else if (slider == &downSampleSliderMid) {
        downSampleLookAndFeelMid.colourPosition = 90.0 / 100.0 * (downSampleSliderMid.getValue());
        temp = String(downSampleSliderMid.getValue()) + " %";
        downSampleIndicatorMid.setText(temp, dontSendNotification);
    }
    else if (slider == &downSampleSliderHigh) {
        downSampleLookAndFeelHigh.colourPosition = 90.0 / 100.0 * (downSampleSliderHigh.getValue());
        temp = String(downSampleSliderHigh.getValue()) + " %";
        downSampleIndicatorHigh.setText(temp, dontSendNotification);
    }

    else if (slider == &jitterSliderLow) {
        jitterLookAndFeelLow.colourPosition = 90.0 / 100.0 * (jitterSliderLow.getValue());
        temp = String(jitterSliderLow.getValue()) + " %";
        jitterIndicatorLow.setText(temp, dontSendNotification);
    }
    else if (slider == &jitterSliderMid) {
        jitterLookAndFeelMid.colourPosition = 90.0 / 100.0 * (jitterSliderMid.getValue());
        temp = String(jitterSliderMid.getValue()) + " %";
        jitterIndicatorMid.setText(temp, dontSendNotification);
    }
    else if (slider == &jitterSliderHigh) {
        jitterLookAndFeelHigh.colourPosition = 90.0 / 100.0 * (jitterSliderHigh.getValue());
        temp = String(jitterSliderHigh.getValue()) + " %";
        jitterIndicatorHigh.setText(temp, dontSendNotification);
    }

    else if (slider == &clipSliderLow) {
        clipLookAndFeelLow.colourPosition = 90.0 / 15.0 * (clipSliderLow.getValue() + 15);
        temp = String(clipSliderLow.getValue()) + " dB";
        clipIndicatorLow.setText(temp, dontSendNotification);
    }
    else if (slider == &clipSliderMid) {
        clipLookAndFeelMid.colourPosition = 90.0 / 15.0 * (clipSliderMid.getValue() + 15);
        temp = String(clipSliderMid.getValue()) + " dB";
        clipIndicatorMid.setText(temp, dontSendNotification);
    }
    else if (slider == &clipSliderHigh) {
        clipLookAndFeelHigh.colourPosition = 90.0 / 15.0 * (clipSliderHigh.getValue() + 15);
        temp = String(clipSliderHigh.getValue()) + " dB";
        clipIndicatorHigh.setText(temp, dontSendNotification);
    }

    else if (slider == &monoSlider) {
        monoLookAndFeel.colourPosition = 90.0 / 100.0 * (monoSlider.getValue());
        temp = String(monoSlider.getValue()) + " %";
        monoIndicator.setText(temp, dontSendNotification);
    }
    else if (slider == &mixSlider) {
        mixLookAndFeel.colourPosition = 90.0 / 100.0 * (mixSlider.getValue());
        temp = String(mixSlider.getValue()) + " dB";
        mixIndicator.setText(temp, dontSendNotification);
    }
}

void SoundofmusicAudioProcessorEditor::sliderDragStarted(Slider* slider) {
    if (slider == &crushSliderLow) {
        crushName.setVisible(false);
        crushIndicatorLow.setVisible(true);
    }
    else if (slider == &crushSliderMid) {
        crushName.setVisible(false);
        crushIndicatorMid.setVisible(true);
    }
    else if (slider == &crushSliderHigh) {
        crushName.setVisible(false);
        crushIndicatorHigh.setVisible(true);
    }

    else if (slider == &downSampleSliderLow) {
        downSampleName.setVisible(false);
        downSampleIndicatorLow.setVisible(true);
    }
    else if (slider == &downSampleSliderMid) {
        downSampleName.setVisible(false);
        downSampleIndicatorMid.setVisible(true);
    }
    else if (slider == &downSampleSliderHigh) {
        downSampleName.setVisible(false);
        downSampleIndicatorHigh.setVisible(true);
    }

    else if (slider == &jitterSliderLow) {
        jitterName.setVisible(false);
        jitterIndicatorLow.setVisible(true);
    }
    else if (slider == &jitterSliderMid) {
        jitterName.setVisible(false);
        jitterIndicatorMid.setVisible(true);
    }
    else if (slider == &jitterSliderHigh) {
        jitterName.setVisible(false);
        jitterIndicatorHigh.setVisible(true);
    }

    else if (slider == &clipSliderLow) {
        clipName.setVisible(false);
        clipIndicatorLow.setVisible(true);
    }
    else if (slider == &clipSliderMid) {
        clipName.setVisible(false);
        clipIndicatorMid.setVisible(true);
    }
    else if (slider == &clipSliderHigh) {
        clipName.setVisible(false);
        clipIndicatorHigh.setVisible(true);
    }

    else if (slider == &monoSlider) {
        monoName.setVisible(false);
        monoIndicator.setVisible(true);
    }
    else if (slider == &mixSlider) {
        mixName.setVisible(false);
        mixIndicator.setVisible(true);
    }
}

void SoundofmusicAudioProcessorEditor::sliderDragEnded(Slider* slider) {
    if (slider == &crushSliderLow) {
        crushName.setVisible(true);
        crushIndicatorLow.setVisible(false);
    }
    else if (slider == &crushSliderMid) {
        crushName.setVisible(true);
        crushIndicatorMid.setVisible(false);
    }
    else if (slider == &crushSliderHigh) {
        crushName.setVisible(true);
        crushIndicatorHigh.setVisible(false);
    }

    else if (slider == &downSampleSliderLow) {
        downSampleName.setVisible(true);
        downSampleIndicatorLow.setVisible(false);
    }
    else if (slider == &downSampleSliderMid) {
        downSampleName.setVisible(true);
        downSampleIndicatorMid.setVisible(false);
    }
    else if (slider == &downSampleSliderHigh) {
        downSampleName.setVisible(true);
        downSampleIndicatorHigh.setVisible(false);
    }

    else if (slider == &jitterSliderLow) {
        jitterName.setVisible(true);
        jitterIndicatorLow.setVisible(false);
    }
    else if (slider == &jitterSliderMid) {
        jitterName.setVisible(true);
        jitterIndicatorMid.setVisible(false);
    }
    else if (slider == &jitterSliderHigh) {
        jitterName.setVisible(true);
        jitterIndicatorHigh.setVisible(false);
    }

    else if (slider == &clipSliderLow) {
        clipName.setVisible(true);
        clipIndicatorLow.setVisible(false);
    }
    else if (slider == &clipSliderMid) {
        clipName.setVisible(true);
        clipIndicatorMid.setVisible(false);
    }
    else if (slider == &clipSliderHigh) {
        clipName.setVisible(true);
        clipIndicatorHigh.setVisible(false);
    }

    else if (slider == &monoSlider) {
        monoName.setVisible(true);
        monoIndicator.setVisible(false);
    }
    else if (slider == &mixSlider) {
        mixName.setVisible(true);
        mixIndicator.setVisible(false);
    }
}

void SoundofmusicAudioProcessorEditor::drawNextFrameOfInSpectrum()
{
    window.multiplyWithWindowingTable(audioProcessor.fftDataIn, audioProcessor.fftSize);

    forwardFFTIn.performFrequencyOnlyForwardTransform(audioProcessor.fftDataIn);

    auto mindB = -100.0f;
    auto maxdB = 0.0f;

    for (int i = 0; i < audioProcessor.scopeSize; ++i)
    {
        auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - (float)i / (float)audioProcessor.scopeSize) * 0.2f);
        auto fftDataIndex = juce::jlimit(0, audioProcessor.fftSize / 2, (int)(skewedProportionX * (float)audioProcessor.fftSize * 0.5f));
        auto level = juce::jmap(juce::jlimit(mindB, maxdB, juce::Decibels::gainToDecibels(audioProcessor.fftDataIn[fftDataIndex])
            - juce::Decibels::gainToDecibels((float)audioProcessor.fftSize)),
            mindB, maxdB, 0.0f, 1.0f);

        audioProcessor.scopeDataIn[i] = level;
    }
}

void SoundofmusicAudioProcessorEditor::drawNextFrameOfOutSpectrum()
{
    window.multiplyWithWindowingTable(audioProcessor.fftDataOut, audioProcessor.fftSize);

    forwardFFTOut.performFrequencyOnlyForwardTransform(audioProcessor.fftDataOut);

    auto mindB = -100.0f;
    auto maxdB = 0.0f;

    for (int i = 0; i < audioProcessor.scopeSize; ++i)
    {
        auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - (float)i / (float)audioProcessor.scopeSize) * 0.2f);
        auto fftDataIndex = juce::jlimit(0, audioProcessor.fftSize / 2, (int)(skewedProportionX * (float)audioProcessor.fftSize * 0.5f));
        auto level = juce::jmap(juce::jlimit(mindB, maxdB, juce::Decibels::gainToDecibels(audioProcessor.fftDataOut[fftDataIndex])
            - juce::Decibels::gainToDecibels((float)audioProcessor.fftSize)),
            mindB, maxdB, 0.0f, 1.0f);

        audioProcessor.scopeDataOut[i] = level;
    }
}

void SoundofmusicAudioProcessorEditor::timerCallback()
{
    if (audioProcessor.nextFFTBlockReadyIn)
    {
        drawNextFrameOfInSpectrum();
        drawNextFrameOfOutSpectrum();
        audioProcessor.nextFFTBlockReadyIn = false;
        audioProcessor.nextFFTBlockReadyOut = false;
        repaint();
    }
}

void SoundofmusicAudioProcessorEditor::drawFrame(juce::Graphics& g)
{
    ColourGradient gradientOutSpectrum(Colour::fromRGB(0x17, 0x7B, 0xC3), 40, 40, Colour::fromRGB(0xAB, 0x06, 0x4D), 40, 220, false);
    ColourGradient gradientInSpectrum(Colour::fromRGB(0x37, 0x9B, 0xE3), 40, 40, Colour::fromRGB(0xCB, 0x16, 0x6D), 40, 220, false);

    for (int i = 5; i < audioProcessor.scopeSize - 5; ++i)
    {
        audioProcessor.scopeDataIn[i] = audioProcessor.scopeDataIn[i - 4] + audioProcessor.scopeDataIn[i - 3] + audioProcessor.scopeDataIn[i - 2] + audioProcessor.scopeDataIn[i - 1] + audioProcessor.scopeDataIn[i] + audioProcessor.scopeDataIn[i + 1] + audioProcessor.scopeDataIn[i + 2] + audioProcessor.scopeDataIn[i + 3] + audioProcessor.scopeDataIn[i + 4];
        audioProcessor.scopeDataIn[i] /= 9.0;

        audioProcessor.scopeDataOut[i] = audioProcessor.scopeDataOut[i - 4] + audioProcessor.scopeDataOut[i - 3] + audioProcessor.scopeDataOut[i - 2] + audioProcessor.scopeDataOut[i - 1] + audioProcessor.scopeDataOut[i] + audioProcessor.scopeDataOut[i + 1] + audioProcessor.scopeDataOut[i + 2] + audioProcessor.scopeDataOut[i + 3] + audioProcessor.scopeDataOut[i + 4];
        audioProcessor.scopeDataOut[i] /= 9.0;
    }

    for (int i = 1; i < audioProcessor.scopeSize; ++i)
    {
        
        g.setGradientFill(gradientOutSpectrum);

        g.drawLine({ (float)juce::jmap(i - 1, 0, audioProcessor.scopeSize - 1, 40, 840),
                              220.0,
                      (float)juce::jmap(i,     0, audioProcessor.scopeSize - 1, 40, 840),
                              juce::jmap(audioProcessor.scopeDataOut[i],     0.0f, 1.0f, 220.0f, 40.0f) });

        
        g.setGradientFill(gradientInSpectrum);

        g.drawLine({ (float)juce::jmap(i - 1, 0, audioProcessor.scopeSize - 1, 40, 840),
                              220.0,
                      (float)juce::jmap(i,     0, audioProcessor.scopeSize - 1, 40, 840),
                              juce::jmap(audioProcessor.scopeDataIn[i],     0.0f, 1.0f, 220.0f, 40.0f) });
    }
}

void SoundofmusicAudioProcessorEditor::mouseDown(const MouseEvent& event) {
    if (event.eventComponent != &spectrum) {
        return;
    }
    
    if (event.getMouseDownX() > 40 && event.getMouseDownX() <= 267) {
        bandSelected(0);
        crushSliderLow.setVisible(true);
        crushSliderMid.setVisible(false);
        crushSliderHigh.setVisible(false);

        downSampleSliderLow.setVisible(true);
        downSampleSliderMid.setVisible(false);
        downSampleSliderHigh.setVisible(false);

        jitterSliderLow.setVisible(true);
        jitterSliderMid.setVisible(false);
        jitterSliderHigh.setVisible(false);

        clipSliderLow.setVisible(true);
        clipSliderMid.setVisible(false);
        clipSliderHigh.setVisible(false);
    }
    else if (event.getMouseDownX() > 267 && event.getMouseDownX() <= 533) {
        bandSelected(1);
        crushSliderLow.setVisible(false);
        crushSliderMid.setVisible(true);
        crushSliderHigh.setVisible(false);

        downSampleSliderLow.setVisible(false);
        downSampleSliderMid.setVisible(true);
        downSampleSliderHigh.setVisible(false);

        jitterSliderLow.setVisible(false);
        jitterSliderMid.setVisible(true);
        jitterSliderHigh.setVisible(false);

        clipSliderLow.setVisible(false);
        clipSliderMid.setVisible(true);
        clipSliderHigh.setVisible(false);
    }
    else if (event.getMouseDownX() > 533 && event.getMouseDownX() <= 900) {
        bandSelected(2);
        crushSliderLow.setVisible(false);
        crushSliderMid.setVisible(false);
        crushSliderHigh.setVisible(true);

        downSampleSliderLow.setVisible(false);
        downSampleSliderMid.setVisible(false);
        downSampleSliderHigh.setVisible(true);

        jitterSliderLow.setVisible(false);
        jitterSliderMid.setVisible(false);
        jitterSliderHigh.setVisible(true);

        clipSliderLow.setVisible(false);
        clipSliderMid.setVisible(false);
        clipSliderHigh.setVisible(true);
    }
}

void SoundofmusicAudioProcessorEditor::bandSelected(int band)
{
    if (band == 0) {
        bandSelectionArea.setBounds(40, 40, 268, 180);
    }
    else if (band == 1) {
        bandSelectionArea.setBounds(310, 40, 264, 180);
    }
    else if (band == 2) {
        bandSelectionArea.setBounds(576, 40, 264, 180);
    }
}