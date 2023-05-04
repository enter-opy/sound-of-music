/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/

class SliderLookAndFeel : public LookAndFeel_V4 {
public:
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle sliderStyle, Slider& slider) override {

        Rectangle<float> area(x, y, width, height);
        Rectangle<float> track(x, sliderPos, width, height);

        g.setColour(Colours::white);
        g.fillRect(track);

        g.setColour(Colours::white);
        g.drawRect(area);
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotatoryStartAngle, float rotatoryEndAngle, juce::Slider& slider) override {
        float diameter = jmin(width - 5, height - 5);
        float radius = diameter / 2;
        float centerX = x + width / 2;
        float centerY = y + height / 2;
        float rx = centerX - radius;
        float ry = centerY - radius;
        float angle = rotatoryStartAngle + sliderPos * (rotatoryEndAngle - rotatoryStartAngle);

        Rectangle<float> area(rx, ry, diameter, diameter);

        Path thumb;
        thumb.addRectangle(0, -radius + 2.0, 2.5f, radius * 0.5);
        g.setColour(Colours::white);
        g.fillPath(thumb, AffineTransform::rotation(angle).translated(centerX, centerY));

        g.drawEllipse(area, 2.5f);
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
