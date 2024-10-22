//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// bmfproj1.h
//
// Code generation for function 'bmfproj1'
//

#ifndef BMFPROJ1_H
#define BMFPROJ1_H

// Include files
#include "bmfproj1_types1.h"
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct bmfproj1StackData;

// Type Definitions
class ParameterSmoother {
public:
  ParameterSmoother();
  ~ParameterSmoother();
  int isInitialized;
  double smoothingFactor;
  double currentValue;
  double targetValue;
};

namespace coder {
class noiseGate {
public:
  noiseGate();
  ~noiseGate();
  boolean_T matlabCodegenIsDeleted;
  int isInitialized;
  boolean_T isSetupComplete;
  boolean_T TunablePropsChanged;
  cell_wrap_3 inputVarSize[1];
  double pSampleRateDialog;
  double Threshold;
  double AttackTime;
  double ReleaseTime;
  double pNumChannels;
  double pAlphaA;
  double pAlphaR;
  double pLevelDetectionState;
  double HoldTime;
  double pHoldTimeState;
  double pHoldTimeSamples;
};

} // namespace coder
class derivedAudioPlugin {
public:
  void updateLPF();
  void updateHS();
  void updateHF();
  void updateMF();
  void updateLF();
  void updateLS();
  void updateHPF();
  derivedAudioPlugin();
  ~derivedAudioPlugin();
  boolean_T matlabCodegenIsDeleted;
  double PrivateSampleRate;
  int PrivateLatency;
  coder::array<char, 2U> BYPASS;
  double THRESHOLD;
  double ATTACK;
  double RELEASE;
  double HOLD;
  double LPF_FREQ;
  double LPF_Q;
  double HS_FREQ;
  double HS_GAIN;
  double HS_Q;
  double HF_FREQ;
  double HF_GAIN;
  double HF_Q;
  double MF_FREQ;
  double MF_GAIN;
  double MF_Q;
  double LF_FREQ;
  double LF_GAIN;
  double LF_Q;
  double LS_FREQ;
  double LS_GAIN;
  double LS_Q;
  double HPF_FREQ;
  double HPF_Q;
  struct_T filter_LPF;
  struct_T filter_HS;
  struct_T filter_HF;
  struct_T filter_MF;
  struct_T filter_LF;
  struct_T filter_LS;
  struct_T filter_HPF;
  ParameterSmoother THRESHOLD_Smoother;
  ParameterSmoother ATTACK_Smoother;
  ParameterSmoother RELEASE_Smoother;
  ParameterSmoother HOLD_Smoother;
  ParameterSmoother LPF_FreqSmoother;
  ParameterSmoother HS_FreqSmoother;
  ParameterSmoother HF_FreqSmoother;
  ParameterSmoother MF_FreqSmoother;
  ParameterSmoother LF_FreqSmoother;
  ParameterSmoother LS_FreqSmoother;
  ParameterSmoother HPF_FreqSmoother;
  ParameterSmoother HS_GainSmoother;
  ParameterSmoother HF_GainSmoother;
  ParameterSmoother MF_GainSmoother;
  ParameterSmoother LF_GainSmoother;
  ParameterSmoother LS_GainSmoother;
  ParameterSmoother LPF_QSmoother;
  ParameterSmoother HS_QSmoother;
  ParameterSmoother HF_QSmoother;
  ParameterSmoother MF_QSmoother;
  ParameterSmoother LF_QSmoother;
  ParameterSmoother LS_QSmoother;
  ParameterSmoother HPF_QSmoother;
  double fs;
  double numChannels;
  coder::noiseGate gate;
};

// Function Declarations
extern void bmfproj1_initialize(bmfproj1StackData *SD);

extern void bmfproj1_terminate(bmfproj1StackData *SD);

extern void createPluginInstance(bmfproj1StackData *SD, unsigned long thisPtr);

extern int getLatencyInSamplesCImpl(bmfproj1StackData *SD);

extern void onParamChangeCImpl(bmfproj1StackData *SD, int paramIdx,
                               double b_value);

extern void processEntryPoint(bmfproj1StackData *SD, double samplesPerFrame,
                              const double i1_data[], const int i1_size[1],
                              const double i2_data[], const int i2_size[1],
                              double o1_data[], int o1_size[1],
                              double o2_data[], int o2_size[1]);

extern void resetCImpl(bmfproj1StackData *SD, double rate);

#endif
// End of code generation (bmfproj1.h)
