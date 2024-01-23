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

class SliderLookAndFeelGradient : public LookAndFeel_V4 {
public:
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float rotatoryStartAngle, float rotatoryEndAngle, juce::Slider& slider) override {
        float diameter = jmin(width - 5, height - 5);
        float radius = diameter / 2;
        float centerX = x + width / 2;
        float centerY = y + height / 2;
        float rx = centerX - radius;
        float ry = centerY - radius;
        float angle = rotatoryStartAngle + sliderPos * (rotatoryEndAngle - rotatoryStartAngle);

        Rectangle<float> area(rx, ry, diameter, diameter);

        Path thumb;
        thumb.addEllipse(0, -radius + 10.0, 10.0f, 10.0f);
        g.setColour(Colour::fromRGB(0x30, 0xE0, 0x30));
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

    void drawNextFrameOfInSpectrum();
    void drawNextFrameOfOutSpectrum();
    void drawFrame(juce::Graphics&);

    void timerCallback(void) override;

    void mouseDown(const MouseEvent&) override;
    void bandSelected(void);

    juce::Rectangle<int> spectrumRect;

private:
    SoundofmusicAudioProcessor& audioProcessor;

    juce::dsp::FFT forwardFFTIn;
    juce::dsp::FFT forwardFFTOut;
    juce::dsp::WindowingFunction<float> window;

    Slider crushSliderLow;
    Slider downSampleSliderLow;
    Slider jitterSliderLow;
    Slider clipSliderLow;

    Slider crushSliderMid;
    Slider downSampleSliderMid;
    Slider jitterSliderMid;
    Slider clipSliderMid;

    Slider crushSliderHigh;
    Slider downSampleSliderHigh;
    Slider jitterSliderHigh;
    Slider clipSliderHigh;

    SliderLookAndFeelGradient crushLookAndFeelLow;
    SliderLookAndFeelGradient downSampleLookAndFeelLow;
    SliderLookAndFeelGradient jitterLookAndFeelLow;
    SliderLookAndFeelGradient clipLookAndFeelLow;

    SliderLookAndFeelGradient crushLookAndFeelMid;
    SliderLookAndFeelGradient downSampleLookAndFeelMid;
    SliderLookAndFeelGradient jitterLookAndFeelMid;
    SliderLookAndFeelGradient clipLookAndFeelMid;

    SliderLookAndFeelGradient crushLookAndFeelHigh;
    SliderLookAndFeelGradient downSampleLookAndFeelHigh;
    SliderLookAndFeelGradient jitterLookAndFeelHigh;
    SliderLookAndFeelGradient clipLookAndFeelHigh;

    Label crushName;
    Label downSampleName;
    Label jitterName;
    Label clipName;

    Label crushIndicatorLow;
    Label downSampleIndicatorLow;
    Label jitterIndicatorLow;
    Label clipIndicatorLow;

    Label crushIndicatorMid;
    Label downSampleIndicatorMid;
    Label jitterIndicatorMid;
    Label clipIndicatorMid;

    Label crushIndicatorHigh;
    Label downSampleIndicatorHigh;
    Label jitterIndicatorHigh;
    Label clipIndicatorHigh;

    Slider monoSlider;
    Slider mixSlider;

    SliderLookAndFeelGradient monoLookAndFeel;
    SliderLookAndFeelGradient mixLookAndFeel;

    Label monoName;
    Label mixName;

    Label monoIndicator;
    Label mixIndicator;

    Label freq1Name;
    Label freq2Name;

    Spectrum spectrum;

    Rectangle<float> temp;

    Rectangle<float> dividerArea;
    Rectangle<float> distortionArea;
    Rectangle<float> outputArea;

    Rectangle<float> marker1;
    Rectangle<float> marker2;

    Label label0;
    Label label1;
    Label label2;
    Label label3;

    Rectangle<int> divider1;
    Rectangle<int> divider2;

    Rectangle<int> bandSelectionArea;

    Slider divider1Slider;
    Slider divider2Slider;

    SliderLookAndFeelGradient divider1LookAndFeel;
    SliderLookAndFeelGradient divider2LookAndFeel;

    int divider1Pos;
    int divider2Pos;

    int band{ 0 };

    int frequency1{ 500 };
    int frequency2{ 2000 };

    float x;

    Typeface::Ptr customFont = juce::Typeface::createSystemTypefaceFor(BinaryData::LACONICK_TTF, BinaryData::LACONICK_TTFSize);

public:
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> crushValueLow;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> downSampleValueLow;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> jitterValueLow;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> clipValueLow;

    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> crushValueMid;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> downSampleValueMid;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> jitterValueMid;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> clipValueMid;

    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> crushValueHigh;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> downSampleValueHigh;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> jitterValueHigh;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> clipValueHigh;

    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> monoValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> mixValue;

    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> divider1Value;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> divider2Value;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundofmusicAudioProcessorEditor)
};
