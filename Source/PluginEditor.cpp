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

    divider1Value = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, FREQ1_ID, divider1Slider);
    divider2Value = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, FREQ2_ID, divider2Slider);

    spectrum.addMouseListener(this, false);
    addAndMakeVisible(spectrum);

    crushSliderLow.setSliderStyle(Slider::RotaryVerticalDrag);
    crushSliderLow.setRange(0.0, 100.0, 1.0);
    crushSliderLow.setValue(audioProcessor.getValue(0));
    crushSliderLow.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    crushName.setText("Quantization", dontSendNotification);
    crushName.setJustificationType(Justification::centred);
    crushName.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
    addAndMakeVisible(&crushName);
    crushIndicatorLow.setJustificationType(Justification::centred);
    crushIndicatorLow.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
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
    crushIndicatorMid.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
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
    crushIndicatorHigh.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
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
    downSampleName.setText("Downsampling", dontSendNotification);
    downSampleName.setJustificationType(Justification::centred);
    downSampleName.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
    addAndMakeVisible(&downSampleName);
    downSampleIndicatorLow.setJustificationType(Justification::centred);
    downSampleIndicatorLow.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
    addAndMakeVisible(&downSampleIndicatorLow);
    downSampleSliderLow.setLookAndFeel(&downSampleLookAndFeelLow);
    downSampleSliderLow.addListener(this);
    addAndMakeVisible(&downSampleSliderLow);

    downSampleSliderMid.setSliderStyle(Slider::RotaryVerticalDrag);
    downSampleSliderMid.setRange(0.0, 100.0, 1.0);
    downSampleSliderMid.setValue(audioProcessor.getValue(5));
    downSampleSliderMid.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    downSampleIndicatorMid.setJustificationType(Justification::centred);
    downSampleIndicatorMid.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
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
    downSampleIndicatorHigh.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
    addAndMakeVisible(&downSampleIndicatorHigh);
    downSampleSliderHigh.setLookAndFeel(&downSampleLookAndFeelHigh);
    downSampleSliderHigh.addListener(this);
    addAndMakeVisible(&downSampleSliderHigh);
    downSampleSliderHigh.setVisible(false);

    jitterSliderLow.setSliderStyle(Slider::RotaryVerticalDrag);
    jitterSliderLow.setRange(0.0, 100.0, 1.0);
    jitterSliderLow.setValue(audioProcessor.getValue(2));
    jitterSliderLow.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    jitterName.setText("Error", dontSendNotification);
    jitterName.setJustificationType(Justification::centred);
    jitterName.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
    addAndMakeVisible(&jitterName);
    jitterIndicatorLow.setJustificationType(Justification::centred);
    jitterIndicatorLow.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
    addAndMakeVisible(&jitterIndicatorLow);
    jitterSliderLow.setLookAndFeel(&jitterLookAndFeelLow);
    jitterSliderLow.addListener(this);
    addAndMakeVisible(&jitterSliderLow);

    jitterSliderMid.setSliderStyle(Slider::RotaryVerticalDrag);
    jitterSliderMid.setRange(0.0, 100.0, 1.0);
    jitterSliderMid.setValue(audioProcessor.getValue(6));
    jitterSliderMid.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    jitterIndicatorMid.setJustificationType(Justification::centred);
    jitterIndicatorMid.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
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
    jitterIndicatorHigh.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
    addAndMakeVisible(&jitterIndicatorHigh);
    jitterSliderHigh.setLookAndFeel(&jitterLookAndFeelHigh);
    jitterSliderHigh.addListener(this);
    addAndMakeVisible(&jitterSliderHigh);
    jitterSliderHigh.setVisible(false);

    clipSliderLow.setSliderStyle(Slider::RotaryVerticalDrag);
    clipSliderLow.setRange(-15.0, 0.0, 0.1);
    clipSliderLow.setValue(audioProcessor.getValue(3));
    clipSliderLow.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    clipName.setText("Clipping", dontSendNotification);
    clipName.setJustificationType(Justification::centred);
    clipName.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
    addAndMakeVisible(&clipName);
    clipIndicatorLow.setJustificationType(Justification::centred);
    clipIndicatorLow.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
    addAndMakeVisible(&clipIndicatorLow);
    clipSliderLow.setLookAndFeel(&clipLookAndFeelLow);
    clipSliderLow.addListener(this);
    addAndMakeVisible(&clipSliderLow);

    clipSliderMid.setSliderStyle(Slider::RotaryVerticalDrag);
    clipSliderMid.setRange(-15.0, 0.0, 0.1);
    clipSliderMid.setValue(audioProcessor.getValue(7));
    clipSliderMid.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    clipIndicatorMid.setJustificationType(Justification::centred);
    clipIndicatorMid.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
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
    clipIndicatorHigh.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
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
    monoName.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
    addAndMakeVisible(&monoName);
    monoIndicator.setJustificationType(Justification::centred);
    monoIndicator.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
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
    mixName.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
    addAndMakeVisible(&mixName);
    mixIndicator.setJustificationType(Justification::centred);
    mixIndicator.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
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

    label0.setText("20 Hz", dontSendNotification);
    label0.setJustificationType(Justification::left);
    label0.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
    addAndMakeVisible(&label0);

    label1.setText("500 Hz", dontSendNotification);
    label1.setJustificationType(Justification::centred);
    label1.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
    addAndMakeVisible(&label1);

    label2.setText("2 kHz", dontSendNotification);
    label2.setJustificationType(Justification::centred);
    label2.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
    addAndMakeVisible(&label2);

    label3.setText("20 kHz", dontSendNotification);
    label3.setJustificationType(Justification::right);
    label3.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
    addAndMakeVisible(&label3);

    divider1Slider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    divider1Slider.setRange(20.0, 20000.0, 1.0);
    divider1Slider.setValue((int)audioProcessor.getValue(14));
    divider1Slider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    divider1Slider.setSkewFactorFromMidPoint(1000.0);
    divider1Slider.setLookAndFeel(&divider1LookAndFeel);
    divider1Slider.addListener(this);
    addAndMakeVisible(&divider1Slider);

    divider2Slider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    divider2Slider.setRange(20.0, 20000.0, 1.0);
    divider2Slider.setValue((int)audioProcessor.getValue(15));
    divider2Slider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    divider2Slider.setSkewFactorFromMidPoint(1000.0);
    divider2Slider.setLookAndFeel(&divider2LookAndFeel);
    divider2Slider.addListener(this);
    addAndMakeVisible(&divider2Slider);

    freq1Name.setText("FREQ 1", dontSendNotification);
    freq1Name.setJustificationType(Justification::centred);
    freq1Name.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
    addAndMakeVisible(&freq1Name);

    freq2Name.setText("FREQ 2", dontSendNotification);
    freq2Name.setJustificationType(Justification::centred);
    freq2Name.setColour(Label::textColourId, Colour::fromRGB(0x30, 0xE0, 0x30));
    addAndMakeVisible(&freq2Name);

    crushName.setFont(Font(customFont).withHeight(16));
    downSampleName.setFont(Font(customFont).withHeight(16));
    jitterName.setFont(Font(customFont).withHeight(16));
    clipName.setFont(Font(customFont).withHeight(16));
    monoName.setFont(Font(customFont).withHeight(16));
    mixName.setFont(Font(customFont).withHeight(16));
    freq1Name.setFont(Font(customFont).withHeight(16));
    freq2Name.setFont(Font(customFont).withHeight(16));

    crushIndicatorLow.setFont(Font(customFont).withHeight(20));
    downSampleIndicatorLow.setFont(Font(customFont).withHeight(20));
    jitterIndicatorLow.setFont(Font(customFont).withHeight(20));
    clipIndicatorLow.setFont(Font(customFont).withHeight(20));

    crushIndicatorMid.setFont(Font(customFont).withHeight(20));
    downSampleIndicatorMid.setFont(Font(customFont).withHeight(20));
    jitterIndicatorMid.setFont(Font(customFont).withHeight(20));
    clipIndicatorMid.setFont(Font(customFont).withHeight(20));

    crushIndicatorHigh.setFont(Font(customFont).withHeight(20));
    downSampleIndicatorHigh.setFont(Font(customFont).withHeight(20));
    jitterIndicatorHigh.setFont(Font(customFont).withHeight(20));
    clipIndicatorHigh.setFont(Font(customFont).withHeight(20));

    monoIndicator.setFont(Font(customFont).withHeight(20));
    mixIndicator.setFont(Font(customFont).withHeight(20));

    label0.setFont(Font(customFont).withHeight(15));
    label1.setFont(Font(customFont).withHeight(15));
    label2.setFont(Font(customFont).withHeight(15));
    label3.setFont(Font(customFont).withHeight(15));
}

