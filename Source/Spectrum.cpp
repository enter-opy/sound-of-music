/*
  ==============================================================================

    Spectrum.cpp
    Created: 12 May 2023 6:44:42pm
    Author:  vivek

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Spectrum.h"

//==============================================================================
Spectrum::Spectrum()
{
    setSize(800, 220);

    label20Hz.setText("20 Hz", dontSendNotification);
    label20Hz.setJustificationType(Justification::left);
    label20Hz.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&label20Hz);

    label80Hz.setText("80 Hz", dontSendNotification);
    label80Hz.setJustificationType(Justification::centred);
    label80Hz.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&label80Hz);

    label300Hz.setText("300 Hz", dontSendNotification);
    label300Hz.setJustificationType(Justification::centred);
    label300Hz.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&label300Hz);

    label1kHz.setText("1 kHz", dontSendNotification);
    label1kHz.setJustificationType(Justification::centred);
    label1kHz.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&label1kHz);

    label5kHz.setText("5 kHz", dontSendNotification);
    label5kHz.setJustificationType(Justification::centred);
    label5kHz.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&label5kHz);

    label10kHz.setText("10 kHz", dontSendNotification);
    label10kHz.setJustificationType(Justification::centred);
    label10kHz.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&label10kHz);

    label20kHz.setText("20 kHz", dontSendNotification);
    label20kHz.setJustificationType(Justification::right);
    label20kHz.setColour(Label::textColourId, Colour::fromRGB(0xB1, 0x2E, 0x82));
    addAndMakeVisible(&label20kHz);
}

Spectrum::~Spectrum()
{
}

void Spectrum::paint (juce::Graphics& g)
{
    g.setColour(Colour::fromRGB(0x0B, 0x0D, 0x11));
    g.fillRect(area);

    g.setColour(Colours::white);
    g.fillRect(marker80Hz);
    g.fillRect(marker300Hz);
    g.fillRect(marker1kHz);
    g.fillRect(marker5kHz);
    g.fillRect(marker10kHz);

    g.drawRect(area);
}

void Spectrum::resized()
{
    area.setBounds(0, 0, 800, 180);

    marker80Hz.setBounds(161, 0, 1, 180);
    marker300Hz.setBounds(314, 0, 1, 180);
    marker1kHz.setBounds(454, 0, 1, 180);
    marker5kHz.setBounds(640, 0, 1, 180);
    marker10kHz.setBounds(720, 0, 1, 180);

    label20Hz.setBounds(-5, 185, 60, 20);
    label80Hz.setBounds(131, 185, 60, 20);
    label300Hz.setBounds(284, 185, 60, 20);
    label1kHz.setBounds(424, 185, 60, 20);
    label5kHz.setBounds(600, 185, 60, 20);
    label10kHz.setBounds(690, 185, 60, 20);
    label20kHz.setBounds(745, 185, 60, 20);
}

void Spectrum::mouseDown(const MouseEvent& event)
{
    int x = event.getMouseDownX();
    int y = mapToLog10(x / 800.0, 20.0, 20000.0);
    DBG(x);
    DBG(y);
}