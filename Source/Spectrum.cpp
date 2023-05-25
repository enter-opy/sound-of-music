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
    setSize(500, 180);
}

Spectrum::~Spectrum()
{
}

void Spectrum::paint (juce::Graphics& g)
{   
    g.setColour(Colours::white);

    g.drawRect(area);
}

void Spectrum::resized()
{
    area.setBounds(0, 0, 500, 180);
}