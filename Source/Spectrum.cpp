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
    ColourGradient gradientOutSplitter(Colour::fromRGBA(0xAB, 0x06, 0x4D, 127), 0, 0, Colour::fromRGBA(0x17, 0x7B, 0xC3, 127), 800, 0, false);
    g.setGradientFill(gradientOutSplitter);
    g.fillRect(splitter);

    g.setColour(Colours::white);
    g.fillRect(marker80Hz);
    g.fillRect(marker300Hz);
    g.fillRect(marker1kHz);
    g.fillRect(marker5kHz);
    g.fillRect(marker10kHz);

    g.drawRect(area);

    for (Rectangle<int> rect: dividers) {
        g.drawRect(rect);
    }
}

void Spectrum::resized()
{
    area.setBounds(0, 0, 800, 180);
    splitter.setBounds(0, 0, 800, 40);

    marker80Hz.setBounds(159, 175, 4, 10);
    marker300Hz.setBounds(312, 175, 4, 10);
    marker1kHz.setBounds(452, 175, 4, 10);
    marker5kHz.setBounds(638, 175, 4, 10);
    marker10kHz.setBounds(718, 175, 4, 10);

    label20Hz.setBounds(-5, 190, 60, 20);
    label80Hz.setBounds(131, 190, 60, 20);
    label300Hz.setBounds(284, 190, 60, 20);
    label1kHz.setBounds(424, 190, 60, 20);
    label5kHz.setBounds(600, 190, 60, 20);
    label10kHz.setBounds(690, 190, 60, 20);
    label20kHz.setBounds(745, 190, 60, 20);
}

void Spectrum::mouseDown(const MouseEvent& event)
{
    if (event.getMouseDownY() < 40 && dividers.size() < 5) {
        int mouseX = event.getMouseDownX();
        int frequency = mapToLog10(mouseX / 800.0, 20.0, 20000.0);

        bands.push_back(frequency);
        sort(bands.begin(), bands.end());

        for (auto z : bands) {
            DBG(" ");
            DBG(z);
        }

        dividers.push_back(Rectangle<int>::Rectangle(mouseX, 0, 1, 180));
    }
    
}