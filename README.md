# bitcrusher
Bitcrusher effect implemented in C++ using the JUCE framework. It additionally incorporates a delay and HPF for more interesting sound.

Bitcrusher sliders and parameters:

• Noise is being generated inside this effect and can be added to the processed audio. Increasing
the value of the slider will increase the noise volume.

• Bit slider allows the user to reduce the number of bits, resulting in resolution reduction and
“clicking” effect. 32 is the maximum value, since there are usually 32-bits used for encoding
audio.

• Rate slider allows the user to increase the the reduction factor for the sampling rate. The higher
the factor, the more extreme downsampling occurs. Maximum value is 50, meaning that a value
of 50 reduces the sample rate to 1/50th of its original rate.

• High-pass filter sliders allows the user to change the value of the desired cut-off frequency

