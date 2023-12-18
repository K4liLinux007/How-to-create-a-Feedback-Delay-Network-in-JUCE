#pragma once
#include "delays.h"




//==============================================================================
class MainContentComponent   : public juce::AudioAppComponent,
                               public juce::ChangeListener
{
public:
    MainContentComponent()
        : state (Stopped)
    {
        addAndMakeVisible (&openButton);
        openButton.setButtonText ("Open...");
        openButton.onClick = [this] { openButtonClicked(); };

        addAndMakeVisible (&playButton);
        playButton.setButtonText ("Play");
        playButton.onClick = [this] { playButtonClicked(); };
        playButton.setColour (juce::TextButton::buttonColourId, juce::Colours::green);
        playButton.setEnabled (false);

        addAndMakeVisible (&stopButton);
        stopButton.setButtonText ("Stop");
        stopButton.onClick = [this] { stopButtonClicked(); };
        stopButton.setColour (juce::TextButton::buttonColourId, juce::Colours::red);
        stopButton.setEnabled (false);

        setSize (300, 200);

        formatManager.registerBasicFormats();       // [1]
        transportSource.addChangeListener (this);   // [2]

        setAudioChannels (0, 2);
    }

    ~MainContentComponent() override
    {
        shutdownAudio();
    }

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {


        transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
        
        

        allpass1L.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass2L.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass3L.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass4L.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass5L.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass6L.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass7L.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass8L.prepareToPlay(samplesPerBlockExpected, sampleRate);
        

        delay1L.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay2L.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay3L.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay4L.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay5L.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay6L.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay7L.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay8L.prepareToPlay(samplesPerBlockExpected, sampleRate);

        allpass1R.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass2R.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass3R.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass4R.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass5R.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass6R.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass7R.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass8R.prepareToPlay(samplesPerBlockExpected, sampleRate);


        delay1R.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay2R.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay3R.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay4R.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay5R.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay6R.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay7R.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay8R.prepareToPlay(samplesPerBlockExpected, sampleRate);


        allpass1L.setDelayTimeSamples(44100 * 0.020346);
        allpass2L.setDelayTimeSamples(44100 * 0.024421);
        allpass3L.setDelayTimeSamples(44100 * 0.031604);
        allpass4L.setDelayTimeSamples(44100 * 0.027333);
        allpass5L.setDelayTimeSamples(44100 * 0.022904);
        allpass6L.setDelayTimeSamples(44100 * 0.029291);
        allpass7L.setDelayTimeSamples(44100 * 0.013458);
        allpass8L.setDelayTimeSamples(44100 * 0.019123);
        
 
        delay1L.setDelayTimeSamples(44100 * 0.153129);
        delay2L.setDelayTimeSamples(44100 * 0.210389);
        delay3L.setDelayTimeSamples(44100 * 0.127837);
        delay4L.setDelayTimeSamples(44100 * 0.256891);
        delay5L.setDelayTimeSamples(44100 * 0.174713);
        delay6L.setDelayTimeSamples(44100 * 0.192303);
        delay7L.setDelayTimeSamples(44100 * 0.125000);
        delay8L.setDelayTimeSamples(44100 * 0.219991);

        allpass1R.setDelayTimeSamples(44100 * 0.020346);
        allpass2R.setDelayTimeSamples(44100 * 0.024421);
        allpass3R.setDelayTimeSamples(44100 * 0.031604);
        allpass4R.setDelayTimeSamples(44100 * 0.027333);
        allpass5R.setDelayTimeSamples(44100 * 0.022904);
        allpass6R.setDelayTimeSamples(44100 * 0.029291);
        allpass7R.setDelayTimeSamples(44100 * 0.013458);
        allpass8R.setDelayTimeSamples(44100 * 0.019123);


        delay1R.setDelayTimeSamples(44100 * 0.153129);
        delay2R.setDelayTimeSamples(44100 * 0.210389);
        delay3R.setDelayTimeSamples(44100 * 0.127837);
        delay4R.setDelayTimeSamples(44100 * 0.256891);
        delay5R.setDelayTimeSamples(44100 * 0.174713);
        delay6R.setDelayTimeSamples(44100 * 0.192303);
        delay7R.setDelayTimeSamples(44100 * 0.125000);
        delay8R.setDelayTimeSamples(44100 * 0.219991);
        
   
        
        reverbBuffer.setSize(16, samplesPerBlockExpected);
   
        for (int channel = 0; channel < 16; ++channel) {
            lowPassFilters[channel].setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, 3000 ));
        }
        
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        if (readerSource.get() == nullptr)
        {
            bufferToFill.clearActiveBufferRegion();
            return;
        }

        //bufferToFill.clearActiveBufferRegion();
        auto bufferLength = bufferToFill.buffer->getNumSamples();


        transportSource.getNextAudioBlock(bufferToFill);

        // HERE I SPLIT THE INPUT      
        for (int channel = 0; channel < 16; ++channel) {
            int originalChannel = channel % 2;
            const float* bufferData = bufferToFill.buffer->getReadPointer(originalChannel);
            reverbBuffer.copyFrom(channel, 0, bufferData, bufferLength);

        }

        auto channelData1L = reverbBuffer.getWritePointer(0);
        auto channelData1R = reverbBuffer.getWritePointer(1);
        auto channelData2L = reverbBuffer.getWritePointer(2);
        auto channelData2R = reverbBuffer.getWritePointer(3);
        auto channelData3L = reverbBuffer.getWritePointer(4);
        auto channelData3R = reverbBuffer.getWritePointer(5);
        auto channelData4L = reverbBuffer.getWritePointer(6);
        auto channelData4R = reverbBuffer.getWritePointer(7);
        auto channelData5L = reverbBuffer.getWritePointer(8);
        auto channelData5R = reverbBuffer.getWritePointer(9);
        auto channelData6L = reverbBuffer.getWritePointer(10);
        auto channelData6R = reverbBuffer.getWritePointer(11);
        auto channelData7L = reverbBuffer.getWritePointer(12);
        auto channelData7R = reverbBuffer.getWritePointer(13);
        auto channelData8L = reverbBuffer.getWritePointer(14);
        auto channelData8R = reverbBuffer.getWritePointer(15);

            for (int sample = 0; sample < reverbBuffer.getNumSamples(); sample++) {
                
                
                //OUTPUT OF DELAY AND LOWPASS
                float one1L = delay1L.popSample();
                float* onepL = &one1L;
                lowPassFilters[0].processSamples(onepL, 1);
                //OUTPUT OF ALLPASS
                float temp11L = allpass1L.popSample();
                //INPUT FOR THE ALLPASS
                float input1L = one1L * 0.2 + channelData1L[sample] - 0.6 * temp11L;
                // ALL THE INPUT SHOULD BE MULTIPLIED BY DECAY
                float feedforward_allpass1L = input1L * 0.6;
                //PUSH THE INPUT IN ALLPASS(WITHOUT THE DECAY OF FEEDFORWARD)
                allpass1L.pushSample(input1L);

                
                float one1D = delay1R.popSample();
                float* onepD = &one1D;
                lowPassFilters[1].processSamples(onepD, 1);
                float temp11D = allpass1R.popSample();
                float input1D = one1D * 0.2 + channelData1R[sample] - 0.6 * temp11D;
                float feedforward_allpass1D = input1D * 0.6;
                allpass1R.pushSample(input1D);

                float two1S = delay2L.popSample();
                float* twopS = &two1S;
                lowPassFilters[1].processSamples(twopS, 1);
                float temp22S = allpass2L.popSample();
                float input2S = two1S * 0.2  + channelData2L[sample] + 0.6 * temp22S;
                float feedforward_allpass2S = -(input2S * 0.6);
                allpass2L.pushSample(input2S);

                float two1D = delay2R.popSample();
                float* twopD = &two1D;
                lowPassFilters[1].processSamples(twopD, 1);
                float temp22D = allpass2R.popSample();
                float input2D = two1D * 0.2 + channelData2R[sample] + 0.6 * temp22D;
                float feedforward_allpass2D = -(input2D * 0.6);
                allpass2R.pushSample(input2D);
 

                float three1S = delay3L.popSample();
                float* threepS = &three1S;
                lowPassFilters[2].processSamples(threepS, 1);
                float temp33S = allpass3L.popSample();
                float input3S = three1S * 0.2 + channelData3L[sample] - 0.6 * temp33S;
                float feedforward_allpass3S = input3S * 0.6;
                allpass3L.pushSample(input3S);

                float three1D = delay3R.popSample();
                float* threepD = &three1D;
                lowPassFilters[2].processSamples(threepD, 1);
                float temp33D = allpass3R.popSample();
                float input3D = three1D * 0.2 + channelData3R[sample] - 0.6 * temp33D;
                float feedforward_allpass3D = input3D * 0.6;
                allpass3R.pushSample(input3D);

                float four1S = delay4L.popSample();
                float* fourpS = &four1S;
                lowPassFilters[3].processSamples(fourpS, 1);
                float temp44S = allpass4L.popSample();
                float input4S = four1S * 0.2  + channelData4L[sample] + 0.6 * temp44S;
                float feedforward_allpass4S = -(input4S * 0.6);
                allpass4L.pushSample(input4S);

                float four1D = delay4R.popSample();
                float* fourpD = &four1D;
                lowPassFilters[3].processSamples(fourpD, 1);
                float temp44D = allpass4R.popSample();
                float input4D = four1D * 0.2 + channelData4R[sample] + 0.6 * temp44D;
                float feedforward_allpass4D = -(input4D * 0.6);
                allpass4R.pushSample(input4D);
 
                float five1S = delay5L.popSample();
                float* fivepS = &five1S;
                lowPassFilters[4].processSamples(fivepS, 1);
                float temp55S = allpass5L.popSample();
                float input5S = five1S * 0.2 - 0.6 * temp55S + channelData5L[sample];
                float feedforward_allpass5S = input5S * 0.6;
                allpass5L.pushSample(input5S);

                float five1D = delay5R.popSample();
                float* fivepD = &five1D;
                lowPassFilters[4].processSamples(fivepD, 1);
                float temp55D = allpass5R.popSample();
                float input5D = five1D * 0.2 - 0.6 * temp55D + channelData5R[sample];
                float feedforward_allpass5D = input5D * 0.6;
                allpass5R.pushSample(input5D);
              
                float six1S = delay6L.popSample();
                float* sixpS = &six1S;
                lowPassFilters[5].processSamples(sixpS, 1);
                float temp66S = allpass6L.popSample();
                float input6S = six1S * 0.2 + 0.6 * temp66S + channelData6L[sample];
                float feedforward_allpass6S = -(input6S * 0.6);
                allpass6L.pushSample(input6S);

                float six1D = delay6R.popSample();
                float* sixpD = &six1D;
                lowPassFilters[5].processSamples(sixpD, 1);
                float temp66D = allpass6R.popSample();
                float input6D = six1D * 0.2 + 0.6 * temp66D + channelData6R[sample];
                float feedforward_allpass6D = -(input6D * 0.6);
                allpass6R.pushSample(input6D);

                float seven1S = delay7L.popSample();
                float* sevenpS = &seven1S;
                lowPassFilters[6].processSamples(sevenpS, 1);
                float temp77S = allpass7L.popSample();
                float input7S = seven1S * 0.2 - 0.6 * temp77S + channelData7L[sample];
                float feedforward_allpass7S = input7S * 0.6;
                allpass7L.pushSample(input7S);

                float seven1D = delay7R.popSample();
                float* sevenpD = &seven1D;
                lowPassFilters[6].processSamples(sevenpD, 1);
                float temp77D = allpass7R.popSample();
                float input7D = seven1D * 0.2 - 0.6 * temp77D + channelData7R[sample];
                float feedforward_allpass7D = input7D * 0.6;
                allpass7R.pushSample(input7D);

                float eigth1S = delay8L.popSample();
                float* eigthpS = &eigth1S;
                lowPassFilters[7].processSamples(eigthpS, 1);
                float temp88S = allpass8L.popSample();
                float input8S = eigth1S * 0.2 + 0.6 * temp88S + channelData8L[sample];
                float feedforward_allpass8S = -(input8S * 0.6);
                allpass8L.pushSample(input8S);

                float eigth1D = delay8R.popSample();
                float* eigthpD = &eigth1D;
                lowPassFilters[7].processSamples(eigthpD, 1);
                float temp88D = allpass8R.popSample();
                float input8D = eigth1D * 0.2 + 0.6 * temp88D + channelData8R[sample];
                float feedforward_allpass8D = -(input8D * 0.6);
                allpass8R.pushSample(input8D);


                //HERE I PREPARE AN ARRAY TO BE PROCESSED IN THE HADAMARD MATRIX THAT I HAVE SEEN IN FAUST
                arrayS[0] = temp11L + feedforward_allpass1L;
                arrayD[0] = temp11D + feedforward_allpass1D;
                arrayS[1] = temp22S + feedforward_allpass2S; 
                arrayD[1] = temp22D + feedforward_allpass2D;
                arrayS[2] = temp33S + feedforward_allpass3S; 
                arrayD[2] = temp33D + feedforward_allpass3D;
                arrayS[3] = temp44S + feedforward_allpass4S;
                arrayD[3] = temp44D + feedforward_allpass4D;
                arrayS[4] = temp55S + feedforward_allpass5S;
                arrayD[4] = temp55D + feedforward_allpass5D;
                arrayS[5] = temp66S + feedforward_allpass6S;
                arrayD[5] = temp66D + feedforward_allpass6D;
                arrayS[6] = temp77S + feedforward_allpass7S;
                arrayD[6] = temp77D + feedforward_allpass7D;
                arrayS[7] = temp88S + feedforward_allpass8S;
                arrayD[7] = temp88D + feedforward_allpass8D;
                
                hadamard(arrayS);
                hadamard(arrayD);
                
                float oneS = arrayS[0];
                float oneD = arrayD[0];
                float twoS = arrayS[1];
                float twoD = arrayD[1];
                float threeS = arrayS[2];
                float threeD = arrayD[2];
                float fourS = arrayS[3];
                float fourD = arrayD[3];
                float fiveS = arrayS[4];
                float fiveD = arrayD[4];
                float sixS = arrayS[5];
                float sixD = arrayD[5];
                float sevenS = arrayS[6];
                float sevenD = arrayD[6];
                float eigthS = arrayS[7];
                float eigthD = arrayD[7];

                
                
              
                
              
                
               
                
                
                // HERE I SET THE 8 OUTPUTS
                channelData1L[sample] = oneS;
                channelData1R[sample] = oneD;
                channelData2L[sample] = twoS;
                channelData2R[sample] = twoD;
                channelData3L[sample] = threeS;
                channelData3R[sample] = threeD;
                channelData4L[sample] = fourS;
                channelData4R[sample] = fourD;
                channelData5L[sample] = fiveS;
                channelData5R[sample] = fiveD;
                channelData6L[sample] = sixS;
                channelData6R[sample] = sixD;
                channelData7L[sample] = sevenS;
                channelData7R[sample] = sevenD;
                channelData8L[sample] = eigthS;
                channelData8R[sample] = eigthD;
              

                // I LOAD THE OUTPUTS IN THE DELAYS
                delay1L.pushSample(oneS);
                delay1R.pushSample(oneD);
                delay2L.pushSample(twoS) ;
                delay2R.pushSample(twoD);
                delay3L.pushSample(threeS) ;
                delay3R.pushSample(threeD);
                delay4L.pushSample(fourS) ;
                delay4R.pushSample(fourD);
                delay5L.pushSample(fiveS) ;
                delay5R.pushSample(fiveD);
                delay6L.pushSample(sixS) ;
                delay6R.pushSample(sixD);
                delay7L.pushSample(sevenS) ;
                delay7R.pushSample(sevenD);
                delay8L.pushSample(eigthS) ;
                delay8R.pushSample(eigthD);


            }

        // HERE I PUT THE 8 OUTPUTS IN THE 2 CHANNELS OUTPUT
        for (int channel = 0; channel < 16; ++channel) {
            int originalChannel = channel % 2;
            const float* bufferData = reverbBuffer.getReadPointer(channel);
            bufferToFill.buffer->copyFromWithRamp(originalChannel, 0, bufferData, bufferLength, 0.4, 0.4);

        }

    }
    
    

    void releaseResources() override
    {
        transportSource.releaseResources();
    }

    void resized() override
    {
        openButton.setBounds (10, 10, getWidth() - 20, 20);
        playButton.setBounds (10, 40, getWidth() - 20, 20);
        stopButton.setBounds (10, 70, getWidth() - 20, 20);
    }

    void changeListenerCallback (juce::ChangeBroadcaster* source) override
    {
        if (source == &transportSource)
        {
            if (transportSource.isPlaying())
                changeState (Playing);
            else
                changeState (Stopped);
        }
    }

