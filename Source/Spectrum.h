/*
  ==============================================================================

    Spectrum.h
    Created: 12 May 2023 6:44:42pm
    Author:  vivek

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

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

    void mouseDown(const MouseEvent&) override;

    std::vector<int> bands{ 20, 20000 };
    std::vector<int> dividerPos;

private:
    Rectangle<float> area;
    Rectangle<float> splitter;

    Rectangle<float> marker80Hz;
    Rectangle<float> marker300Hz;
    Rectangle<float> marker1kHz;
    Rectangle<float> marker5kHz;
    Rectangle<float> marker10kHz;

    Label label20Hz;
    Label label80Hz;
    Label label300Hz;
    Label label1kHz;
    Label label5kHz;
    Label label10kHz;
    Label label20kHz;

    std::vector<Rectangle<int>> dividers;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Spectrum)
};
