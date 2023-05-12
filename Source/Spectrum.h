/*
  ==============================================================================

    Spectrum.h
    Created: 12 May 2023 6:44:42pm
    Author:  vivek

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Spectrum  : public juce::Component
{
public:
    Spectrum();
    ~Spectrum() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseDown(const MouseEvent& event) override;

private:
    Rectangle<float> area;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Spectrum)
};
