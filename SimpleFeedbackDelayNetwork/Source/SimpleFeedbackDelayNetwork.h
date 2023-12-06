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
        
        

        allpass1.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass2.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass3.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass4.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass5.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass6.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass7.prepareToPlay(samplesPerBlockExpected, sampleRate);
        allpass8.prepareToPlay(samplesPerBlockExpected, sampleRate);
        

        delay1.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay2.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay3.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay4.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay5.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay6.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay7.prepareToPlay(samplesPerBlockExpected, sampleRate);
        delay8.prepareToPlay(samplesPerBlockExpected, sampleRate);


        allpass1.setDelayTimeSamples(44100 * 0.020346);
        allpass2.setDelayTimeSamples(44100 * 0.024421);
        allpass3.setDelayTimeSamples(44100 * 0.031604);
        allpass4.setDelayTimeSamples(44100 * 0.027333);
        allpass5.setDelayTimeSamples(44100 * 0.022904);
        allpass6.setDelayTimeSamples(44100 * 0.029291);
        allpass7.setDelayTimeSamples(44100 * 0.013458);
        allpass8.setDelayTimeSamples(44100 * 0.019123);
        
 
        delay1.setDelayTimeSamples(44100 * 0.153129);
        delay2.setDelayTimeSamples(44100 * 0.210389);
        delay3.setDelayTimeSamples(44100 * 0.127837);
        delay4.setDelayTimeSamples(44100 * 0.256891);
        delay5.setDelayTimeSamples(44100 * 0.174713);
        delay6.setDelayTimeSamples(44100 * 0.192303);
        delay7.setDelayTimeSamples(44100 * 0.125000);
        delay8.setDelayTimeSamples(44100 * 0.219991);

        
   
        
        reverbBuffer.setSize(8, samplesPerBlockExpected);
   
        for (int channel = 0; channel < 8; ++channel) {
            lowPassFilters[channel].setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, 4000 ));
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
        for (int channel = 0; channel < 8; ++channel) {
            int originalChannel = channel % 2;
            const float* bufferData = bufferToFill.buffer->getReadPointer(originalChannel);
            reverbBuffer.copyFrom(channel, 0, bufferData, bufferLength);

        }

        auto channelData1 = reverbBuffer.getWritePointer(0);
        auto channelData2 = reverbBuffer.getWritePointer(1);
        auto channelData3 = reverbBuffer.getWritePointer(2);
        auto channelData4 = reverbBuffer.getWritePointer(3);
        auto channelData5 = reverbBuffer.getWritePointer(4);
        auto channelData6 = reverbBuffer.getWritePointer(5);
        auto channelData7 = reverbBuffer.getWritePointer(6);
        auto channelData8 = reverbBuffer.getWritePointer(7);

            for (int sample = 0; sample < reverbBuffer.getNumSamples(); sample++) {
                
                
                //OUTPUT OF DELAY AND LOWPASS
                float one1 = delay1.popSample();
                float* onep = &one1;
                lowPassFilters[0].processSamples(onep, 1);
                //OUTPUT OF ALLPASS
                float temp11 = allpass1.popSample();
                //INPUT FOR THE ALLPASS
                float input1 = one1 * 0.2 + channelData1[sample] - 0.6 * temp11;
                // ALL THE INPUT SHOULD BE MULTIPLIED BY DECAY
                float feedforward_allpass1 = input1 * 0.6;
                //PUSH THE INPUT IN ALLPASS(WITHOUT THE DECAY OF FEEDFORWARD)
                allpass1.pushSample(input1);

                float two1 = delay2.popSample();
                float* twop = &two1;
                lowPassFilters[1].processSamples(twop, 1);
                float temp22 = allpass2.popSample();
                float input2 = two1 * 0.2  + channelData2[sample] + 0.6 * temp22;
                float feedforward_allpass2 = -(input2 * 0.6);
                allpass2.pushSample(input2);
 

                float three1 = delay3.popSample();
                float* threep = &three1;
                lowPassFilters[2].processSamples(threep, 1);
                float temp33 = allpass3.popSample();
                float input3 = three1 * 0.2 + channelData3[sample] - 0.6 * temp33;
                float feedforward_allpass3 = input3 * 0.6;
                allpass3.pushSample(input3);

                float four1 = delay4.popSample();
                float* fourp = &four1;
                lowPassFilters[3].processSamples(fourp, 1);
                float temp44 = allpass4.popSample();
                float input4 = four1 * 0.2  + channelData4[sample] + 0.6 * temp44;
                float feedforward_allpass4 = -(input4 * 0.6);
                allpass4.pushSample(input4);
 
                float five1 = delay5.popSample();
                float* fivep = &five1;
                lowPassFilters[4].processSamples(fivep, 1);
                float temp55 = allpass5.popSample();
                float input5 = five1 * 0.2 - 0.6 * temp55 + channelData5[sample];
                float feedforward_allpass5 = input5 * 0.6;
                allpass5.pushSample(input5);
              
                float six1 = delay6.popSample();
                float* sixp = &six1;
                lowPassFilters[5].processSamples(sixp, 1);
                float temp66 = allpass6.popSample();
                float input6 = six1 * 0.2 + 0.6 * temp66 + channelData6[sample];
                float feedforward_allpass6 = -(input6 * 0.6);
                allpass6.pushSample(input6);

                float seven1 = delay7.popSample();
                float* sevenp = &seven1;
                lowPassFilters[6].processSamples(sevenp, 1);
                float temp77 = allpass7.popSample();
                float input7 = seven1 * 0.2 - 0.6 * temp77 + channelData7[sample];
                float feedforward_allpass7 = input7 * 0.6;
                allpass7.pushSample(input7);

                float eigth1 = delay8.popSample();
                float* eigthp = &eigth1;
                lowPassFilters[7].processSamples(eigthp, 1);
                float temp88 = allpass8.popSample();
                float input8 = eigth1 * 0.2 + 0.6 * temp88 + channelData8[sample];
                float feedforward_allpass8 = -(input8 * 0.6);
                allpass8.pushSample(input8);


                //HERE I PREPARE AN ARRAY TO BE PROCESSED IN THE HADAMARD MATRIX THAT I HAVE SEEN IN FAUST
                array12[0] = temp11 + feedforward_allpass1;
                array12[1] = temp22 + feedforward_allpass2; 
                array12[2] = temp33 + feedforward_allpass3; 
                array12[3] = temp44 + feedforward_allpass4;
                array12[4] = temp55 + feedforward_allpass5;
                array12[5] = temp66 + feedforward_allpass6;
                array12[6] = temp77 + feedforward_allpass7;
                array12[7] = temp88 + feedforward_allpass8;
                hadamard(array12);
                
                float one = array12[0];
                float two = array12[1];
                float three = array12[2];
                float four = array12[3];
                float five = array12[4];
                float six = array12[5];
                float seven = array12[6];
                float eigth = array12[7];
                
                
                // HERE I SET THE 8 OUTPUTS
                channelData1[sample] = one;
                channelData2[sample] = two;
                channelData3[sample] = three;
                channelData4[sample] = four;
                channelData5[sample] = five;
                channelData6[sample] = six;
                channelData7[sample] = seven;
                channelData8[sample] = eigth;

                // I LOAD THE OUTPUTS IN THE DELAYS
                delay1.pushSample(one);
                delay2.pushSample(two) ;
                delay3.pushSample(three) ;
                delay4.pushSample(four) ;
                delay5.pushSample(five) ;
                delay6.pushSample(six) ;
                delay7.pushSample(seven) ;
                delay8.pushSample(eigth) ;

               


              
             
            }

        // HERE I PUT THE 8 OUTPUTS IN THE 2 CHANNELS OUTPUTS
        for (int channel = 0; channel < 8; ++channel) {
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

   

    float array12[8];
    juce::IIRFilter lowPassFilters[8];

    SDSP::DelayLine delay1;
    SDSP::DelayLine delay2;
    SDSP::DelayLine delay3;
    SDSP::DelayLine delay4;
    SDSP::DelayLine delay5;
    SDSP::DelayLine delay6;
    SDSP::DelayLine delay7;
    SDSP::DelayLine delay8;


    SDSP::APF allpass1;
    SDSP::APF allpass2;
    SDSP::APF allpass3;
    SDSP::APF allpass4;
    SDSP::APF allpass5;
    SDSP::APF allpass6;
    SDSP::APF allpass7;
    SDSP::APF allpass8;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
