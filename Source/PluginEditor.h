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
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotatoryStartAngle, float rotatoryEndAngle, juce::Slider& slider) override {
        float diameter = jmin(width - 5, height - 5);
        float radius = diameter / 2;
        float centerX = x + width / 2;
        float centerY = y + height / 2;
        float rx = centerX - radius;
        float ry = centerY - radius;
        float angle = rotatoryStartAngle + sliderPos * (rotatoryEndAngle - rotatoryStartAngle);

        Rectangle<float> area(rx, ry, diameter, diameter);

        g.setColour(Colours::white);
        g.fillEllipse(area);

        Path highlight;
        highlight.addRectangle(0, -radius + 2.0, 2.5f, radius * 0.5);
        g.setColour(Colours::black);
        g.fillPath(highlight, AffineTransform::rotation(angle).translated(centerX, centerY));

        g.drawEllipse(area, 2.5f);
    }
};

class SoundofmusicAudioProcessorEditor  : public juce::AudioProcessorEditor,
    public Slider::Listener
{
public:
    SoundofmusicAudioProcessorEditor (SoundofmusicAudioProcessor&);
    ~SoundofmusicAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void processBackground();

    void sliderValueChanged(Slider* slider) override;
    void mouseUp(const MouseEvent& event) override;
    void mouseDown(const MouseEvent& event) override;

private:
    SoundofmusicAudioProcessor& audioProcessor;

    Slider bitdepthSlider;
    Slider samplerateSlider;
    Slider jitterSlider;
    Slider clipCeilingSlider;
    Slider crackleSlider;
    Slider monoSlider;
    Slider mixSlider;

    Image oldGuitarist;
    Image oldGuitaristCopy;
    Image frame;
    Image sliderLabels;

    int bitepth_;
    int step_;
    float jitter_;
    float clipCeiling_;
    float crackle_;
    float mono_;
    float mix_;

    float red;
    float green;
    float blue;

    Random random;

    bool clicked;
    bool valueChanged;

    SliderLookAndFeel sliderLookAndFeel;

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
