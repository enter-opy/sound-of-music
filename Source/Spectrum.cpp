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
    setSize(800, 200);
}

Spectrum::~Spectrum()
{
}

void Spectrum::paint (juce::Graphics& g)
{
    g.setColour(Colour::fromRGB(0x0B, 0x0D, 0x11));
    g.fillRect(area);
    g.setColour(Colours::white);
    g.drawRect(area);
}

void Spectrum::resized()
{
    area.setBounds(0, 0, 800, 180);
}

void Spectrum::mouseDown(const MouseEvent& event)
{
    int x = event.getMouseDownX();
    int y = mapToLog10(x / 800.0, 20.0, 20000.0);
    DBG(y);
}

