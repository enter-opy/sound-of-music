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
    void drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle sliderStyle, Slider& slider) override {
        Rectangle<float> area(x, y, width, height);
        Rectangle<float> track(x, sliderPos, width, height);

        ColourGradient gradient(Colour::fromRGB(0x37, 0x9B, 0xE3), x + height / 2, y, Colour::fromRGB(0xCB, 0x16, 0x6D), x + height / 2, y + height, false);

        g.setGradientFill(gradient);
        g.fillRect(track);
        g.drawRect(area);
    }

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float rotatoryStartAngle, float rotatoryEndAngle, juce::Slider& slider) override {
        float diameter = jmin(width - 5, height - 5);
        float radius = diameter / 2;
        float centerX = x + width / 2;
        float centerY = y + height / 2;
        float rx = centerX - radius;
        float ry = centerY - radius;
        float angle = rotatoryStartAngle + sliderPos * (rotatoryEndAngle - rotatoryStartAngle);

        ColourGradient gradient(Colour::fromRGB(0x37, 0x9B, 0xE3), centerX, 0, Colour::fromRGB(0xCB, 0x16, 0x6D), centerX, height, false);

        Rectangle<float> area(rx, ry, diameter, diameter);

        Path thumb;
        thumb.addEllipse(0, -radius + 10.0, 10.0f, 10.0f);
        g.setGradientFill(gradient);
        g.fillPath(thumb, AffineTransform::rotation(angle).translated(centerX, centerY));

        g.drawEllipse(area, 4.0f);
    }
};


class SoundofmusicAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SoundofmusicAudioProcessorEditor (SoundofmusicAudioProcessor&);
    ~SoundofmusicAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    SoundofmusicAudioProcessor& audioProcessor;

    Slider bitdepthSlider;
    Slider samplerateSlider;
    Slider jitterSlider;
    Slider clipCeilingSlider;
    Slider crackleSlider;
    Slider monoSlider;
    Slider mixSlider;

    SliderLookAndFeel sliderLookAndFeel;

    Label bitdepthLabel;
    Label samplerateLabel;
    Label jitterLabel;
    Label clipCeilingLabel;
    Label crackleLabel;
    Label monoLabel;
    Label mixLabel;

    Spectrum spectrum;

public:
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> bitdepthValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> samplerateValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> jitterValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> clipCeilingValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> crackleValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> monoValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> mixValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundofmusicAudioProcessorEditor)
};