SoundofmusicAudioProcessorEditor::~SoundofmusicAudioProcessorEditor()
{
}

//==============================================================================
void SoundofmusicAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setColour(Colour::fromRGB(0x20, 0x20, 0x40));
    g.fillAll();

    g.setColour(Colour::fromRGB(0x30, 0xC0, 0x30));

    drawFrame(g);

    g.setColour(Colours::white);
    g.fillRect(marker1);
    g.fillRect(marker2);
    g.fillRect(divider1);
    g.fillRect(divider2);

    g.setColour(Colour::fromRGBA(0xE0, 0xE0, 0xE0, 0x50));
    g.fillRect(bandSelectionArea);
}

void SoundofmusicAudioProcessorEditor::resized()
{
    spectrum.setBounds(40, 40, 500, 180);

    dividerArea.setBounds(580, 40, 260, 180);
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

    x = mapToLog10((19980 - (frequency1 - 20.0)) / 19980.0, 40.0, 540.0);
    x = mapToLog10((x - 40) / 500.0, 40.0, 540.0);
    divider1Pos = 580 - x;

    x = mapToLog10((19980 - (frequency2 - 20.0)) / 19980.0, 40.0, 540.0);
    x = mapToLog10((x - 40) / 500.0, 40.0, 540.0);
    divider2Pos = 580 - x;

    marker1.setBounds(divider1Pos - 1, 215, 4, 10);
    marker2.setBounds(divider2Pos - 1, 215, 4, 10);

    divider1.setBounds(divider1Pos, 40, 2, 180);
    divider2.setBounds(divider2Pos, 40, 2, 180);

    label0.setBounds(35, 230, 60, 20);
    label1.setBounds(divider1Pos - 31, 230, 60, 20);
    label2.setBounds(divider2Pos - 31, 230, 60, 20);
    label3.setBounds(485, 230, 60, 20);

    divider1Slider.setBounds(610, 70, 80, 80);
    divider2Slider.setBounds(730, 70, 80, 80);

    bandSelectionArea.setBounds(40, 40, divider1Pos - 40, 180);

    freq1Name.setBounds(600, 180, 100, 20);
    freq2Name.setBounds(720, 180, 100, 20);
}

