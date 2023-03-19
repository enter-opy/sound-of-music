/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoundofmusicAudioProcessorEditor::SoundofmusicAudioProcessorEditor (SoundofmusicAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize(632, 900);

    bitdepthValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, BITDEPTH_ID, bitdepthSlider);
    samplerateValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, SAMPLERATE_ID, samplerateSlider);
    jitterValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, JITTER_ID, jitterSlider);
    clipCeilingValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, CLIPCELING_ID, clipCeilingSlider);
    crackleValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, CRACKLE_ID, crackleSlider);
    monoValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, MONO_ID, monoSlider);
    mixValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, MIX_ID, mixSlider);

    bitdepthSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    bitdepthSlider.setRange(2.0, 32.0, 1.0);
    bitdepthSlider.setValue(audioProcessor.getValue(0));
    bitdepthSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    bitdepthSlider.setLookAndFeel(&sliderLookAndFeel);
    bitdepthSlider.addMouseListener(this, true);
    bitdepthSlider.addListener(this);
    addAndMakeVisible(&bitdepthSlider);

    samplerateSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    samplerateSlider.setRange(1102.5, 44100.0, 1.0);
    samplerateSlider.setValue(audioProcessor.getValue(1));
    samplerateSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    samplerateSlider.setLookAndFeel(&sliderLookAndFeel);
    samplerateSlider.addMouseListener(this, true);
    samplerateSlider.addListener(this);
    addAndMakeVisible(&samplerateSlider);

    jitterSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    jitterSlider.setRange(0.0, 100.0, 1.0);
    jitterSlider.setValue(audioProcessor.getValue(2));
    jitterSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    jitterSlider.setLookAndFeel(&sliderLookAndFeel);
    jitterSlider.addMouseListener(this, true);
    jitterSlider.addListener(this);
    addAndMakeVisible(&jitterSlider);

    clipCeilingSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    clipCeilingSlider.setRange(-15.0, 0.0, 0.1);
    clipCeilingSlider.setValue(audioProcessor.getValue(3));
    clipCeilingSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    clipCeilingSlider.setLookAndFeel(&sliderLookAndFeel);
    clipCeilingSlider.addMouseListener(this, true);
    clipCeilingSlider.addListener(this);
    addAndMakeVisible(&clipCeilingSlider);

    crackleSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    crackleSlider.setRange(0.0, 100.0, 1.0);
    crackleSlider.setValue(audioProcessor.getValue(4));
    crackleSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    crackleSlider.setLookAndFeel(&sliderLookAndFeel);
    crackleSlider.addMouseListener(this, true);
    crackleSlider.addListener(this);
    addAndMakeVisible(&crackleSlider);

    monoSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    monoSlider.setRange(0.0, 100.0, 1.0);
    monoSlider.setValue(audioProcessor.getValue(5));
    monoSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    monoSlider.setLookAndFeel(&sliderLookAndFeel);
    monoSlider.addMouseListener(this, true);
    monoSlider.addListener(this);
    addAndMakeVisible(&monoSlider);

    mixSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    mixSlider.setRange(0.0, 100.0, 0.0);
    mixSlider.setValue(audioProcessor.getValue(6));
    mixSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    mixSlider.setLookAndFeel(&sliderLookAndFeel);
    mixSlider.addMouseListener(this, true);
    mixSlider.addListener(this);
    addAndMakeVisible(&mixSlider);
}

SoundofmusicAudioProcessorEditor::~SoundofmusicAudioProcessorEditor()
{
}

//==============================================================================
void SoundofmusicAudioProcessorEditor::paint (juce::Graphics& g)
{
    oldGuitarist = ImageCache::getFromMemory(BinaryData::oldguitarist_png, BinaryData::oldguitarist_pngSize);
    oldGuitaristCopy = ImageCache::getFromMemory(BinaryData::oldguitaristcopy_png, BinaryData::oldguitaristcopy_pngSize);
    frame = ImageCache::getFromMemory(BinaryData::frame_png, BinaryData::frame_pngSize);
    sliderLabels = ImageCache::getFromMemory(BinaryData::sliderlabels_png, BinaryData::sliderlabels_pngSize);

    g.drawImageAt(oldGuitaristCopy, 56, 60);
    g.drawImageAt(frame, 0, 0);
    g.drawImageAt(sliderLabels, 103, 210);

    bitdepthSlider.setValue(audioProcessor.getValue(0));
}

