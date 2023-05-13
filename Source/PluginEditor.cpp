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
    startTimerHz(30);

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
    monoName.setText("STEREO/MONO", dontSendNotification);
    monoName.setJustificationType(Justification::centred);
    monoName.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&monoName);
    monoIndicator.setJustificationType(Justification::centred);
    monoIndicator.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&monoIndicator);
    monoSlider.setLookAndFeel(&monoLookAndFeel);
    monoLookAndFeel.colourPosition = 90.0 / 100.0 * (monoSlider.getValue());
    monoSlider.addListener(this);
    addAndMakeVisible(&monoSlider);

    mixSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    mixSlider.setRange(0.0, 100.0, 1.0);
    mixSlider.setValue(audioProcessor.getValue(6));
    mixSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    mixName.setText("DRY/WET", dontSendNotification);
    mixName.setJustificationType(Justification::centred);
    mixName.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&mixName);
    mixIndicator.setJustificationType(Justification::centred);
    mixIndicator.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&mixIndicator);
    mixSlider.setLookAndFeel(&mixLookAndFeel);
    mixLookAndFeel.colourPosition = 90.0 / 100.0 * (mixSlider.getValue());
    mixSlider.addListener(this);
    addAndMakeVisible(&mixSlider);

    crushLookAndFeel.colourPosition = 90.0 / 100.0 * (crushSlider.getValue());
    downSampleLookAndFeel.colourPosition = 90.0 / 100.0 * (downSampleSlider.getValue());
    jitterLookAndFeel.colourPosition = 90.0 / 100.0 * (jitterSlider.getValue());
    clipLookAndFeel.colourPosition = 90.0 / 15.0 * (clipSlider.getValue() + 15);
    monoLookAndFeel.colourPosition = 90.0 / 100.0 * (monoSlider.getValue());
    mixLookAndFeel.colourPosition = 90.0 / 100.0 * (mixSlider.getValue());
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
}

void SoundofmusicAudioProcessorEditor::resized()
{
    spectrum.setBounds(40, 40, 840, 260);
    spectrumArea.setBounds(40, 40, 840, 220);

    distortionArea.setBounds(40, 290, 500, 190);
    outputArea.setBounds(580, 290, 260, 190);

    crushSlider.setBounds(70, 320, 80, 80);
    crushName.setBounds(60, 430, 100, 20);
    crushIndicator.setBounds(60, 430, 100, 20);
    downSampleSlider.setBounds(190, 320, 80, 80);
    downSampleName.setBounds(180, 430, 100, 20);
    downSampleIndicator.setBounds(180, 430, 100, 20);
    jitterSlider.setBounds(310, 320, 80, 80);
    jitterName.setBounds(300, 430, 100, 20);
    jitterIndicator.setBounds(300, 430, 100, 20);
    clipSlider.setBounds(430, 320, 80, 80);
    clipName.setBounds(420, 430, 100, 20);
    clipIndicator.setBounds(420, 430, 100, 20);

    monoSlider.setBounds(610, 310, 80, 80);
    monoName.setBounds(600, 420, 100, 20);
    monoIndicator.setBounds(600, 420, 100, 20);
    mixSlider.setBounds(730, 310, 80, 80);
    mixName.setBounds(720, 420, 100, 20);
    mixIndicator.setBounds(720, 420, 100, 20);
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
        monoName.setVisible(false);
        monoIndicator.setVisible(true);
    }
    else if (slider == &mixSlider) {
        mixName.setVisible(false);
        mixIndicator.setVisible(true);
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