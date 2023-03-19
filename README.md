<h1>DigitalF</h2>

<h3>An audio plugin that adds digital artifacts to an audio signal</h3>

<br>

<h1>Artifacts</h1>

<h2>Bitdepth</h2>

<h3><i>reduces bits per sample</i><h3>

<br>

```cpp
float wetSampleValue = round((drySampleValue)*maxBitdepthValue) / maxBitdepthValue;
```

<hr>

<h2>Sample rate</h2>

<h3><i>reduces number of samples per second</i><h3>

<br>

```cpp
int step = samplerate / newSamplerate;

for (int i = 0; i < step && sample < buffer.getNumSamples(); i++, sample++) {
    channelData[sample] = wetSampleValue;
}
```

<hr>

<h2>Jitter</h2>

<h3><i>Adds random variation in the waveform</i><h3>

Reference: <a href="https://headfonics.com/what-is-jitter-in-audio/">📖</a>

<br>

```cpp
wetSampleValue += 2 * (random.nextInt(3) - 1) * jitterValue;
```

<hr>

<h2>Clip</h2>

<h3><i>Limits the peak value of the signal</i><h3>

<br>

```cpp
if (wetSampleValue >= clipCeiling) {
    wetSampleValue = clipCeiling;
}
else if (wetSampleValue <= -clipCeiling) {
    wetSampleValue = -clipCeiling;
}
wetSampleValue *= 1 / clipCeiling;
```

<h2>Crackle</h2>

<h3><i>increases the amount of occurance of cracks and pops in the signal</i><h3>

<br>

```cpp
if (crackleValue > 0) {
    if (random.nextInt(100 - crackleValue + 2) == 0) {
        if (random.nextInt(10) == 0) {
            wetSampleValue = -wetSampleValue;
        }
        else {
            wetSampleValue = 0.0;
        }
    }
}
```

<hr>

<br>

<h1>Output</h1>

<br>

<h2>Mono</h2>

<h3><i>Decreases the stereo width of the signal</i><h3>

<br>

```cpp
// monoValue ranges from 0 to 0.5
for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
    leftSampleValue = (1 - monoValue) * leftchannelData[sample] + monoValue * rightchannelData[sample];
    rightSampleValue = monoValue * leftchannelData[sample] + (1 - monoValue) * rightchannelData[sample];

    leftchannelData[sample] = (1 - mixValue) * leftchannelData[sample] + mixValue * leftSampleValue;
    rightchannelData[sample] = (1 - mixValue) * rightchannelData[sample] + mixValue * rightSampleValue;
}
```

<hr>

<h2>Mix</h2>

<h3><i>Blends the dry signal with the wet signal</i><h3>

<br>

```cpp
channelData[sample] = (1 - mixValue) * drySampleValue + mixValue * wetSampleValue;
```

<hr>