void SoundofmusicAudioProcessorEditor::sliderValueChanged(Slider* slider) {
    String temp;

    if (slider == &crushSliderLow) {
        temp = String(crushSliderLow.getValue()) + " %";
        crushIndicatorLow.setText(temp, dontSendNotification);
    }
    else if (slider == &crushSliderMid) {
        temp = String(crushSliderMid.getValue()) + " %";
        crushIndicatorMid.setText(temp, dontSendNotification);
    }
    else if (slider == &crushSliderHigh) {
        temp = String(crushSliderHigh.getValue()) + " %";
        crushIndicatorHigh.setText(temp, dontSendNotification);
    }

    else if (slider == &downSampleSliderLow) {
        temp = String(downSampleSliderLow.getValue()) + " %";
        downSampleIndicatorLow.setText(temp, dontSendNotification);
    }
    else if (slider == &downSampleSliderMid) {
        temp = String(downSampleSliderMid.getValue()) + " %";
        downSampleIndicatorMid.setText(temp, dontSendNotification);
    }
    else if (slider == &downSampleSliderHigh) {
        temp = String(downSampleSliderHigh.getValue()) + " %";
        downSampleIndicatorHigh.setText(temp, dontSendNotification);
    }

    else if (slider == &jitterSliderLow) {
        temp = String(jitterSliderLow.getValue()) + " %";
        jitterIndicatorLow.setText(temp, dontSendNotification);
    }
    else if (slider == &jitterSliderMid) {
        temp = String(jitterSliderMid.getValue()) + " %";
        jitterIndicatorMid.setText(temp, dontSendNotification);
    }
    else if (slider == &jitterSliderHigh) {
        temp = String(jitterSliderHigh.getValue()) + " %";
        jitterIndicatorHigh.setText(temp, dontSendNotification);
    }

    else if (slider == &clipSliderLow) {
        temp = String(clipSliderLow.getValue()) + " dB";
        clipIndicatorLow.setText(temp, dontSendNotification);
    }
    else if (slider == &clipSliderMid) {
        temp = String(clipSliderMid.getValue()) + " dB";
        clipIndicatorMid.setText(temp, dontSendNotification);
    }
    else if (slider == &clipSliderHigh) {
        temp = String(clipSliderHigh.getValue()) + " dB";
        clipIndicatorHigh.setText(temp, dontSendNotification);
    }

    else if (slider == &monoSlider) {
        temp = String(monoSlider.getValue()) + " %";
        monoIndicator.setText(temp, dontSendNotification);
    }
    else if (slider == &mixSlider) {
        temp = String(mixSlider.getValue()) + " dB";
        mixIndicator.setText(temp, dontSendNotification);
    }

    else if (slider == &divider1Slider) {
        

        if (divider1Slider.getValue() > frequency2 - 100) {
            divider1Slider.setValue(frequency2 - 100);
            divider1Slider.setEnabled(false);
        }

        if (divider1Slider.getValue() <= 200.0) {
            label0.setVisible(false);
        }
        else {
            label0.setVisible(true);
        }

        frequency1 = divider1Slider.getValue();
        x = mapToLog10((19980 - (frequency1 - 20.0)) / 19980.0, 40.0, 540.0);
        x = mapToLog10((x - 40) / 500.0, 40.0, 540.0);
        divider1Pos = 580 - x;

        if (divider1Pos >= divider2Pos - 50) {
            label1.setVisible(true);
            label2.setVisible(false);
        }
        else {
            label1.setVisible(true);
            label2.setVisible(true);
        }

        if (divider1Slider.getValue() >= 1000.0) {
            temp = String((float)divider1Slider.getValue() / 1000.0) + " kHz";
            label1.setText(temp, dontSendNotification);
        }
        else {
            temp = String((int)divider1Slider.getValue()) + " Hz";
            label1.setText(temp, dontSendNotification);
        }

        marker1.setBounds(divider1Pos - 1, 215, 4, 10);
        divider1.setBounds(divider1Pos, 40, 2, 180);
        label1.setBounds(divider1Pos - 41, 230, 60, 20);

        if (band == 0) {
            bandSelectionArea.setBounds(40, 40, divider1Pos - 40, 180);
        }
        else if (band == 1) {
            bandSelectionArea.setBounds(divider1Pos, 40, divider2Pos - divider1Pos, 180);
        }
        else if (band == 2) {
            bandSelectionArea.setBounds(divider2Pos, 40, 840 - divider2Pos, 180);
        }
    }
    else if (slider == &divider2Slider) {
        if (divider2Slider.getValue() < frequency1 + 100) {
            divider2Slider.setValue(frequency1 + 100);
            divider2Slider.setEnabled(false);
        }

        if (divider2Slider.getValue() >= 8000.0) {
            label3.setVisible(false);
        }
        else {
            label3.setVisible(true);
        }

        frequency2 = divider2Slider.getValue();
        x = mapToLog10((19980 - (frequency2 - 20.0)) / 19980.0, 40.0, 540.0);
        x = mapToLog10((x - 40) / 500.0, 40.0, 540.0);
        divider2Pos = 580 - x;

        if (divider2Pos <= divider1Pos + 50) {
            label1.setVisible(false);
            label2.setVisible(true);
        }
        else {
            label1.setVisible(true);
            label2.setVisible(true);
        }
        
        if (divider2Slider.getValue() >= 1000.0) {
            temp = String((float)divider2Slider.getValue() / 1000.0) + " kHz";
            label2.setText(temp, dontSendNotification);
        }
        else {
            temp = String((int)divider2Slider.getValue()) + " Hz";
            label2.setText(temp, dontSendNotification);
        }

        marker2.setBounds(divider2Pos - 1, 215, 4, 10);
        divider2.setBounds(divider2Pos, 40, 2, 180);
        label2.setBounds(divider2Pos - 41, 230, 60, 20);

        if (band == 0) {
            bandSelectionArea.setBounds(40, 40, divider1Pos - 40, 180);
        }
        else if (band == 1) {
            bandSelectionArea.setBounds(divider1Pos, 40, divider2Pos - divider1Pos, 180);
        }
        else if (band == 2) {
            bandSelectionArea.setBounds(divider2Pos, 40, spectrum.getRight() - divider2Pos, 180);
        }
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

    else if (slider == &divider1Slider) {
        divider1Slider.setEnabled(true);
    }
    else if (slider == &divider2Slider) {
        divider2Slider.setEnabled(true);
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
    for (int i = 1; i < audioProcessor.scopeSize; ++i)
    {
        
        g.setColour(Colour::fromRGB(0x30, 0xE0, 0x30));

        g.drawLine({ (float)juce::jmap(i - 1, 0, audioProcessor.scopeSize - 1, 40, 540),
                              220.0,
                      (float)juce::jmap(i,     0, audioProcessor.scopeSize - 1, 40, 540),
                              juce::jmap(audioProcessor.scopeDataOut[i],     0.0f, 1.0f, 220.0f, 40.0f) });

        
        g.setColour(Colour::fromRGBA(0x30, 0xE0, 0x30, 0x50));

        g.drawLine({ (float)juce::jmap(i - 1, 0, audioProcessor.scopeSize - 1, 40, 540),
                              220.0,
                      (float)juce::jmap(i,     0, audioProcessor.scopeSize - 1, 40, 540),
                              juce::jmap(audioProcessor.scopeDataIn[i],     0.0f, 1.0f, 220.0f, 40.0f) });
    }
}

void SoundofmusicAudioProcessorEditor::mouseDown(const MouseEvent& event) {
    if (event.eventComponent != &spectrum) {
        return;
    }
    
    if (event.getMouseDownX() > 0 && event.getMouseDownX() <= divider1Pos - 40) {
        band = 0;
        bandSelected();
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
    else if (event.getMouseDownX() > divider1Pos - 40 && event.getMouseDownX() <= divider2Pos - 40) {
        band = 1;
        bandSelected();
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
    else if (event.getMouseDownX() > divider2Pos - 40 && event.getMouseDownX() <= 600) {
        band = 2;
        bandSelected();
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

void SoundofmusicAudioProcessorEditor::bandSelected()
{
    if (band == 0) {
        bandSelectionArea.setBounds(40, 40, divider1Pos - 40, 180);
    }
    else if (band == 1) {
        bandSelectionArea.setBounds(divider1Pos, 40, divider2Pos - divider1Pos, 180);
    }
    else if (band == 2) {
        bandSelectionArea.setBounds(divider2Pos, 40, 540 - divider2Pos, 180);
    }
}