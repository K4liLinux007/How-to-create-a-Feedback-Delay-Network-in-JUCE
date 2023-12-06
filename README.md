# How-to-create-a-Feedback-Delay-Network-in-JUCE
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




