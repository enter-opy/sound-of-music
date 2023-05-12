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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

Spectrum::~Spectrum()
{
}

void Spectrum::paint (juce::Graphics& g)
{
    g.setColour(Colour::fromRGB(0x0B, 0x0D, 0x11));
    g.fillRect(spectrum);
    g.setColour(Colours::white);
    g.drawRect(spectrum);
}

void Spectrum::resized()
{
    spectrum.setBounds(40, 40, 800, 180);
}
