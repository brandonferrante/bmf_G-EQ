#include "../JuceLibraryCode/JuceHeader.h"

#include "bmfproj1.h"
#include "bmfproj1_types.h"

#if JUCE_VERSION >= 0x050400
using Parameter = AudioProcessorValueTreeState::Parameter;
#endif

struct onParamChangeListener : AudioProcessorValueTreeState::Listener
{
    onParamChangeListener(bmfproj1StackData* sd) : SD(sd)
    {
    }

    void parameterChanged (const String& parameterID, float newValue) override
    {
        const int idx = getParameterIndex(parameterID);
        onParamChangeCImpl(SD, idx, static_cast<double>(newValue));
    }
    
private:
    int getParameterIndex (const String& parameterID)
    {
        (void)parameterID;
    
        if (parameterID == "BYPASS") {
            return 0;
        }
        if (parameterID == "LPF_FREQ") {
            return 1;
        }
        if (parameterID == "LPF_Q") {
            return 2;
        }
        if (parameterID == "HS_FREQ") {
            return 3;
        }
        if (parameterID == "HS_GAIN") {
            return 4;
        }
        if (parameterID == "HS_Q") {
            return 5;
        }
        if (parameterID == "HF_FREQ") {
            return 6;
        }
        if (parameterID == "HF_GAIN") {
            return 7;
        }
        if (parameterID == "HF_Q") {
            return 8;
        }
        if (parameterID == "MF_FREQ") {
            return 9;
        }
        if (parameterID == "MF_GAIN") {
            return 10;
        }
        if (parameterID == "MF_Q") {
            return 11;
        }
        if (parameterID == "LF_FREQ") {
            return 12;
        }
        if (parameterID == "LF_GAIN") {
            return 13;
        }
        if (parameterID == "LF_Q") {
            return 14;
        }
        if (parameterID == "LS_FREQ") {
            return 15;
        }
        if (parameterID == "LS_GAIN") {
            return 16;
        }
        if (parameterID == "LS_Q") {
            return 17;
        }
        if (parameterID == "HPF_FREQ") {
            return 18;
        }
        if (parameterID == "HPF_Q") {
            return 19;
        }
        if (parameterID == "THRESHOLD") {
            return 20;
        }
        if (parameterID == "ATTACK") {
            return 21;
        }
        if (parameterID == "HOLD") {
            return 22;
        }
        if (parameterID == "RELEASE") {
            return 23;
        }
    
        return (-1);
    }    
    
    bmfproj1StackData *SD;
};

