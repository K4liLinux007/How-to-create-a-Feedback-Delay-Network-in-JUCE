# HOW TO CREATE A FEEDBACK DELAY NETWORK IN JUCE!
How to create a feedback delay network in juce! The project is essentialy the audio player from Juce, I only added the part the code for the reverb. The algorithm is very simple and doesn't have parameters and a good GUI for now!

## VISUAL REPRESENTATION OF THE FEEDBACK DELAY NETWORK
<img src="SimpleFeedbackDelayNetwork/Faust, .exe, wav/VISUAL REPRESENTATION OF THE ALGORITHM (FROM FAUST).png" alt="Employee data" title="Employee Data title">

## WHAT IS A FDN?
#### Feedback delay networks (FDNs) are recursive filters, which are widely used for artificial reverberation and decorrelation.        FDNs are based on a feedback loop with multiple channels containing delay elements, as well as a mixing matrix providing a connection between the channels. For a practical FDN implementation, many parameters have to be chosen: the number of channels, the mixing matrix, the delays, several gains, and, in the case of FDNs that implement a frequency-dependent reverberation time, filters for each channel. 

## WHY I DISCOVERED FDNs
#### My favorite effect is reverb, and I started studying audio programming to create quality reverb effects. My first algorithm was the one invented by Shroeder. However, I did not like the metallic sound. When he found out about the existence of Faust, among the demos he discovered an algorithm called [Zita-Rev1](https://www.dsprelated.com/freebooks/pasp/Zita_Rev1.html) based on a feedback delay network! My algorithm is an extreme simplification, it only served me to be able to figure out how to create a complex feedback loop with the DelayLine class in JUCE and how to use matrices.

## HOW TO USE .DSP FILE IN FAUST
#### Faust is conceptually a block-diagram language. To use my algorythm in FAUST:
#### 1) Download the SimpleFeedbackDelayNetwork.dsp
#### 2) Open [Faust](https://faust.grame.fr/) clicking "Try it Online"!
#### 3) On the left there is the button "Import" to import the .dsp file.  

#### In FAUST is possible to inspect the block to understand how everything work!

## HOW TO USE IN JUCE
#### 1) Download the files
#### 2) Open the .JUCER file with Projucer
#### 3) Select your exporter and open the project with it

# EXPLAINATION OF THE CODE
#### I decided to divide my workflow into 5 steps

![FAUST IDE](https://github.com/K4liLinux007/How-to-create-a-Feedback-Delay-Network-in-JUCE/assets/125820469/813a4271-c577-44de-8a20-f4bd634c9076)
#### 1) Split input from 2 inputs to 8 inputs
#### 2) Set the allpasses, that will have as input: the delayed sounds of the allpasses, the input and the delayed sounds of delays that however will be first processed in the lowpass filters
#### 3) The outputs of the allpasses, which is the sum of the feedforward paths and the delayed sound of allpasses used in the first step, are used as inputs for the matrix
#### 4) The outputs of the matrix will be push in the delays and also will be used as outputs
#### 5) The outputs will be condensed in 2 outputs.