void SoundofmusicAudioProcessorEditor::resized()
{
    bitdepthSlider.setBounds(116, 120, 80, 80);
    samplerateSlider.setBounds(116, 260, 80, 80);
    jitterSlider.setBounds(116, 400, 80, 80);
    clipCeilingSlider.setBounds(116, 540, 80, 80);
    crackleSlider.setBounds(116, 680, 80, 80);
    monoSlider.setBounds(370, 245, 120, 120);
    mixSlider.setBounds(370, 510, 120, 120);
}

void SoundofmusicAudioProcessorEditor::mouseUp(const MouseEvent& event) {
    if (valueChanged) {
        processBackground();
    }

    valueChanged = false;
    clicked = false;
}

void SoundofmusicAudioProcessorEditor::mouseDown(const MouseEvent& event) {

    clicked = true;
}

void SoundofmusicAudioProcessorEditor::sliderValueChanged(Slider* slider) {
    valueChanged = true;
}

void SoundofmusicAudioProcessorEditor::processBackground() {
    int height = oldGuitarist.getHeight();
    int width = oldGuitarist.getWidth();

    // get parameters
    bitepth_ = pow(2, bitdepthSlider.getValue()) / 2;
    step_ = audioProcessor.getSampleRate() / samplerateSlider.getValue();
    jitter_ = jitterSlider.getValue() / 1000.0;
    clipCeiling_ = Decibels::decibelsToGain(clipCeilingSlider.getValue());
    crackle_ = crackleSlider.getValue();
    mono_ = monoSlider.getValue() / 300.0;
    mix_ = mixSlider.getValue() / 100.0;

    for (int i = 1; i <= width; ) {
        int m = i;

        for (int j = 1; j <= height; ) {
            i = m;

            red = oldGuitarist.getPixelAt(i, j).getFloatRed();
            green = oldGuitarist.getPixelAt(i, j).getFloatGreen();
            blue = oldGuitarist.getPixelAt(i, j).getFloatBlue();

            // bit depth reduction happens here
            red = round((red)*bitepth_) / bitepth_;
            green = round((green)*bitepth_) / bitepth_;
            blue = round((blue)*bitepth_) / bitepth_;

            // noise is added here
            //if (!(red + 2 * (random.nextInt(3) - 1) * jitter_ > 1.0 || red + 2 * (random.nextInt(3) - 1) * jitter_ > 0.0)) {
            red += (random.nextInt(3) - 1) * jitter_;
            //}
            //if (!(green + 2 * (random.nextInt(3) - 1) * jitter_ > 1.0 || green + 2 * (random.nextInt(3) - 1) * jitter_ > 0.0)) {
            green += (random.nextInt(3) - 1) * jitter_;
            //}
            //if (!(blue + 2 * (random.nextInt(3) - 1) * jitter_ > 1.0 || blue + 2 * (random.nextInt(3) - 1) * jitter_ > 0.0)) {
            blue += (random.nextInt(3) - 1) * jitter_;
            //}

            // clipping happens here
            if (red >= clipCeiling_) {
                red = clipCeiling_;
            }
            if (green >= clipCeiling_) {
                green = clipCeiling_;
            }
            if (blue >= clipCeiling_) {
                blue = clipCeiling_;
            }
            red *= 1 / clipCeiling_;
            green *= 1 / clipCeiling_;
            blue *= 1 / clipCeiling_;

            // crackle is added here
            if (crackle_ > 0) {
                if (random.nextInt(100 - crackle_ + 2) == 0) {
                    red = 0.0;
                    green = 0.0;
                    blue = 0.0;
                }
            }

            // monochrome is applied here
            red = (1 - 2 * mono_) * red + mono_ * green + mono_ * blue;
            green = mono_ * red + (1 - 2 * mono_) * green + mono_ * blue;
            blue = mono_ * red + mono_ * green + (1 - 2 * mono_) * blue;

            int n = j;

            // reduction of resolution happens here
            for (int k = 1; k <= step_ && k <= width; k++, i++) {
                j = n;
                for (int l = 1; l <= step_ && l <= height; l++, j++) {
                    // mix is applied here
                    red = (1 - mix_) * oldGuitarist.getPixelAt(i, j).getFloatRed() + mix_ * red;
                    green = (1 - mix_) * oldGuitarist.getPixelAt(i, j).getFloatGreen() + mix_ * green;
                    blue = (1 - mix_) * oldGuitarist.getPixelAt(i, j).getFloatBlue() + mix_ * blue;

                    oldGuitaristCopy.setPixelAt(i, j, Colour::fromFloatRGBA(red, green, blue, 1.0));
                }
            }

            repaint();
        }
    }
}