/*
  ==============================================================================

    matrix.h
    Created: 25 Nov 2023 4:48:06pm
    Author:  antof

  ==============================================================================
*/




#pragma once 
#include <cmath>

namespace SDSP
{
    // Use like `Householder<double, 8>::inPlace(data)` - size must be ≥ 1
    template<typename Sample, int size>
    class [[maybe_unused]] Householder {
        static constexpr Sample multiplier{ -2.0 / size };
    public:
        static void inPlace(Sample* arr) {
            Sample sum = 0;
            for (int i = 0; i < size; ++i) {
                sum += arr[i];
            }
            sum *= multiplier;
            for (int i = 0; i < size; ++i) {
                arr[i] += sum;
            }
        }
    };

    // Use like `Hadamard<double, 8>::inPlace(data)` - size must be a power of 2
    template<typename Sample, int size>
    class [[maybe_unused]] Hadamard {
    public:
        static inline void recursiveUnscaled(Sample* data) {
            if constexpr (size <= 1) return;
            constexpr int hSize = size / 2;
            // Two (unscaled) Hadamards of half the size
            Hadamard<Sample, hSize>::recursiveUnscaled(data);
            Hadamard<Sample, hSize>::recursiveUnscaled(data + hSize);
            // Combine the two halves using sum/difference
            for (int i = 0; i < hSize; ++i) {
                Sample a = data[i];
                Sample b = data[i + hSize];
                data[i] = (a + b);
                data[i + hSize] = (a - b);
            }
        }
        static inline void inPlace(Sample* data) {
            recursiveUnscaled(data);
            auto scalingFactor = static_cast<Sample>(std::sqrt(1.0 / size));
            for (int c = 0; c < size; ++c) {
                data[c] *= scalingFactor;
            }
        }
    };


    class [[maybe_unused]] APFBase {
    public:
        [[maybe_unused]] float tap(const int index) {
            return m_delayLine.popSample(0, static_cast<float>(index), false);
        }
        [[maybe_unused]] void setDelayTimeSamples(int newDelayTimeSamples) {
            m_delayTimeSamples = newDelayTimeSamples;
            if (m_hasBeenPrepared) {
                m_delayLine.setDelay(static_cast<float>(m_delayTimeSamples));
            }
        }

        [[maybe_unused]] void setCoeff(float newCoeff) {
            m_coeff = newCoeff;
            
        }

        [[maybe_unused]] float popSample() {
            if (m_hasBeenPrepared) {
                return(m_delayLine.popSample(0));
                
            }
        }


        [[maybe_unused]] void pushSample(float sample) {
            samplee = sample;
            if (m_hasBeenPrepared) {
                m_delayLine.pushSample(0, samplee);
            }
        }

        

        virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
            m_sampleRate = sampleRate;
            m_hasBeenPrepared = true;
            juce::dsp::ProcessSpec spec{ sampleRate, static_cast<juce::uint64>(samplesPerBlockExpected), 1 };
            m_delayLine.prepare(spec);
            // Allow pad for delay modulation
            m_delayLine.setMaximumDelayInSamples(static_cast<int>(2048));
            m_delayLine.setDelay(static_cast<float>(m_delayTimeSamples));
            float m_coef;
            float samplee;
            float channell;
        }

        virtual float processSample(float in) = 0;

        

        void releaseResources()
        {
            m_delayLine.reset();

        }

        virtual void clearBuffers() noexcept = 0;

    protected:
        double m_sampleRate{ 44100 };
        bool m_hasBeenPrepared{ false };
        int m_delayTimeSamples{ 0 };
        float maxdelayy{ 0 };
        juce::dsp::DelayLine<float> m_delayLine;
        float m_coeff{ 0.6f };
        float samplee;

    };


    class [[maybe_unused]] DelayBase {
    public:
        [[maybe_unused]] float tap(const int index) {
            return m_delayLine.popSample(0, static_cast<float>(index), false);
        }
        [[maybe_unused]] void setDelayTimeSamples(int newDelayTimeSamples) {
            m_delayTimeSamples = newDelayTimeSamples;
            if (m_hasBeenPrepared) {
                m_delayLine.setDelay(static_cast<float>(m_delayTimeSamples));
            }
        }


        [[maybe_unused]] float popSample() {
            if (m_hasBeenPrepared) {
                return (m_delayLine.popSample(0));
            }
        }


        [[maybe_unused]] void pushSample(float sample) {
            samplee = sample;
            if (m_hasBeenPrepared) {
                m_delayLine.pushSample(0, samplee);
            }
        }

        

        virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
            m_sampleRate = sampleRate;
            m_hasBeenPrepared = true;
            juce::dsp::ProcessSpec spec{ sampleRate, static_cast<juce::uint64>(samplesPerBlockExpected), 1 };
            m_delayLine.prepare(spec);
            // Allow pad for delay modulation
            m_delayLine.setMaximumDelayInSamples(static_cast<int>(16384));
            m_delayLine.setDelay(static_cast<float>(m_delayTimeSamples));

            
        }

        virtual float processSample(float in) = 0;

        void releaseResources()
        {

        }

        virtual void clearBuffers() noexcept = 0;

    protected:
        double m_sampleRate{ 44100 };
        bool m_hasBeenPrepared{ false };
        int m_delayTimeSamples{ 0 };
        juce::dsp::DelayLine<float> m_delayLine;
        float samplee;
        float channell;
        

    };



    class [[maybe_unused]] APF : public APFBase {
    public:
        float processSample(float in) override
        {
            /*auto delayed = m_delayLine.popSample(0);
            auto feedforward = in * m_coeff;
            in += (delayed * -m_coeff);
            m_delayLine.pushSample(0, in);
            return (delayed + feedforward );
            //return (delayed);*/
            return 0;
        }

        void clearBuffers() noexcept override {
            m_delayLine.reset();
        }
    };

    class [[maybe_unused]] Delay : public DelayBase {
    public:
        float processSample(float in) override
        {
            /*auto delayed = m_delayLine.popSample(0);
            in += (delayed * -0.2);
            m_delayLine.pushSample(0, in);
            return delayed;*/
            return 0;
        }

        void clearBuffers() noexcept override {
            m_delayLine.reset();
        }
    };

    class [[maybe_unused]] DelayLine : public DelayBase {
    public:
        float processSample(float in) override
        {
            /*auto delayed = m_delayLine.popSample(0);
            in += (delayed * -m_coeff);
            m_delayLine.pushSample(0, in);
            return delayed;*/ 
            /*m_delayLine.pushSample(0, in);
            auto delayed = m_delayLine.popSample(0);
            //m_delayLine.pushSample(0, in);
            return delayed;*/

            return 0;
        }

        void clearBuffers() noexcept override {
            m_delayLine.reset();
        }
    };
}