//==============================================================================
class bmfproj1AudioProcessor  : public AudioProcessor
{
    //==============================================================================
#if JUCE_VERSION >= 0x050400
    const StringArray m_choices1;

public:
    bmfproj1AudioProcessor()
        : paramListener(&mStackData),
          m_choices1({ "OFF", "ON" }),
          parameters(*this, nullptr, "bmfproj1", {
                std::make_unique<Parameter>("BYPASS", juce::String::fromUTF8(u8"Bypass"), juce::String::fromUTF8(u8""),
                    NormalisableRange<float>(0.f, m_choices1.size()-1.f, 1.f), 0.f,
                    [=](float value) { return m_choices1[(int) (value + 0.5)]; },
                    [=](const String& text) { return (float) m_choices1.indexOf(text); }, false, true, true),
                std::make_unique<Parameter>("LPF_FREQ", juce::String::fromUTF8(u8"LPF Freq"), juce::String::fromUTF8(u8"Hz"),
NormalisableRange<float>(20.f,20000.f,[](float min, float max, float norm) {return min*powf(max/min,norm);}, [](float min, float max, float val) {return logf(val/min)/logf(max/min);}), 20000.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("LPF_Q", juce::String::fromUTF8(u8"LPF Q"), juce::String::fromUTF8(u8""),
NormalisableRange<float>(0.1f,10.f), 0.71f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("HS_FREQ", juce::String::fromUTF8(u8"HS Freq"), juce::String::fromUTF8(u8"Hz"),
NormalisableRange<float>(10000.f,20000.f,[](float min, float max, float norm) {return min*powf(max/min,norm);}, [](float min, float max, float val) {return logf(val/min)/logf(max/min);}), 10000.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("HS_GAIN", juce::String::fromUTF8(u8"HS Gain"), juce::String::fromUTF8(u8"dB"),
NormalisableRange<float>(-12.f,12.f), 0.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("HS_Q", juce::String::fromUTF8(u8"HS Q"), juce::String::fromUTF8(u8""),
NormalisableRange<float>(0.1f,2.f), 1.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("HF_FREQ", juce::String::fromUTF8(u8"HF Freq"), juce::String::fromUTF8(u8"Hz"),
NormalisableRange<float>(8000.f,20000.f,[](float min, float max, float norm) {return min*powf(max/min,norm);}, [](float min, float max, float val) {return logf(val/min)/logf(max/min);}), 8000.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("HF_GAIN", juce::String::fromUTF8(u8"HF Gain"), juce::String::fromUTF8(u8"dB"),
NormalisableRange<float>(-12.f,12.f), 0.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("HF_Q", juce::String::fromUTF8(u8"HF Q"), juce::String::fromUTF8(u8""),
NormalisableRange<float>(0.1f,30.f), 0.5f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("MF_FREQ", juce::String::fromUTF8(u8"MF Freq"), juce::String::fromUTF8(u8"Hz"),
NormalisableRange<float>(500.f,10000.f,[](float min, float max, float norm) {return min*powf(max/min,norm);}, [](float min, float max, float val) {return logf(val/min)/logf(max/min);}), 1000.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("MF_GAIN", juce::String::fromUTF8(u8"MF Gain"), juce::String::fromUTF8(u8"Hz"),
NormalisableRange<float>(-12.f,12.f), 0.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("MF_Q", juce::String::fromUTF8(u8"MF Q"), juce::String::fromUTF8(u8""),
NormalisableRange<float>(0.1f,30.f), 0.5f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("LF_FREQ", juce::String::fromUTF8(u8"LF Freq"), juce::String::fromUTF8(u8"Hz"),
NormalisableRange<float>(20.f,1000.f,[](float min, float max, float norm) {return min*powf(max/min,norm);}, [](float min, float max, float val) {return logf(val/min)/logf(max/min);}), 500.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("LF_GAIN", juce::String::fromUTF8(u8"LF Gain"), juce::String::fromUTF8(u8"dB"),
NormalisableRange<float>(-12.f,12.f), 0.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("LF_Q", juce::String::fromUTF8(u8"LF Q"), juce::String::fromUTF8(u8""),
NormalisableRange<float>(0.1f,30.f), 0.5f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("LS_FREQ", juce::String::fromUTF8(u8"LS Freq"), juce::String::fromUTF8(u8"Hz"),
NormalisableRange<float>(20.f,2000.f,[](float min, float max, float norm) {return min*powf(max/min,norm);}, [](float min, float max, float val) {return logf(val/min)/logf(max/min);}), 200.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("LS_GAIN", juce::String::fromUTF8(u8"LS Gain"), juce::String::fromUTF8(u8"dB"),
NormalisableRange<float>(-12.f,12.f), 0.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("LS_Q", juce::String::fromUTF8(u8"LS Q"), juce::String::fromUTF8(u8""),
NormalisableRange<float>(0.1f,2.f), 1.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("HPF_FREQ", juce::String::fromUTF8(u8"HPF Freq"), juce::String::fromUTF8(u8"Hz"),
NormalisableRange<float>(20.f,20000.f,[](float min, float max, float norm) {return min*powf(max/min,norm);}, [](float min, float max, float val) {return logf(val/min)/logf(max/min);}), 20.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("HPF_Q", juce::String::fromUTF8(u8"HPF Q"), juce::String::fromUTF8(u8""),
NormalisableRange<float>(0.1f,10.f), 0.71f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("THRESHOLD", juce::String::fromUTF8(u8"Threshold"), juce::String::fromUTF8(u8"dB"),
NormalisableRange<float>(-80.f,0.f), -50.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("ATTACK", juce::String::fromUTF8(u8"Attack"), juce::String::fromUTF8(u8"ms"),
NormalisableRange<float>(0.f,500.f), 5.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("HOLD", juce::String::fromUTF8(u8"Hold"), juce::String::fromUTF8(u8"ms"),
NormalisableRange<float>(0.f,1000.f), 10.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("RELEASE", juce::String::fromUTF8(u8"Release"), juce::String::fromUTF8(u8"ms"),
NormalisableRange<float>(0.f,1000.f), 100.f, [](float val) {return String(val, 3);}, nullptr) })

    {
        mStackData.pd = &mPersistentData;
        
        bmfproj1_initialize(&mStackData);

        createPluginInstance(&mStackData, reinterpret_cast<unsigned long long>(this));

        parameters.addParameterListener("BYPASS", &paramListener);
        parameters.addParameterListener("LPF_FREQ", &paramListener);
        parameters.addParameterListener("LPF_Q", &paramListener);
        parameters.addParameterListener("HS_FREQ", &paramListener);
        parameters.addParameterListener("HS_GAIN", &paramListener);
        parameters.addParameterListener("HS_Q", &paramListener);
        parameters.addParameterListener("HF_FREQ", &paramListener);
        parameters.addParameterListener("HF_GAIN", &paramListener);
        parameters.addParameterListener("HF_Q", &paramListener);
        parameters.addParameterListener("MF_FREQ", &paramListener);
        parameters.addParameterListener("MF_GAIN", &paramListener);
        parameters.addParameterListener("MF_Q", &paramListener);
        parameters.addParameterListener("LF_FREQ", &paramListener);
        parameters.addParameterListener("LF_GAIN", &paramListener);
        parameters.addParameterListener("LF_Q", &paramListener);
        parameters.addParameterListener("LS_FREQ", &paramListener);
        parameters.addParameterListener("LS_GAIN", &paramListener);
        parameters.addParameterListener("LS_Q", &paramListener);
        parameters.addParameterListener("HPF_FREQ", &paramListener);
        parameters.addParameterListener("HPF_Q", &paramListener);
        parameters.addParameterListener("THRESHOLD", &paramListener);
        parameters.addParameterListener("ATTACK", &paramListener);
        parameters.addParameterListener("HOLD", &paramListener);
        parameters.addParameterListener("RELEASE", &paramListener);

    }
    //==============================================================================
#else // For JUCE prior to 5.4.0
public:
    bmfproj1AudioProcessor()
    :   paramListener(&mStackData), parameters (*this, nullptr)
    {
        mStackData.pd = &mPersistentData;
        
        bmfproj1_initialize(&mStackData);

        createPluginInstance(&mStackData, reinterpret_cast<unsigned long long>(this));

        //
        // Parameter property BYPASS
        //
        const StringArray choices1({ "OFF", "ON" });
        parameters.createAndAddParameter ("BYPASS", juce::String::fromUTF8(u8"Bypass"), juce::String::fromUTF8(u8""),
            NormalisableRange<float>(0.f, choices1.size()-1.f, 1.f), 0.f,
            [=](float value) { return choices1[(int) (value + 0.5)]; },
            [=](const String& text) { return (float) choices1.indexOf(text); },
            false, true, true);
        parameters.addParameterListener("BYPASS", &paramListener);

        //
        // Parameter property LPF_FREQ
        //
        parameters.createAndAddParameter ("LPF_FREQ", juce::String::fromUTF8(u8"LPF Freq"), juce::String::fromUTF8(u8"Hz"),
            NormalisableRange<float>(20.f, 20000.f, 
                [](float min, float max, float norm) {return min * powf(max/min, norm);},
                [](float min, float max, float val) {return logf(val/min)/logf(max/min);} ),
            20000.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("LPF_FREQ", &paramListener);

        //
        // Parameter property LPF_Q
        //
        parameters.createAndAddParameter ("LPF_Q", juce::String::fromUTF8(u8"LPF Q"), juce::String::fromUTF8(u8""),
            NormalisableRange<float>(0.1f, 10.f), 0.71f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("LPF_Q", &paramListener);

        //
        // Parameter property HS_FREQ
        //
        parameters.createAndAddParameter ("HS_FREQ", juce::String::fromUTF8(u8"HS Freq"), juce::String::fromUTF8(u8"Hz"),
            NormalisableRange<float>(10000.f, 20000.f, 
                [](float min, float max, float norm) {return min * powf(max/min, norm);},
                [](float min, float max, float val) {return logf(val/min)/logf(max/min);} ),
            10000.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("HS_FREQ", &paramListener);

        //
        // Parameter property HS_GAIN
        //
        parameters.createAndAddParameter ("HS_GAIN", juce::String::fromUTF8(u8"HS Gain"), juce::String::fromUTF8(u8"dB"),
            NormalisableRange<float>(-12.f, 12.f), 0.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("HS_GAIN", &paramListener);

        //
        // Parameter property HS_Q
        //
        parameters.createAndAddParameter ("HS_Q", juce::String::fromUTF8(u8"HS Q"), juce::String::fromUTF8(u8""),
            NormalisableRange<float>(0.1f, 2.f), 1.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("HS_Q", &paramListener);

        //
        // Parameter property HF_FREQ
        //
        parameters.createAndAddParameter ("HF_FREQ", juce::String::fromUTF8(u8"HF Freq"), juce::String::fromUTF8(u8"Hz"),
            NormalisableRange<float>(8000.f, 20000.f, 
                [](float min, float max, float norm) {return min * powf(max/min, norm);},
                [](float min, float max, float val) {return logf(val/min)/logf(max/min);} ),
            8000.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("HF_FREQ", &paramListener);

        //
        // Parameter property HF_GAIN
        //
        parameters.createAndAddParameter ("HF_GAIN", juce::String::fromUTF8(u8"HF Gain"), juce::String::fromUTF8(u8"dB"),
            NormalisableRange<float>(-12.f, 12.f), 0.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("HF_GAIN", &paramListener);

        //
        // Parameter property HF_Q
        //
        parameters.createAndAddParameter ("HF_Q", juce::String::fromUTF8(u8"HF Q"), juce::String::fromUTF8(u8""),
            NormalisableRange<float>(0.1f, 30.f), 0.5f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("HF_Q", &paramListener);

        //
        // Parameter property MF_FREQ
        //
        parameters.createAndAddParameter ("MF_FREQ", juce::String::fromUTF8(u8"MF Freq"), juce::String::fromUTF8(u8"Hz"),
            NormalisableRange<float>(500.f, 10000.f, 
                [](float min, float max, float norm) {return min * powf(max/min, norm);},
                [](float min, float max, float val) {return logf(val/min)/logf(max/min);} ),
            1000.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("MF_FREQ", &paramListener);

        //
        // Parameter property MF_GAIN
        //
        parameters.createAndAddParameter ("MF_GAIN", juce::String::fromUTF8(u8"MF Gain"), juce::String::fromUTF8(u8"Hz"),
            NormalisableRange<float>(-12.f, 12.f), 0.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("MF_GAIN", &paramListener);

        //
        // Parameter property MF_Q
        //
        parameters.createAndAddParameter ("MF_Q", juce::String::fromUTF8(u8"MF Q"), juce::String::fromUTF8(u8""),
            NormalisableRange<float>(0.1f, 30.f), 0.5f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("MF_Q", &paramListener);

        //
        // Parameter property LF_FREQ
        //
        parameters.createAndAddParameter ("LF_FREQ", juce::String::fromUTF8(u8"LF Freq"), juce::String::fromUTF8(u8"Hz"),
            NormalisableRange<float>(20.f, 1000.f, 
                [](float min, float max, float norm) {return min * powf(max/min, norm);},
                [](float min, float max, float val) {return logf(val/min)/logf(max/min);} ),
            500.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("LF_FREQ", &paramListener);

        //
        // Parameter property LF_GAIN
        //
        parameters.createAndAddParameter ("LF_GAIN", juce::String::fromUTF8(u8"LF Gain"), juce::String::fromUTF8(u8"dB"),
            NormalisableRange<float>(-12.f, 12.f), 0.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("LF_GAIN", &paramListener);

        //
        // Parameter property LF_Q
        //
        parameters.createAndAddParameter ("LF_Q", juce::String::fromUTF8(u8"LF Q"), juce::String::fromUTF8(u8""),
            NormalisableRange<float>(0.1f, 30.f), 0.5f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("LF_Q", &paramListener);

        //
        // Parameter property LS_FREQ
        //
        parameters.createAndAddParameter ("LS_FREQ", juce::String::fromUTF8(u8"LS Freq"), juce::String::fromUTF8(u8"Hz"),
            NormalisableRange<float>(20.f, 2000.f, 
                [](float min, float max, float norm) {return min * powf(max/min, norm);},
                [](float min, float max, float val) {return logf(val/min)/logf(max/min);} ),
            200.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("LS_FREQ", &paramListener);

        //
        // Parameter property LS_GAIN
        //
        parameters.createAndAddParameter ("LS_GAIN", juce::String::fromUTF8(u8"LS Gain"), juce::String::fromUTF8(u8"dB"),
            NormalisableRange<float>(-12.f, 12.f), 0.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("LS_GAIN", &paramListener);

        //
        // Parameter property LS_Q
        //
        parameters.createAndAddParameter ("LS_Q", juce::String::fromUTF8(u8"LS Q"), juce::String::fromUTF8(u8""),
            NormalisableRange<float>(0.1f, 2.f), 1.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("LS_Q", &paramListener);

        //
        // Parameter property HPF_FREQ
        //
        parameters.createAndAddParameter ("HPF_FREQ", juce::String::fromUTF8(u8"HPF Freq"), juce::String::fromUTF8(u8"Hz"),
            NormalisableRange<float>(20.f, 20000.f, 
                [](float min, float max, float norm) {return min * powf(max/min, norm);},
                [](float min, float max, float val) {return logf(val/min)/logf(max/min);} ),
            20.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("HPF_FREQ", &paramListener);

        //
        // Parameter property HPF_Q
        //
        parameters.createAndAddParameter ("HPF_Q", juce::String::fromUTF8(u8"HPF Q"), juce::String::fromUTF8(u8""),
            NormalisableRange<float>(0.1f, 10.f), 0.71f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("HPF_Q", &paramListener);

        //
        // Parameter property THRESHOLD
        //
        parameters.createAndAddParameter ("THRESHOLD", juce::String::fromUTF8(u8"Threshold"), juce::String::fromUTF8(u8"dB"),
            NormalisableRange<float>(-80.f, 0.f), -50.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("THRESHOLD", &paramListener);

        //
        // Parameter property ATTACK
        //
        parameters.createAndAddParameter ("ATTACK", juce::String::fromUTF8(u8"Attack"), juce::String::fromUTF8(u8"ms"),
            NormalisableRange<float>(0.f, 500.f), 5.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("ATTACK", &paramListener);

        //
        // Parameter property HOLD
        //
        parameters.createAndAddParameter ("HOLD", juce::String::fromUTF8(u8"Hold"), juce::String::fromUTF8(u8"ms"),
            NormalisableRange<float>(0.f, 1000.f), 10.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("HOLD", &paramListener);

        //
        // Parameter property RELEASE
        //
        parameters.createAndAddParameter ("RELEASE", juce::String::fromUTF8(u8"Release"), juce::String::fromUTF8(u8"ms"),
            NormalisableRange<float>(0.f, 1000.f), 100.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("RELEASE", &paramListener);

        parameters.state = ValueTree(Identifier("bmfproj1"));
    }
#endif

    //==============================================================================
    ~bmfproj1AudioProcessor()
    {
        bmfproj1_terminate(&mStackData);
    }
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        (void)samplesPerBlock;
        resetCImpl(&mStackData, sampleRate);
        setLatencySamples(getLatencyInSamplesCImpl(&mStackData));
    }

    void releaseResources() override                { }
    
    
    void processBlock (AudioBuffer<double>& buffer, MidiBuffer&) override
    {
        ScopedNoDenormals noDenormals;
        const int nSamples = buffer.getNumSamples();
        const int nChannels = buffer.getNumChannels();
        const double* const* pin = buffer.getArrayOfReadPointers();
        double* const* pout = buffer.getArrayOfWritePointers();
        std::vector<const double*> inputs(pin,pin+nChannels);
        std::vector<double*> outputs(pout,pout+nChannels);
        int i_;

        bmfproj1StackData *SD = &mStackData;

        if (nChannels < 2) {
            const int numExtraChannels = 2 - nChannels;
            tempBuffer.setSize(numExtraChannels, nSamples);
            if (nChannels < 2) {
                tempBuffer.clear(0, nSamples);
                const double* const* p = tempBuffer.getArrayOfReadPointers();
                std::copy(p, p+numExtraChannels, std::back_inserter(inputs));
            }
            if (nChannels < 2) {
                double* const* p = tempBuffer.getArrayOfWritePointers();
                std::copy(p, p+numExtraChannels, std::back_inserter(outputs));
            }
        }

        int osz0_;
        int osz1_;
        if (nSamples <= MAX_SAMPLES_PER_FRAME) {
            /* Fast path for common frame sizes. */
            const int isz0_ = nSamples;
            const int isz1_ = nSamples;
            processEntryPoint(SD, (double)nSamples,
                    inputs[0], &isz0_,
                    inputs[1], &isz1_,
                    outputs[0], &osz0_,
                    outputs[1], &osz1_);
        } else {
            /* Fallback for unusually large frames. */
            int isz0_ = MAX_SAMPLES_PER_FRAME;
            int isz1_ = MAX_SAMPLES_PER_FRAME;
            int n = MAX_SAMPLES_PER_FRAME;
            for (i_ = 0; i_ < nSamples; i_ += MAX_SAMPLES_PER_FRAME) {
                if (i_ + MAX_SAMPLES_PER_FRAME > nSamples) {
                    n = nSamples - i_;
                    isz0_ = nSamples - i_;
                    isz1_ = nSamples - i_;
                }
                processEntryPoint(SD, (double)n,
                        inputs[0]+i_, &isz0_,
                        inputs[1]+i_, &isz1_,
                        outputs[0]+i_, &osz0_,
                        outputs[1]+i_, &osz1_);
            }
        }

    }
    
    void processBlock (AudioBuffer<float>& buffer,  MidiBuffer& midiMessages) override
    {
        AudioBuffer<double> doubleBuffer;
        doubleBuffer.makeCopyOf(buffer);
        processBlock(doubleBuffer, midiMessages);
        buffer.makeCopyOf(doubleBuffer);
    }
    
    //==============================================================================
    bool hasEditor() const override                 { return true; }
    AudioProcessorEditor* createEditor() override;
    
    //==============================================================================
    const String getName() const override           { return JucePlugin_Name; }

    bool acceptsMidi() const override               { return false; }
    bool producesMidi() const override              { return false; }
    bool isMidiEffect () const override             { return false; }
    double getTailLengthSeconds() const override    { return 0.0;   }

    //==============================================================================
    // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
    int getNumPrograms() override                       { return 1;  }
    int getCurrentProgram() override                    { return 0;  }
    void setCurrentProgram (int index) override         { (void) index; }
    const String getProgramName (int index) override    { (void) index; return {}; }
    void changeProgramName (int index, const String& newName) override  { (void) index; (void) newName; }
    
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override
    {
        auto xml (parameters.state.createXml());
        copyXmlToBinary (*xml, destData);
    }
    
    void setStateInformation (const void* data, int sizeInBytes) override
    {
        auto xmlState (getXmlFromBinary (data, sizeInBytes));
        if (xmlState != nullptr)
            if (xmlState->hasTagName (parameters.state.getType()))
                parameters.state = ValueTree::fromXml (*xmlState);
    }
    
    bool supportsDoublePrecisionProcessing() const override  { return true; }
    
private:
    //==============================================================================
    static const int MAX_SAMPLES_PER_FRAME = 4096;

    bmfproj1StackData mStackData;
    bmfproj1PersistentData mPersistentData;
    onParamChangeListener paramListener;
    AudioBuffer<double> tempBuffer;
    
    //==============================================================================
    AudioProcessorValueTreeState parameters;
 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (bmfproj1AudioProcessor)
};

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new bmfproj1AudioProcessor();
}

#include "bmfproj1PluginEditor.h"

AudioProcessorEditor* bmfproj1AudioProcessor::createEditor()
{
    return new bmfproj1AudioProcessorEditor(*this, parameters);
}

