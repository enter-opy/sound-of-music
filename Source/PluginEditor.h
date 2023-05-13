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
    Slider::Listener, Timer
{
public:
    SoundofmusicAudioProcessorEditor (SoundofmusicAudioProcessor&);
    ~SoundofmusicAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider*) override;
    void sliderDragStarted(Slider*) override;
    void sliderDragEnded(Slider*) override;

    void drawNextFrameOfSpectrum();
    void drawFrame(juce::Graphics&);

    void timerCallback() override;

    juce::Rectangle<int> spectrumRect;

private:
    SoundofmusicAudioProcessor& audioProcessor;

    juce::dsp::FFT forwardFFT;
    juce::dsp::WindowingFunction<float> window;

    Slider crushSlider;
    Slider downSampleSlider;
    Slider jitterSlider;
    Slider clipSlider;

    SliderLookAndFeel crushLookAndFeel;
    SliderLookAndFeel downSampleLookAndFeel;
    SliderLookAndFeel jitterLookAndFeel;
    SliderLookAndFeel clipLookAndFeel;

    Label crushName;
    Label downSampleName;
    Label jitterName;
    Label clipName;

    Label crushIndicator;
    Label downSampleIndicator;
    Label jitterIndicator;
    Label clipIndicator;

    Slider monoSlider;
    Slider mixSlider;

    SliderLookAndFeel monoLookAndFeel;
    SliderLookAndFeel mixLookAndFeel;

    Label monoName;
    Label mixName;

    Label monoIndicator;
    Label mixIndicator;

    Spectrum spectrum;

    Rectangle<float> temp;

    Rectangle<float> distortionArea;
    Rectangle<float> spectrumArea;

public:
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> crushValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> downSampleValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> jitterValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> clipValue;

    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> monoValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> mixValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundofmusicAudioProcessorEditor)
};