private:
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };

    void changeState (TransportState newState)
    {
        if (state != newState)
        {
            state = newState;

            switch (state)
            {
                case Stopped:                           // [3]
                    stopButton.setEnabled (false);
                    playButton.setEnabled (true);
                    transportSource.setPosition (0.0);
                    break;

                case Starting:                          // [4]
                    playButton.setEnabled (false);
                    transportSource.start();
                    break;

                case Playing:                           // [5]
                    stopButton.setEnabled (true);
                    break;

                case Stopping:                          // [6]
                    transportSource.stop();
                    break;
            }
        }
    }

    void openButtonClicked()
    {
        chooser = std::make_unique<juce::FileChooser> ("Select a Wave file to play...",
                                                       juce::File{},
                                                       "*.wav");                     // [7]
        auto chooserFlags = juce::FileBrowserComponent::openMode
                          | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)     // [8]
        {
            auto file = fc.getResult();

            if (file != juce::File{})                                                // [9]
            {
                auto* reader = formatManager.createReaderFor (file);                 // [10]

                if (reader != nullptr)
                {
                    auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);   // [11]
                    transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);       // [12]
                    playButton.setEnabled (true);                                                      // [13]
                    readerSource.reset (newSource.release());                                          // [14]
                }
            }
        });
    }

    void playButtonClicked()
    {
        changeState (Starting);
    }

    void stopButtonClicked()
    {
        changeState (Stopping);
    }

    virtual float hadamard(float array[]) {
        float temp1 = 0;
        float temp2 = 0;
        float temp3 = 0;
        float temp4 = 0;
        float temp5 = 0;
        float temp6 = 0;
        float temp7 = 0;
        float temp8 = 0;

        float temp11 = 0;
        float temp22 = 0;
        float temp33 = 0;
        float temp44 = 0;
        float temp55 = 0;
        float temp66 = 0;
        float temp77 = 0;
        float temp88 = 0;

        temp1 = array[0] + array[4];
        temp2 = array[1] + array[5];
        temp3 = array[2] + array[6];
        temp4 = array[3] + array[7];
        temp5 = array[0] - array[4];
        temp6 = array[1] - array[5];
        temp7 = array[2] - array[6];
        temp8 = array[3] - array[7];

        temp11 = temp1 + temp3;
        temp22 = temp2 + temp4;
        temp33 = temp1 - temp3;
        temp44 = temp2 - temp4;
        temp55 = temp5 + temp7;
        temp66 = temp6 + temp8;
        temp77 = temp5 - temp7;
        temp88 = temp6 - temp8;

        temp1 = temp11 + temp22;
        temp2 = temp11 - temp22;
        temp3 = temp33 + temp44;
        temp4 = temp33 - temp44;
        temp5 = temp55 + temp66;
        temp6 = temp55 - temp66;
        temp7 = temp77 + temp88;
        temp8 = temp77 - temp88;

        array[0] = temp1;
        array[1] = temp2;
        array[2] = temp3;
        array[3] = temp4;
        array[4] = temp5;
        array[5] = temp6;
        array[6] = temp7;
        array[7] = temp8;


        return 0;

    }

    

    //==========================================================================
    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;

    std::unique_ptr<juce::FileChooser> chooser;

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;
    juce::AudioBuffer<float>reverbBuffer;

   

    float arrayS[8];
    float arrayD[8];
    juce::IIRFilter lowPassFilters[16];

    SDSP::DelayLine delay1L;
    SDSP::DelayLine delay2L;
    SDSP::DelayLine delay3L;
    SDSP::DelayLine delay4L;
    SDSP::DelayLine delay5L;
    SDSP::DelayLine delay6L;
    SDSP::DelayLine delay7L;
    SDSP::DelayLine delay8L;


    SDSP::APF allpass1L;
    SDSP::APF allpass2L;
    SDSP::APF allpass3L;
    SDSP::APF allpass4L;
    SDSP::APF allpass5L;
    SDSP::APF allpass6L;
    SDSP::APF allpass7L;
    SDSP::APF allpass8L;

    SDSP::DelayLine delay1R;
    SDSP::DelayLine delay2R;
    SDSP::DelayLine delay3R;
    SDSP::DelayLine delay4R;
    SDSP::DelayLine delay5R;
    SDSP::DelayLine delay6R;
    SDSP::DelayLine delay7R;
    SDSP::DelayLine delay8R;


    SDSP::APF allpass1R;
    SDSP::APF allpass2R;
    SDSP::APF allpass3R;
    SDSP::APF allpass4R;
    SDSP::APF allpass5R;
    SDSP::APF allpass6R;
    SDSP::APF allpass7R;
    SDSP::APF allpass8R;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
