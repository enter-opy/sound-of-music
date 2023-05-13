/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Spectrum.h"

//==============================================================================
/**
*/

class SliderLookAndFeel : public LookAndFeel_V4 {
public:
    float colourPosition;

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float rotatoryStartAngle, float rotatoryEndAngle, juce::Slider& slider) override {
        float diameter = jmin(width - 5, height - 5);
        float radius = diameter / 2;
        float centerX = x + width / 2;
        float centerY = y + height / 2;
        float rx = centerX - radius;
        float ry = centerY - radius;
        float angle = rotatoryStartAngle + sliderPos * (rotatoryEndAngle - rotatoryStartAngle);

        ColourGradient gradient(Colour::fromRGB(0x37, 0x9B, 0xE3), centerX, 0, Colour::fromRGB(0xCB, 0x16, 0x6D), centerX, height + 50.0 - colourPosition, false);

        Rectangle<float> area(rx, ry, diameter, diameter);

        Path thumb;
        thumb.addEllipse(0, -radius + 10.0, 10.0f, 10.0f);
        g.setGradientFill(gradient);
        g.fillPath(thumb, AffineTransform::rotation(angle).translated(centerX, centerY));

        g.drawEllipse(area, 4.0f);
    }
};


class SoundofmusicAudioProcessorEditor  : public juce::AudioProcessorEditor,
    Slider::Listener
{
public:
    SoundofmusicAudioProcessorEditor (SoundofmusicAudioProcessor&);
    ~SoundofmusicAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider* slider) override;

private:
    SoundofmusicAudioProcessor& audioProcessor;

    Slider bitdepthSlider;
    Slider samplerateSlider;
    Slider jitterSlider;
    Slider clipCeilingSlider;
    Slider monoSlider;
    Slider mixSlider;

    SliderLookAndFeel bitdepthLookAndFeel;
    SliderLookAndFeel samplerateLookAndFeel;
    SliderLookAndFeel jitterLookAndFeel;
    SliderLookAndFeel clipCeilingLookAndFeel;
    SliderLookAndFeel monoLookAndFeel;
    SliderLookAndFeel mixLookAndFeel;

    Label bitdepthLabel;
    Label samplerateLabel;
    Label jitterLabel;
    Label clipCeilingLabel;
    Label monoLabel;
    Label mixLabel;

    Spectrum spectrum;

    Rectangle<float> distortionArea;

public:
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> bitdepthValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> samplerateValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> jitterValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> clipCeilingValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> monoValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> mixValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundofmusicAudioProcessorEditor)
};
