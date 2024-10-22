//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// bmfproj1.cpp
//
// Code generation for function 'bmfproj1'
//

// Include files
#include "bmfproj1.h"
#include "bmfproj1_types.h"
#include "bmfproj1_types1.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Type Definitions
class bmfproj1 {
public:
  static int processBiquad(const double x_data[], int x_size,
                           const double filt_w[4], double filt_a0,
                           double filt_a1, double filt_a2, double filt_b0,
                           double filt_b1, double filt_b2, double ch,
                           double y_data[], double &w);
};

class audioPlugin {
public:
  static void wormholeToConstructor_init(bmfproj1StackData *SD);
};

// Function Declarations
static derivedAudioPlugin *getPluginInstance(bmfproj1StackData *SD);

static void getPluginInstance_delete(bmfproj1StackData *SD);

static void getPluginInstance_init(bmfproj1StackData *SD);

static void getPluginInstance_new(bmfproj1StackData *SD);

static double rt_powd_snf(double u0, double u1);

// Function Definitions
int bmfproj1::processBiquad(const double x_data[], int x_size,
                            const double filt_w[4], double filt_a0,
                            double filt_a1, double filt_a2, double filt_b0,
                            double filt_b1, double filt_b2, double ch,
                            double y_data[], double &w)
{
  double a0;
  double a1;
  double a2;
  double b0;
  double b1;
  double w1;
  double w2;
  int w1_tmp;
  int y_size;
  //  Process biquad filter
  //  Initialize memory
  y_size = x_size;
  if (x_size - 1 >= 0) {
    std::memset(&y_data[0], 0,
                static_cast<unsigned int>(x_size) * sizeof(double));
  }
  w = 0.0;
  w1_tmp = (static_cast<int>(ch) - 1) << 1;
  w1 = filt_w[w1_tmp];
  //  w(n-1)
  w2 = filt_w[w1_tmp + 1];
  //  w(n-2)
  //  Set coefficients
  b0 = filt_b0;
  if (std::abs(filt_b0) < 1.0E-6) {
    b0 = 1.0E-6;
    //  prevent division by a small value
  }
  //  a = feedforward, b = feedback
  a0 = filt_a0 / b0;
  a1 = filt_a1 / b0;
  a2 = filt_a2 / b0;
  b1 = filt_b1 / b0;
  b0 = filt_b2 / b0;
  //  Process by sample
  for (w1_tmp = 0; w1_tmp < x_size; w1_tmp++) {
    w = (x_data[w1_tmp] - b1 * w1) - b0 * w2;
    //  feedback
    y_data[w1_tmp] = (a0 * w + a1 * w1) + a2 * w2;
    //  feedforward
    w2 = w1;
    w1 = w;
  }
  //  Update filter states
  return y_size;
}

void derivedAudioPlugin::updateHF()
{
  double A;
  double alpha;
  double d;
  double w0;
  //  Update High Peak Filter (HF)
  if (HF_FreqSmoother.isInitialized != 1) {
    HF_FreqSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  HF_FreqSmoother.currentValue =
      HF_FreqSmoother.smoothingFactor * HF_FreqSmoother.currentValue +
      (1.0 - HF_FreqSmoother.smoothingFactor) * HF_FreqSmoother.targetValue;
  if (HF_QSmoother.isInitialized != 1) {
    HF_QSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  HF_QSmoother.currentValue =
      HF_QSmoother.smoothingFactor * HF_QSmoother.currentValue +
      (1.0 - HF_QSmoother.smoothingFactor) * HF_QSmoother.targetValue;
  if (HF_GainSmoother.isInitialized != 1) {
    HF_GainSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  HF_GainSmoother.currentValue =
      HF_GainSmoother.smoothingFactor * HF_GainSmoother.currentValue +
      (1.0 - HF_GainSmoother.smoothingFactor) * HF_GainSmoother.targetValue;
  //  Calculate coefficients for different filter types
  w0 = 6.2831853071795862 * HF_FreqSmoother.currentValue / fs;
  alpha = std::sin(w0) / (2.0 * HF_QSmoother.currentValue);
  A = std::sqrt(rt_powd_snf(10.0, HF_GainSmoother.currentValue / 20.0));
  d = A * alpha;
  filter_HF.a0 = d + 1.0;
  w0 = -2.0 * std::cos(w0);
  filter_HF.a1 = w0;
  filter_HF.a2 = 1.0 - d;
  d = alpha / A;
  filter_HF.b0 = d + 1.0;
  filter_HF.b1 = w0;
  filter_HF.b2 = 1.0 - d;
}

void derivedAudioPlugin::updateHPF()
{
  double b_gamma;
  double beta;
  double d;
  double w0;
  //  Update High Pass Filter (HPF)
  if (HPF_FreqSmoother.isInitialized != 1) {
    HPF_FreqSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  HPF_FreqSmoother.currentValue =
      HPF_FreqSmoother.smoothingFactor * HPF_FreqSmoother.currentValue +
      (1.0 - HPF_FreqSmoother.smoothingFactor) * HPF_FreqSmoother.targetValue;
  if (HPF_QSmoother.isInitialized != 1) {
    HPF_QSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  HPF_QSmoother.currentValue =
      HPF_QSmoother.smoothingFactor * HPF_QSmoother.currentValue +
      (1.0 - HPF_QSmoother.smoothingFactor) * HPF_QSmoother.targetValue;
  //  Calculate coefficients for different filter types
  w0 = 6.2831853071795862 * HPF_FreqSmoother.currentValue / fs;
  //  a0 = (1 + cos(w0)) / 2;
  //  a1 = -(1 - cos(w0));
  //  a2 = (1 + cos(w0)) / 2;
  //  b0 = 1 + alpha;
  //  b1 = -2 * cos(w0);
  //  b2 = 1 - alpha;
  b_gamma = 1.0 / HPF_QSmoother.currentValue / 2.0 * std::sin(w0);
  beta = 0.5 * ((1.0 - b_gamma) / (b_gamma + 1.0));
  b_gamma = (beta + 0.5) * std::cos(w0);
  w0 = (beta + 0.5) + b_gamma;
  d = w0 / 2.0;
  filter_HPF.a0 = d;
  filter_HPF.a1 = -w0;
  filter_HPF.a2 = d;
  filter_HPF.b0 = 1.0;
  filter_HPF.b1 = -2.0 * b_gamma;
  filter_HPF.b2 = 2.0 * beta;
}

void derivedAudioPlugin::updateHS()
{
  double A;
  double a0_tmp;
  double d;
  double d1;
  double d2;
  double d3;
  double w0;
  //  Update High Shelf (HS)
  if (HS_FreqSmoother.isInitialized != 1) {
    HS_FreqSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  HS_FreqSmoother.currentValue =
      HS_FreqSmoother.smoothingFactor * HS_FreqSmoother.currentValue +
      (1.0 - HS_FreqSmoother.smoothingFactor) * HS_FreqSmoother.targetValue;
  if (HS_QSmoother.isInitialized != 1) {
    HS_QSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  HS_QSmoother.currentValue =
      HS_QSmoother.smoothingFactor * HS_QSmoother.currentValue +
      (1.0 - HS_QSmoother.smoothingFactor) * HS_QSmoother.targetValue;
  if (HS_GainSmoother.isInitialized != 1) {
    HS_GainSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  HS_GainSmoother.currentValue =
      HS_GainSmoother.smoothingFactor * HS_GainSmoother.currentValue +
      (1.0 - HS_GainSmoother.smoothingFactor) * HS_GainSmoother.targetValue;
  //  Calculate coefficients for different filter types
  w0 = 6.2831853071795862 * HS_FreqSmoother.currentValue / fs;
  A = std::sqrt(rt_powd_snf(10.0, HS_GainSmoother.currentValue / 20.0));
  a0_tmp = std::cos(w0);
  d = (A - 1.0) * a0_tmp;
  d1 = 2.0 * std::sqrt(A) * (std::sin(w0) / (2.0 * HS_QSmoother.currentValue));
  d2 = (A + 1.0) - d;
  d3 = d2 + d1;
  filter_HS.a0 = A * d3;
  w0 = (A - 1.0) - (A + 1.0) * a0_tmp;
  filter_HS.a1 = -2.0 * A * w0;
  filter_HS.a2 = A * (((A + 1.0) + d) - d1);
  filter_HS.b0 = d3;
  filter_HS.b1 = 2.0 * w0;
  filter_HS.b2 = d2 - d1;
}

void derivedAudioPlugin::updateLF()
{
  double A;
  double alpha;
  double d;
  double w0;
  //  Update Low Peak Filter (LF)
  if (LF_FreqSmoother.isInitialized != 1) {
    LF_FreqSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  LF_FreqSmoother.currentValue =
      LF_FreqSmoother.smoothingFactor * LF_FreqSmoother.currentValue +
      (1.0 - LF_FreqSmoother.smoothingFactor) * LF_FreqSmoother.targetValue;
  if (LF_QSmoother.isInitialized != 1) {
    LF_QSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  LF_QSmoother.currentValue =
      LF_QSmoother.smoothingFactor * LF_QSmoother.currentValue +
      (1.0 - LF_QSmoother.smoothingFactor) * LF_QSmoother.targetValue;
  if (LF_GainSmoother.isInitialized != 1) {
    LF_GainSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  LF_GainSmoother.currentValue =
      LF_GainSmoother.smoothingFactor * LF_GainSmoother.currentValue +
      (1.0 - LF_GainSmoother.smoothingFactor) * LF_GainSmoother.targetValue;
  //  Calculate coefficients for different filter types
  w0 = 6.2831853071795862 * LF_FreqSmoother.currentValue / fs;
  alpha = std::sin(w0) / (2.0 * LF_QSmoother.currentValue);
  A = std::sqrt(rt_powd_snf(10.0, LF_GainSmoother.currentValue / 20.0));
  d = A * alpha;
  filter_LF.a0 = d + 1.0;
  w0 = -2.0 * std::cos(w0);
  filter_LF.a1 = w0;
  filter_LF.a2 = 1.0 - d;
  d = alpha / A;
  filter_LF.b0 = d + 1.0;
  filter_LF.b1 = w0;
  filter_LF.b2 = 1.0 - d;
}

void derivedAudioPlugin::updateLPF()
{
  double alpha;
  double d;
  double w0;
  //  Update Low Pass Filter (LPF)
  if (LPF_FreqSmoother.isInitialized != 1) {
    LPF_FreqSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  LPF_FreqSmoother.currentValue =
      LPF_FreqSmoother.smoothingFactor * LPF_FreqSmoother.currentValue +
      (1.0 - LPF_FreqSmoother.smoothingFactor) * LPF_FreqSmoother.targetValue;
  if (LPF_QSmoother.isInitialized != 1) {
    LPF_QSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  LPF_QSmoother.currentValue =
      LPF_QSmoother.smoothingFactor * LPF_QSmoother.currentValue +
      (1.0 - LPF_QSmoother.smoothingFactor) * LPF_QSmoother.targetValue;
  //  Calculate coefficients for different filter types
  w0 = 6.2831853071795862 * LPF_FreqSmoother.currentValue / fs;
  alpha = std::sin(w0) / (2.0 * LPF_QSmoother.currentValue);
  w0 = std::cos(w0);
  d = (1.0 - w0) / 2.0;
  filter_LPF.a0 = d;
  filter_LPF.a1 = 1.0 - w0;
  filter_LPF.a2 = d;
  filter_LPF.b0 = alpha + 1.0;
  filter_LPF.b1 = -2.0 * w0;
  filter_LPF.b2 = 1.0 - alpha;
}

void derivedAudioPlugin::updateLS()
{
  double A;
  double a0_tmp;
  double d;
  double d1;
  double d2;
  double w0;
  //  Update Low Shelf Filter (LS)
  if (LS_FreqSmoother.isInitialized != 1) {
    LS_FreqSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  LS_FreqSmoother.currentValue =
      LS_FreqSmoother.smoothingFactor * LS_FreqSmoother.currentValue +
      (1.0 - LS_FreqSmoother.smoothingFactor) * LS_FreqSmoother.targetValue;
  if (LS_QSmoother.isInitialized != 1) {
    LS_QSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  LS_QSmoother.currentValue =
      LS_QSmoother.smoothingFactor * LS_QSmoother.currentValue +
      (1.0 - LS_QSmoother.smoothingFactor) * LS_QSmoother.targetValue;
  if (LS_GainSmoother.isInitialized != 1) {
    LS_GainSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  LS_GainSmoother.currentValue =
      LS_GainSmoother.smoothingFactor * LS_GainSmoother.currentValue +
      (1.0 - LS_GainSmoother.smoothingFactor) * LS_GainSmoother.targetValue;
  //  Calculate coefficients for different filter types
  w0 = 6.2831853071795862 * LS_FreqSmoother.currentValue / fs;
  A = std::sqrt(rt_powd_snf(10.0, LS_GainSmoother.currentValue / 20.0));
  a0_tmp = std::cos(w0);
  d = (A - 1.0) * a0_tmp;
  d1 = (A + 1.0) - d;
  d2 = 2.0 * std::sqrt(A) * (std::sin(w0) / (2.0 * LS_QSmoother.currentValue));
  filter_LS.a0 = A * (d1 + d2);
  w0 = (A + 1.0) * a0_tmp;
  filter_LS.a1 = 2.0 * A * ((A - 1.0) - w0);
  filter_LS.a2 = A * (d1 - d2);
  d += A + 1.0;
  filter_LS.b0 = d + d2;
  filter_LS.b1 = -2.0 * ((A - 1.0) + w0);
  filter_LS.b2 = d - d2;
}

void derivedAudioPlugin::updateMF()
{
  double A;
  double alpha;
  double d;
  double w0;
  //  Update Mid Peak Filter (MF)
  if (MF_FreqSmoother.isInitialized != 1) {
    MF_FreqSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  MF_FreqSmoother.currentValue =
      MF_FreqSmoother.smoothingFactor * MF_FreqSmoother.currentValue +
      (1.0 - MF_FreqSmoother.smoothingFactor) * MF_FreqSmoother.targetValue;
  if (MF_QSmoother.isInitialized != 1) {
    MF_QSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  MF_QSmoother.currentValue =
      MF_QSmoother.smoothingFactor * MF_QSmoother.currentValue +
      (1.0 - MF_QSmoother.smoothingFactor) * MF_QSmoother.targetValue;
  if (MF_GainSmoother.isInitialized != 1) {
    MF_GainSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  MF_GainSmoother.currentValue =
      MF_GainSmoother.smoothingFactor * MF_GainSmoother.currentValue +
      (1.0 - MF_GainSmoother.smoothingFactor) * MF_GainSmoother.targetValue;
  //  Calculate coefficients for different filter types
  w0 = 6.2831853071795862 * MF_FreqSmoother.currentValue / fs;
  alpha = std::sin(w0) / (2.0 * MF_QSmoother.currentValue);
  A = std::sqrt(rt_powd_snf(10.0, MF_GainSmoother.currentValue / 20.0));
  d = A * alpha;
  filter_MF.a0 = d + 1.0;
  w0 = -2.0 * std::cos(w0);
  filter_MF.a1 = w0;
  filter_MF.a2 = 1.0 - d;
  d = alpha / A;
  filter_MF.b0 = d + 1.0;
  filter_MF.b1 = w0;
  filter_MF.b2 = 1.0 - d;
}

void audioPlugin::wormholeToConstructor_init(bmfproj1StackData *SD)
{
  SD->pd->thisPtr_not_empty = false;
}

static derivedAudioPlugin *getPluginInstance(bmfproj1StackData *SD)
{
  if (!SD->pd->plugin_not_empty) {
    double initialValue;
    boolean_T flag;
    //  Pass constructor args to plugin.
    SD->pd->plugin.BYPASS.set_size(1, 3);
    SD->pd->plugin.BYPASS[0] = 'O';
    SD->pd->plugin.BYPASS[1] = 'F';
    SD->pd->plugin.BYPASS[2] = 'F';
    SD->pd->plugin.THRESHOLD = -50.0;
    SD->pd->plugin.ATTACK = 5.0;
    SD->pd->plugin.RELEASE = 100.0;
    SD->pd->plugin.HOLD = 10.0;
    SD->pd->plugin.LPF_FREQ = 20000.0;
    SD->pd->plugin.LPF_Q = 0.71;
    SD->pd->plugin.HS_FREQ = 10000.0;
    SD->pd->plugin.HS_GAIN = 0.0;
    SD->pd->plugin.HS_Q = 1.0;
    SD->pd->plugin.HF_FREQ = 8000.0;
    SD->pd->plugin.HF_GAIN = 0.0;
    SD->pd->plugin.HF_Q = 0.5;
    SD->pd->plugin.MF_FREQ = 1000.0;
    SD->pd->plugin.MF_GAIN = 0.0;
    SD->pd->plugin.MF_Q = 0.5;
    SD->pd->plugin.LF_FREQ = 500.0;
    SD->pd->plugin.LF_GAIN = 0.0;
    SD->pd->plugin.LF_Q = 0.5;
    SD->pd->plugin.LS_FREQ = 200.0;
    SD->pd->plugin.LS_GAIN = 0.0;
    SD->pd->plugin.LS_Q = 1.0;
    SD->pd->plugin.HPF_FREQ = 20.0;
    SD->pd->plugin.HPF_Q = 0.71;
    SD->pd->plugin.filter_LPF.a0 = 1.0;
    SD->pd->plugin.filter_LPF.a1 = 0.0;
    SD->pd->plugin.filter_LPF.a2 = 0.0;
    SD->pd->plugin.filter_LPF.b0 = 1.0;
    SD->pd->plugin.filter_LPF.b1 = 0.0;
    SD->pd->plugin.filter_LPF.b2 = 0.0;
    SD->pd->plugin.filter_HS.a0 = 1.0;
    SD->pd->plugin.filter_HS.a1 = 0.0;
    SD->pd->plugin.filter_HS.a2 = 0.0;
    SD->pd->plugin.filter_HS.b0 = 1.0;
    SD->pd->plugin.filter_HS.b1 = 0.0;
    SD->pd->plugin.filter_HS.b2 = 0.0;
    SD->pd->plugin.filter_HF.a0 = 1.0;
    SD->pd->plugin.filter_HF.a1 = 0.0;
    SD->pd->plugin.filter_HF.a2 = 0.0;
    SD->pd->plugin.filter_HF.b0 = 1.0;
    SD->pd->plugin.filter_HF.b1 = 0.0;
    SD->pd->plugin.filter_HF.b2 = 0.0;
    SD->pd->plugin.filter_MF.a0 = 1.0;
    SD->pd->plugin.filter_MF.a1 = 0.0;
    SD->pd->plugin.filter_MF.a2 = 0.0;
    SD->pd->plugin.filter_MF.b0 = 1.0;
    SD->pd->plugin.filter_MF.b1 = 0.0;
    SD->pd->plugin.filter_MF.b2 = 0.0;
    SD->pd->plugin.filter_LF.a0 = 1.0;
    SD->pd->plugin.filter_LF.a1 = 0.0;
    SD->pd->plugin.filter_LF.a2 = 0.0;
    SD->pd->plugin.filter_LF.b0 = 1.0;
    SD->pd->plugin.filter_LF.b1 = 0.0;
    SD->pd->plugin.filter_LF.b2 = 0.0;
    SD->pd->plugin.filter_LS.a0 = 1.0;
    SD->pd->plugin.filter_LS.a1 = 0.0;
    SD->pd->plugin.filter_LS.a2 = 0.0;
    SD->pd->plugin.filter_LS.b0 = 1.0;
    SD->pd->plugin.filter_LS.b1 = 0.0;
    SD->pd->plugin.filter_LS.b2 = 0.0;
    SD->pd->plugin.filter_LPF.w[0] = 0.0;
    SD->pd->plugin.filter_HS.w[0] = 0.0;
    SD->pd->plugin.filter_HF.w[0] = 0.0;
    SD->pd->plugin.filter_MF.w[0] = 0.0;
    SD->pd->plugin.filter_LF.w[0] = 0.0;
    SD->pd->plugin.filter_LS.w[0] = 0.0;
    SD->pd->plugin.filter_HPF.w[0] = 0.0;
    SD->pd->plugin.filter_LPF.w[1] = 0.0;
    SD->pd->plugin.filter_HS.w[1] = 0.0;
    SD->pd->plugin.filter_HF.w[1] = 0.0;
    SD->pd->plugin.filter_MF.w[1] = 0.0;
    SD->pd->plugin.filter_LF.w[1] = 0.0;
    SD->pd->plugin.filter_LS.w[1] = 0.0;
    SD->pd->plugin.filter_HPF.w[1] = 0.0;
    SD->pd->plugin.filter_LPF.w[2] = 0.0;
    SD->pd->plugin.filter_HS.w[2] = 0.0;
    SD->pd->plugin.filter_HF.w[2] = 0.0;
    SD->pd->plugin.filter_MF.w[2] = 0.0;
    SD->pd->plugin.filter_LF.w[2] = 0.0;
    SD->pd->plugin.filter_LS.w[2] = 0.0;
    SD->pd->plugin.filter_HPF.w[2] = 0.0;
    SD->pd->plugin.filter_LPF.w[3] = 0.0;
    SD->pd->plugin.filter_HS.w[3] = 0.0;
    SD->pd->plugin.filter_HF.w[3] = 0.0;
    SD->pd->plugin.filter_MF.w[3] = 0.0;
    SD->pd->plugin.filter_LF.w[3] = 0.0;
    SD->pd->plugin.filter_LS.w[3] = 0.0;
    SD->pd->plugin.filter_HPF.w[3] = 0.0;
    SD->pd->plugin.filter_HPF.a0 = 1.0;
    SD->pd->plugin.filter_HPF.a1 = 0.0;
    SD->pd->plugin.filter_HPF.a2 = 0.0;
    SD->pd->plugin.filter_HPF.b0 = 1.0;
    SD->pd->plugin.filter_HPF.b1 = 0.0;
    SD->pd->plugin.filter_HPF.b2 = 0.0;
    SD->pd->plugin.fs = 44100.0;
    //  Constructor
    SD->pd->plugin.PrivateSampleRate = 44100.0;
    SD->pd->plugin.PrivateLatency = 0;
    SD->pd->thisPtr_not_empty = true;
    //  Initialize parameter smoothers with default values
    initialValue = SD->pd->plugin.THRESHOLD;
    SD->pd->plugin.THRESHOLD_Smoother.isInitialized = 0;
    SD->pd->plugin.THRESHOLD_Smoother.currentValue = initialValue;
    SD->pd->plugin.THRESHOLD_Smoother.targetValue = initialValue;
    SD->pd->plugin.THRESHOLD_Smoother.smoothingFactor = 0.25;
    initialValue = SD->pd->plugin.ATTACK;
    SD->pd->plugin.ATTACK_Smoother.isInitialized = 0;
    SD->pd->plugin.ATTACK_Smoother.currentValue = initialValue;
    SD->pd->plugin.ATTACK_Smoother.targetValue = initialValue;
    SD->pd->plugin.ATTACK_Smoother.smoothingFactor = 0.25;
    initialValue = SD->pd->plugin.RELEASE;
    SD->pd->plugin.RELEASE_Smoother.isInitialized = 0;
    SD->pd->plugin.RELEASE_Smoother.currentValue = initialValue;
    SD->pd->plugin.RELEASE_Smoother.targetValue = initialValue;
    SD->pd->plugin.RELEASE_Smoother.smoothingFactor = 0.25;
    initialValue = SD->pd->plugin.HOLD;
    SD->pd->plugin.HOLD_Smoother.isInitialized = 0;
    SD->pd->plugin.HOLD_Smoother.currentValue = initialValue;
    SD->pd->plugin.HOLD_Smoother.targetValue = initialValue;
    SD->pd->plugin.HOLD_Smoother.smoothingFactor = 0.25;
    initialValue = SD->pd->plugin.LPF_FREQ;
    SD->pd->plugin.LPF_FreqSmoother.isInitialized = 0;
    SD->pd->plugin.LPF_FreqSmoother.currentValue = initialValue;
    SD->pd->plugin.LPF_FreqSmoother.targetValue = initialValue;
    SD->pd->plugin.LPF_FreqSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.HS_FREQ;
    SD->pd->plugin.HS_FreqSmoother.isInitialized = 0;
    SD->pd->plugin.HS_FreqSmoother.currentValue = initialValue;
    SD->pd->plugin.HS_FreqSmoother.targetValue = initialValue;
    SD->pd->plugin.HS_FreqSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.HF_FREQ;
    SD->pd->plugin.HF_FreqSmoother.isInitialized = 0;
    SD->pd->plugin.HF_FreqSmoother.currentValue = initialValue;
    SD->pd->plugin.HF_FreqSmoother.targetValue = initialValue;
    SD->pd->plugin.HF_FreqSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.MF_FREQ;
    SD->pd->plugin.MF_FreqSmoother.isInitialized = 0;
    SD->pd->plugin.MF_FreqSmoother.currentValue = initialValue;
    SD->pd->plugin.MF_FreqSmoother.targetValue = initialValue;
    SD->pd->plugin.MF_FreqSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.LF_FREQ;
    SD->pd->plugin.LF_FreqSmoother.isInitialized = 0;
    SD->pd->plugin.LF_FreqSmoother.currentValue = initialValue;
    SD->pd->plugin.LF_FreqSmoother.targetValue = initialValue;
    SD->pd->plugin.LF_FreqSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.LS_FREQ;
    SD->pd->plugin.LS_FreqSmoother.isInitialized = 0;
    SD->pd->plugin.LS_FreqSmoother.currentValue = initialValue;
    SD->pd->plugin.LS_FreqSmoother.targetValue = initialValue;
    SD->pd->plugin.LS_FreqSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.HPF_FREQ;
    SD->pd->plugin.HPF_FreqSmoother.isInitialized = 0;
    SD->pd->plugin.HPF_FreqSmoother.currentValue = initialValue;
    SD->pd->plugin.HPF_FreqSmoother.targetValue = initialValue;
    SD->pd->plugin.HPF_FreqSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.HS_GAIN;
    SD->pd->plugin.HS_GainSmoother.isInitialized = 0;
    SD->pd->plugin.HS_GainSmoother.currentValue = initialValue;
    SD->pd->plugin.HS_GainSmoother.targetValue = initialValue;
    SD->pd->plugin.HS_GainSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.HF_GAIN;
    SD->pd->plugin.HF_GainSmoother.isInitialized = 0;
    SD->pd->plugin.HF_GainSmoother.currentValue = initialValue;
    SD->pd->plugin.HF_GainSmoother.targetValue = initialValue;
    SD->pd->plugin.HF_GainSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.MF_GAIN;
    SD->pd->plugin.MF_GainSmoother.isInitialized = 0;
    SD->pd->plugin.MF_GainSmoother.currentValue = initialValue;
    SD->pd->plugin.MF_GainSmoother.targetValue = initialValue;
    SD->pd->plugin.MF_GainSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.LF_GAIN;
    SD->pd->plugin.LF_GainSmoother.isInitialized = 0;
    SD->pd->plugin.LF_GainSmoother.currentValue = initialValue;
    SD->pd->plugin.LF_GainSmoother.targetValue = initialValue;
    SD->pd->plugin.LF_GainSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.LS_GAIN;
    SD->pd->plugin.LS_GainSmoother.isInitialized = 0;
    SD->pd->plugin.LS_GainSmoother.currentValue = initialValue;
    SD->pd->plugin.LS_GainSmoother.targetValue = initialValue;
    SD->pd->plugin.LS_GainSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.LPF_Q;
    SD->pd->plugin.LPF_QSmoother.isInitialized = 0;
    SD->pd->plugin.LPF_QSmoother.currentValue = initialValue;
    SD->pd->plugin.LPF_QSmoother.targetValue = initialValue;
    SD->pd->plugin.LPF_QSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.HS_Q;
    SD->pd->plugin.HS_QSmoother.isInitialized = 0;
    SD->pd->plugin.HS_QSmoother.currentValue = initialValue;
    SD->pd->plugin.HS_QSmoother.targetValue = initialValue;
    SD->pd->plugin.HS_QSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.HF_Q;
    SD->pd->plugin.HF_QSmoother.isInitialized = 0;
    SD->pd->plugin.HF_QSmoother.currentValue = initialValue;
    SD->pd->plugin.HF_QSmoother.targetValue = initialValue;
    SD->pd->plugin.HF_QSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.MF_Q;
    SD->pd->plugin.MF_QSmoother.isInitialized = 0;
    SD->pd->plugin.MF_QSmoother.currentValue = initialValue;
    SD->pd->plugin.MF_QSmoother.targetValue = initialValue;
    SD->pd->plugin.MF_QSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.LF_Q;
    SD->pd->plugin.LF_QSmoother.isInitialized = 0;
    SD->pd->plugin.LF_QSmoother.currentValue = initialValue;
    SD->pd->plugin.LF_QSmoother.targetValue = initialValue;
    SD->pd->plugin.LF_QSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.LS_Q;
    SD->pd->plugin.LS_QSmoother.isInitialized = 0;
    SD->pd->plugin.LS_QSmoother.currentValue = initialValue;
    SD->pd->plugin.LS_QSmoother.targetValue = initialValue;
    SD->pd->plugin.LS_QSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.HPF_Q;
    SD->pd->plugin.HPF_QSmoother.isInitialized = 0;
    SD->pd->plugin.HPF_QSmoother.currentValue = initialValue;
    SD->pd->plugin.HPF_QSmoother.targetValue = initialValue;
    SD->pd->plugin.HPF_QSmoother.smoothingFactor = 0.5;
    //  Initialize Gate
    SD->pd->plugin.gate.pSampleRateDialog = 44100.0;
    SD->pd->plugin.gate.pNumChannels = -1.0;
    SD->pd->plugin.gate.isInitialized = 0;
    SD->pd->plugin.gate.matlabCodegenIsDeleted = false;
    //         %% Update Functions
    //  Update Noise Gate
    flag = (SD->pd->plugin.gate.isInitialized == 1);
    if (flag) {
      SD->pd->plugin.gate.TunablePropsChanged = true;
    }
    if (SD->pd->plugin.THRESHOLD_Smoother.isInitialized != 1) {
      SD->pd->plugin.THRESHOLD_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    SD->pd->plugin.THRESHOLD_Smoother.currentValue =
        SD->pd->plugin.THRESHOLD_Smoother.smoothingFactor *
            SD->pd->plugin.THRESHOLD_Smoother.currentValue +
        (1.0 - SD->pd->plugin.THRESHOLD_Smoother.smoothingFactor) *
            SD->pd->plugin.THRESHOLD_Smoother.targetValue;
    initialValue = SD->pd->plugin.THRESHOLD_Smoother.currentValue;
    SD->pd->plugin.gate.Threshold = initialValue;
    flag = (SD->pd->plugin.gate.isInitialized == 1);
    if (flag) {
      SD->pd->plugin.gate.TunablePropsChanged = true;
    }
    if (SD->pd->plugin.ATTACK_Smoother.isInitialized != 1) {
      SD->pd->plugin.ATTACK_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    SD->pd->plugin.ATTACK_Smoother.currentValue =
        SD->pd->plugin.ATTACK_Smoother.smoothingFactor *
            SD->pd->plugin.ATTACK_Smoother.currentValue +
        (1.0 - SD->pd->plugin.ATTACK_Smoother.smoothingFactor) *
            SD->pd->plugin.ATTACK_Smoother.targetValue;
    initialValue = SD->pd->plugin.ATTACK_Smoother.currentValue;
    SD->pd->plugin.gate.AttackTime = initialValue / 1000.0;
    flag = (SD->pd->plugin.gate.isInitialized == 1);
    if (flag) {
      SD->pd->plugin.gate.TunablePropsChanged = true;
    }
    if (SD->pd->plugin.RELEASE_Smoother.isInitialized != 1) {
      SD->pd->plugin.RELEASE_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    SD->pd->plugin.RELEASE_Smoother.currentValue =
        SD->pd->plugin.RELEASE_Smoother.smoothingFactor *
            SD->pd->plugin.RELEASE_Smoother.currentValue +
        (1.0 - SD->pd->plugin.RELEASE_Smoother.smoothingFactor) *
            SD->pd->plugin.RELEASE_Smoother.targetValue;
    initialValue = SD->pd->plugin.RELEASE_Smoother.currentValue;
    SD->pd->plugin.gate.ReleaseTime = initialValue / 1000.0;
    flag = (SD->pd->plugin.gate.isInitialized == 1);
    if (flag) {
      SD->pd->plugin.gate.TunablePropsChanged = true;
    }
    if (SD->pd->plugin.HOLD_Smoother.isInitialized != 1) {
      SD->pd->plugin.HOLD_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    SD->pd->plugin.HOLD_Smoother.currentValue =
        SD->pd->plugin.HOLD_Smoother.smoothingFactor *
            SD->pd->plugin.HOLD_Smoother.currentValue +
        (1.0 - SD->pd->plugin.HOLD_Smoother.smoothingFactor) *
            SD->pd->plugin.HOLD_Smoother.targetValue;
    initialValue = SD->pd->plugin.HOLD_Smoother.currentValue;
    SD->pd->plugin.gate.HoldTime = initialValue / 1000.0;
    //  Initialize EQ Filters
    SD->pd->plugin.updateLPF();
    SD->pd->plugin.updateHS();
    SD->pd->plugin.updateHF();
    SD->pd->plugin.updateMF();
    SD->pd->plugin.updateLF();
    SD->pd->plugin.updateLS();
    SD->pd->plugin.updateHPF();
    SD->pd->plugin.matlabCodegenIsDeleted = false;
    SD->pd->plugin_not_empty = true;
  }
  return &SD->pd->plugin;
}

static void getPluginInstance_delete(bmfproj1StackData *SD)
{
  if (!SD->pd->plugin.matlabCodegenIsDeleted) {
    SD->pd->plugin.matlabCodegenIsDeleted = true;
  }
  if (!SD->pd->plugin.gate.matlabCodegenIsDeleted) {
    SD->pd->plugin.gate.matlabCodegenIsDeleted = true;
    if (SD->pd->plugin.gate.isInitialized == 1) {
      SD->pd->plugin.gate.isInitialized = 2;
      if (SD->pd->plugin.gate.isSetupComplete) {
        SD->pd->plugin.gate.pNumChannels = -1.0;
      }
    }
  }
}

static void getPluginInstance_init(bmfproj1StackData *SD)
{
  SD->pd->plugin_not_empty = false;
}

static void getPluginInstance_new(bmfproj1StackData *SD)
{
  SD->pd->plugin.gate.matlabCodegenIsDeleted = true;
  SD->pd->plugin.matlabCodegenIsDeleted = true;
}

static double rt_powd_snf(double u0, double u1)
{
  double y;
  if (std::isnan(u0) || std::isnan(u1)) {
    y = rtNaN;
  } else {
    double d;
    double d1;
    d = std::abs(u0);
    d1 = std::abs(u1);
    if (std::isinf(u1)) {
      if (d == 1.0) {
        y = 1.0;
      } else if (d > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = std::sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > std::floor(u1))) {
      y = rtNaN;
    } else {
      y = std::pow(u0, u1);
    }
  }
  return y;
}

ParameterSmoother::ParameterSmoother() = default;

ParameterSmoother::~ParameterSmoother() = default;

namespace coder {
noiseGate::noiseGate() = default;

noiseGate::~noiseGate() = default;

} // namespace coder
derivedAudioPlugin::derivedAudioPlugin() = default;

derivedAudioPlugin::~derivedAudioPlugin() = default;

void bmfproj1_initialize(bmfproj1StackData *SD)
{
  getPluginInstance_new(SD);
  getPluginInstance_init(SD);
  audioPlugin::wormholeToConstructor_init(SD);
}

void bmfproj1_terminate(bmfproj1StackData *SD)
{
  getPluginInstance_delete(SD);
}

void createPluginInstance(bmfproj1StackData *SD, unsigned long)
{
  SD->pd->thisPtr_not_empty = true;
  getPluginInstance(SD);
}

int getLatencyInSamplesCImpl(bmfproj1StackData *SD)
{
  derivedAudioPlugin *plugin;
  plugin = getPluginInstance(SD);
  return plugin->PrivateLatency;
}

void onParamChangeCImpl(bmfproj1StackData *SD, int paramIdx, double b_value)
{
  derivedAudioPlugin *plugin;
  plugin = getPluginInstance(SD);
  switch (paramIdx) {
  case 0:
    switch (static_cast<int>(b_value)) {
    case 0:
      //         %% Set Functions
      plugin->BYPASS.set_size(1, 3);
      plugin->BYPASS[0] = 'O';
      plugin->BYPASS[1] = 'F';
      plugin->BYPASS[2] = 'F';
      break;
    case 1:
      //         %% Set Functions
      plugin->BYPASS.set_size(1, 2);
      plugin->BYPASS[0] = 'O';
      plugin->BYPASS[1] = 'N';
      break;
    }
    break;
  case 1:
    //  LPF Setters
    plugin->LPF_FREQ = b_value;
    plugin->LPF_FreqSmoother.targetValue = b_value;
    plugin->updateLPF();
    break;
  case 2:
    plugin->LPF_Q = b_value;
    plugin->LPF_QSmoother.targetValue = b_value;
    plugin->updateLPF();
    break;
  case 3:
    //  HS Setters
    plugin->HS_FREQ = b_value;
    plugin->HS_FreqSmoother.targetValue = b_value;
    plugin->updateHS();
    break;
  case 4:
    plugin->HS_GAIN = b_value;
    plugin->HS_GainSmoother.targetValue = b_value;
    plugin->updateHS();
    break;
  case 5:
    plugin->HS_Q = b_value;
    plugin->HS_QSmoother.targetValue = b_value;
    plugin->updateHS();
    break;
  case 6:
    //  HF Setters
    plugin->HF_FREQ = b_value;
    plugin->HF_FreqSmoother.targetValue = b_value;
    plugin->updateHF();
    break;
  case 7:
    plugin->HF_GAIN = b_value;
    plugin->HF_GainSmoother.targetValue = b_value;
    plugin->updateHF();
    break;
  case 8:
    plugin->HF_Q = b_value;
    plugin->HF_QSmoother.targetValue = b_value;
    plugin->updateHF();
    break;
  case 9:
    //  MF Setters
    plugin->MF_FREQ = b_value;
    plugin->MF_FreqSmoother.targetValue = b_value;
    plugin->updateMF();
    break;
  case 10:
    plugin->MF_GAIN = b_value;
    plugin->MF_GainSmoother.targetValue = b_value;
    plugin->updateMF();
    break;
  case 11:
    plugin->MF_Q = b_value;
    plugin->MF_QSmoother.targetValue = b_value;
    plugin->updateMF();
    break;
  case 12:
    //  LF Setters
    plugin->LF_FREQ = b_value;
    plugin->LF_FreqSmoother.targetValue = b_value;
    plugin->updateLF();
    break;
  case 13:
    plugin->LF_GAIN = b_value;
    plugin->LF_GainSmoother.targetValue = b_value;
    plugin->updateLF();
    break;
  case 14:
    plugin->LF_Q = b_value;
    plugin->LF_QSmoother.targetValue = b_value;
    plugin->updateLF();
    break;
  case 15:
    //  LS Setters
    plugin->LS_FREQ = b_value;
    plugin->LS_FreqSmoother.targetValue = b_value;
    plugin->updateLS();
    break;
  case 16:
    plugin->LS_GAIN = b_value;
    plugin->LS_GainSmoother.targetValue = b_value;
    plugin->updateLS();
    break;
  case 17:
    plugin->LS_Q = b_value;
    plugin->LS_QSmoother.targetValue = b_value;
    plugin->updateLS();
    break;
  case 18:
    //  HFP Setters
    plugin->HPF_FREQ = b_value;
    plugin->HPF_FreqSmoother.targetValue = b_value;
    plugin->updateHPF();
    break;
  case 19:
    plugin->HPF_Q = b_value;
    plugin->HPF_QSmoother.targetValue = b_value;
    plugin->updateHPF();
    break;
  case 20: {
    double c_value;
    boolean_T flag;
    //  Gate Setters
    plugin->THRESHOLD = b_value;
    plugin->THRESHOLD_Smoother.targetValue = b_value;
    //         %% Update Functions
    //  Update Noise Gate
    flag = (plugin->gate.isInitialized == 1);
    if (flag) {
      plugin->gate.TunablePropsChanged = true;
    }
    if (plugin->THRESHOLD_Smoother.isInitialized != 1) {
      plugin->THRESHOLD_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->THRESHOLD_Smoother.currentValue =
        plugin->THRESHOLD_Smoother.smoothingFactor *
            plugin->THRESHOLD_Smoother.currentValue +
        (1.0 - plugin->THRESHOLD_Smoother.smoothingFactor) *
            plugin->THRESHOLD_Smoother.targetValue;
    c_value = plugin->THRESHOLD_Smoother.currentValue;
    plugin->gate.Threshold = c_value;
    flag = (plugin->gate.isInitialized == 1);
    if (flag) {
      plugin->gate.TunablePropsChanged = true;
    }
    if (plugin->ATTACK_Smoother.isInitialized != 1) {
      plugin->ATTACK_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->ATTACK_Smoother.currentValue =
        plugin->ATTACK_Smoother.smoothingFactor *
            plugin->ATTACK_Smoother.currentValue +
        (1.0 - plugin->ATTACK_Smoother.smoothingFactor) *
            plugin->ATTACK_Smoother.targetValue;
    c_value = plugin->ATTACK_Smoother.currentValue;
    plugin->gate.AttackTime = c_value / 1000.0;
    flag = (plugin->gate.isInitialized == 1);
    if (flag) {
      plugin->gate.TunablePropsChanged = true;
    }
    if (plugin->RELEASE_Smoother.isInitialized != 1) {
      plugin->RELEASE_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->RELEASE_Smoother.currentValue =
        plugin->RELEASE_Smoother.smoothingFactor *
            plugin->RELEASE_Smoother.currentValue +
        (1.0 - plugin->RELEASE_Smoother.smoothingFactor) *
            plugin->RELEASE_Smoother.targetValue;
    c_value = plugin->RELEASE_Smoother.currentValue;
    plugin->gate.ReleaseTime = c_value / 1000.0;
    flag = (plugin->gate.isInitialized == 1);
    if (flag) {
      plugin->gate.TunablePropsChanged = true;
    }
    if (plugin->HOLD_Smoother.isInitialized != 1) {
      plugin->HOLD_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->HOLD_Smoother.currentValue =
        plugin->HOLD_Smoother.smoothingFactor *
            plugin->HOLD_Smoother.currentValue +
        (1.0 - plugin->HOLD_Smoother.smoothingFactor) *
            plugin->HOLD_Smoother.targetValue;
    c_value = plugin->HOLD_Smoother.currentValue;
    plugin->gate.HoldTime = c_value / 1000.0;
  } break;
  case 21: {
    double c_value;
    boolean_T flag;
    plugin->ATTACK = b_value;
    plugin->ATTACK_Smoother.targetValue = b_value;
    //         %% Update Functions
    //  Update Noise Gate
    flag = (plugin->gate.isInitialized == 1);
    if (flag) {
      plugin->gate.TunablePropsChanged = true;
    }
    if (plugin->THRESHOLD_Smoother.isInitialized != 1) {
      plugin->THRESHOLD_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->THRESHOLD_Smoother.currentValue =
        plugin->THRESHOLD_Smoother.smoothingFactor *
            plugin->THRESHOLD_Smoother.currentValue +
        (1.0 - plugin->THRESHOLD_Smoother.smoothingFactor) *
            plugin->THRESHOLD_Smoother.targetValue;
    c_value = plugin->THRESHOLD_Smoother.currentValue;
    plugin->gate.Threshold = c_value;
    flag = (plugin->gate.isInitialized == 1);
    if (flag) {
      plugin->gate.TunablePropsChanged = true;
    }
    if (plugin->ATTACK_Smoother.isInitialized != 1) {
      plugin->ATTACK_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->ATTACK_Smoother.currentValue =
        plugin->ATTACK_Smoother.smoothingFactor *
            plugin->ATTACK_Smoother.currentValue +
        (1.0 - plugin->ATTACK_Smoother.smoothingFactor) *
            plugin->ATTACK_Smoother.targetValue;
    c_value = plugin->ATTACK_Smoother.currentValue;
    plugin->gate.AttackTime = c_value / 1000.0;
    flag = (plugin->gate.isInitialized == 1);
    if (flag) {
      plugin->gate.TunablePropsChanged = true;
    }
    if (plugin->RELEASE_Smoother.isInitialized != 1) {
      plugin->RELEASE_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->RELEASE_Smoother.currentValue =
        plugin->RELEASE_Smoother.smoothingFactor *
            plugin->RELEASE_Smoother.currentValue +
        (1.0 - plugin->RELEASE_Smoother.smoothingFactor) *
            plugin->RELEASE_Smoother.targetValue;
    c_value = plugin->RELEASE_Smoother.currentValue;
    plugin->gate.ReleaseTime = c_value / 1000.0;
    flag = (plugin->gate.isInitialized == 1);
    if (flag) {
      plugin->gate.TunablePropsChanged = true;
    }
    if (plugin->HOLD_Smoother.isInitialized != 1) {
      plugin->HOLD_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->HOLD_Smoother.currentValue =
        plugin->HOLD_Smoother.smoothingFactor *
            plugin->HOLD_Smoother.currentValue +
        (1.0 - plugin->HOLD_Smoother.smoothingFactor) *
            plugin->HOLD_Smoother.targetValue;
    c_value = plugin->HOLD_Smoother.currentValue;
    plugin->gate.HoldTime = c_value / 1000.0;
  } break;
  case 22: {
    double c_value;
    boolean_T flag;
    plugin->HOLD = b_value;
    plugin->HOLD_Smoother.targetValue = b_value;
    //         %% Update Functions
    //  Update Noise Gate
    flag = (plugin->gate.isInitialized == 1);
    if (flag) {
      plugin->gate.TunablePropsChanged = true;
    }
    if (plugin->THRESHOLD_Smoother.isInitialized != 1) {
      plugin->THRESHOLD_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->THRESHOLD_Smoother.currentValue =
        plugin->THRESHOLD_Smoother.smoothingFactor *
            plugin->THRESHOLD_Smoother.currentValue +
        (1.0 - plugin->THRESHOLD_Smoother.smoothingFactor) *
            plugin->THRESHOLD_Smoother.targetValue;
    c_value = plugin->THRESHOLD_Smoother.currentValue;
    plugin->gate.Threshold = c_value;
    flag = (plugin->gate.isInitialized == 1);
    if (flag) {
      plugin->gate.TunablePropsChanged = true;
    }
    if (plugin->ATTACK_Smoother.isInitialized != 1) {
      plugin->ATTACK_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->ATTACK_Smoother.currentValue =
        plugin->ATTACK_Smoother.smoothingFactor *
            plugin->ATTACK_Smoother.currentValue +
        (1.0 - plugin->ATTACK_Smoother.smoothingFactor) *
            plugin->ATTACK_Smoother.targetValue;
    c_value = plugin->ATTACK_Smoother.currentValue;
    plugin->gate.AttackTime = c_value / 1000.0;
    flag = (plugin->gate.isInitialized == 1);
    if (flag) {
      plugin->gate.TunablePropsChanged = true;
    }
    if (plugin->RELEASE_Smoother.isInitialized != 1) {
      plugin->RELEASE_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->RELEASE_Smoother.currentValue =
        plugin->RELEASE_Smoother.smoothingFactor *
            plugin->RELEASE_Smoother.currentValue +
        (1.0 - plugin->RELEASE_Smoother.smoothingFactor) *
            plugin->RELEASE_Smoother.targetValue;
    c_value = plugin->RELEASE_Smoother.currentValue;
    plugin->gate.ReleaseTime = c_value / 1000.0;
    flag = (plugin->gate.isInitialized == 1);
    if (flag) {
      plugin->gate.TunablePropsChanged = true;
    }
    if (plugin->HOLD_Smoother.isInitialized != 1) {
      plugin->HOLD_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->HOLD_Smoother.currentValue =
        plugin->HOLD_Smoother.smoothingFactor *
            plugin->HOLD_Smoother.currentValue +
        (1.0 - plugin->HOLD_Smoother.smoothingFactor) *
            plugin->HOLD_Smoother.targetValue;
    c_value = plugin->HOLD_Smoother.currentValue;
    plugin->gate.HoldTime = c_value / 1000.0;
  } break;
  case 23: {
    double c_value;
    boolean_T flag;
    plugin->RELEASE = b_value;
    plugin->RELEASE_Smoother.targetValue = b_value;
    //         %% Update Functions
    //  Update Noise Gate
    flag = (plugin->gate.isInitialized == 1);
    if (flag) {
      plugin->gate.TunablePropsChanged = true;
    }
    if (plugin->THRESHOLD_Smoother.isInitialized != 1) {
      plugin->THRESHOLD_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->THRESHOLD_Smoother.currentValue =
        plugin->THRESHOLD_Smoother.smoothingFactor *
            plugin->THRESHOLD_Smoother.currentValue +
        (1.0 - plugin->THRESHOLD_Smoother.smoothingFactor) *
            plugin->THRESHOLD_Smoother.targetValue;
    c_value = plugin->THRESHOLD_Smoother.currentValue;
    plugin->gate.Threshold = c_value;
    flag = (plugin->gate.isInitialized == 1);
    if (flag) {
      plugin->gate.TunablePropsChanged = true;
    }
    if (plugin->ATTACK_Smoother.isInitialized != 1) {
      plugin->ATTACK_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->ATTACK_Smoother.currentValue =
        plugin->ATTACK_Smoother.smoothingFactor *
            plugin->ATTACK_Smoother.currentValue +
        (1.0 - plugin->ATTACK_Smoother.smoothingFactor) *
            plugin->ATTACK_Smoother.targetValue;
    c_value = plugin->ATTACK_Smoother.currentValue;
    plugin->gate.AttackTime = c_value / 1000.0;
    flag = (plugin->gate.isInitialized == 1);
    if (flag) {
      plugin->gate.TunablePropsChanged = true;
    }
    if (plugin->RELEASE_Smoother.isInitialized != 1) {
      plugin->RELEASE_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->RELEASE_Smoother.currentValue =
        plugin->RELEASE_Smoother.smoothingFactor *
            plugin->RELEASE_Smoother.currentValue +
        (1.0 - plugin->RELEASE_Smoother.smoothingFactor) *
            plugin->RELEASE_Smoother.targetValue;
    c_value = plugin->RELEASE_Smoother.currentValue;
    plugin->gate.ReleaseTime = c_value / 1000.0;
    flag = (plugin->gate.isInitialized == 1);
    if (flag) {
      plugin->gate.TunablePropsChanged = true;
    }
    if (plugin->HOLD_Smoother.isInitialized != 1) {
      plugin->HOLD_Smoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->HOLD_Smoother.currentValue =
        plugin->HOLD_Smoother.smoothingFactor *
            plugin->HOLD_Smoother.currentValue +
        (1.0 - plugin->HOLD_Smoother.smoothingFactor) *
            plugin->HOLD_Smoother.targetValue;
    c_value = plugin->HOLD_Smoother.currentValue;
    plugin->gate.HoldTime = c_value / 1000.0;
  } break;
  }
}

void processEntryPoint(bmfproj1StackData *SD, double samplesPerFrame,
                       const double i1_data[], const int i1_size[1],
                       const double i2_data[], const int i2_size[1],
                       double o1_data[], int o1_size[1], double o2_data[],
                       int o2_size[1])
{
  static const char cv[2]{'O', 'N'};
  derivedAudioPlugin *plugin;
  coder::array<double, 2U> result;
  coder::array<double, 2U> t1;
  coder::array<char, 2U> a;
  double b_y_data[4097];
  double Fs;
  double d;
  double d1;
  double d2;
  int b_loop_ub;
  int c_loop_ub;
  int i;
  int i1;
  int loop_ub;
  short i2;
  short varargin_1_idx_0_tmp;
  boolean_T b_bool;
  plugin = getPluginInstance(SD);
  loop_ub = i1_size[0];
  result.set_size(i1_size[0], 2);
  for (i = 0; i < loop_ub; i++) {
    result[i] = i1_data[i];
  }
  b_loop_ub = i2_size[0];
  for (i = 0; i < b_loop_ub; i++) {
    result[i + result.size(0)] = i2_data[i];
  }
  //  Process
  //  Pre-allocate memory
  t1.set_size(i1_size[0], 2);
  plugin->numChannels = 2.0;
  //  Initialize the filter states
  if (plugin->numChannels != 2.0) {
    //  Reset
    Fs = plugin->PrivateSampleRate;
    plugin->fs = Fs;
    plugin->numChannels = 2.0;
    //  Reset filter states
    plugin->filter_LPF.w[0] = 0.0;
    plugin->filter_HS.w[0] = 0.0;
    plugin->filter_HF.w[0] = 0.0;
    plugin->filter_MF.w[0] = 0.0;
    plugin->filter_LF.w[0] = 0.0;
    plugin->filter_LS.w[0] = 0.0;
    plugin->filter_HPF.w[0] = 0.0;
    plugin->filter_LPF.w[1] = 0.0;
    plugin->filter_HS.w[1] = 0.0;
    plugin->filter_HF.w[1] = 0.0;
    plugin->filter_MF.w[1] = 0.0;
    plugin->filter_LF.w[1] = 0.0;
    plugin->filter_LS.w[1] = 0.0;
    plugin->filter_HPF.w[1] = 0.0;
    plugin->filter_LPF.w[2] = 0.0;
    plugin->filter_HS.w[2] = 0.0;
    plugin->filter_HF.w[2] = 0.0;
    plugin->filter_MF.w[2] = 0.0;
    plugin->filter_LF.w[2] = 0.0;
    plugin->filter_LS.w[2] = 0.0;
    plugin->filter_HPF.w[2] = 0.0;
    plugin->filter_LPF.w[3] = 0.0;
    plugin->filter_HS.w[3] = 0.0;
    plugin->filter_HF.w[3] = 0.0;
    plugin->filter_MF.w[3] = 0.0;
    plugin->filter_LF.w[3] = 0.0;
    plugin->filter_LS.w[3] = 0.0;
    plugin->filter_HPF.w[3] = 0.0;
  }
  //  Apply Filter DSP
  varargin_1_idx_0_tmp = static_cast<short>(result.size(0));
  if (static_cast<short>(result.size(0)) > 2) {
    i = 2;
  } else {
    i = static_cast<short>(result.size(0));
  }
  if (i - 1 >= 0) {
    i1 = (loop_ub + 1 >= 2);
    c_loop_ub = varargin_1_idx_0_tmp;
    if (loop_ub == 1) {
      i2 = static_cast<short>(result.size(0));
    } else if (result.size(0) == 1) {
      i2 = static_cast<short>(loop_ub);
    } else if (result.size(0) == loop_ub) {
      i2 = static_cast<short>(result.size(0));
    } else if (loop_ub < result.size(0)) {
      i2 = static_cast<short>(loop_ub);
    } else {
      i2 = static_cast<short>(result.size(0));
    }
  }
  for (int ch{0}; ch < i; ch++) {
    double G_data[4096];
    double x_data[4096];
    double y_data[4096];
    double F;
    double atck_count;
    double lim;
    int i3;
    int k;
    int x_tmp;
    short inSize[8];
    boolean_T exitg1;
    //  one channel
    //  Apply Gate
    if (plugin->gate.isInitialized != 1) {
      cell_wrap_3 varSizes;
      plugin->gate.isSetupComplete = false;
      plugin->gate.isInitialized = 1;
      varSizes.f1[0] = static_cast<unsigned int>(result.size(0));
      varSizes.f1[1] = 1U;
      for (i3 = 0; i3 < 6; i3++) {
        varSizes.f1[i3 + 2] = 1U;
      }
      plugin->gate.inputVarSize[0] = varSizes;
      Fs = plugin->gate.pSampleRateDialog;
      if (plugin->gate.AttackTime != 0.0) {
        plugin->gate.pAlphaA =
            std::exp(-2.1972245773362196 / (plugin->gate.AttackTime * Fs));
      } else {
        plugin->gate.pAlphaA = 0.0;
      }
      if (plugin->gate.ReleaseTime != 0.0) {
        plugin->gate.pAlphaR =
            std::exp(-2.1972245773362196 / (plugin->gate.ReleaseTime * Fs));
      } else {
        plugin->gate.pAlphaR = 0.0;
      }
      plugin->gate.pLevelDetectionState = 0.0;
      plugin->gate.pNumChannels = 1.0;
      Fs = plugin->gate.HoldTime;
      F = plugin->gate.pSampleRateDialog;
      plugin->gate.pHoldTimeSamples = std::floor(Fs * F);
      plugin->gate.pHoldTimeState = 0.0;
      plugin->gate.isSetupComplete = true;
      plugin->gate.TunablePropsChanged = false;
      Fs = plugin->gate.HoldTime;
      F = plugin->gate.pSampleRateDialog;
      plugin->gate.pHoldTimeSamples = std::floor(Fs * F);
      plugin->gate.pLevelDetectionState = 0.0;
      Fs = plugin->gate.pSampleRateDialog;
      if (plugin->gate.AttackTime != 0.0) {
        plugin->gate.pAlphaA =
            std::exp(-2.1972245773362196 / (plugin->gate.AttackTime * Fs));
      } else {
        plugin->gate.pAlphaA = 0.0;
      }
      if (plugin->gate.ReleaseTime != 0.0) {
        plugin->gate.pAlphaR =
            std::exp(-2.1972245773362196 / (plugin->gate.ReleaseTime * Fs));
      } else {
        plugin->gate.pAlphaR = 0.0;
      }
      plugin->gate.pHoldTimeState = 0.0;
    }
    if (plugin->gate.TunablePropsChanged) {
      plugin->gate.TunablePropsChanged = false;
      Fs = plugin->gate.pSampleRateDialog;
      if (plugin->gate.AttackTime != 0.0) {
        plugin->gate.pAlphaA =
            std::exp(-2.1972245773362196 / (plugin->gate.AttackTime * Fs));
      } else {
        plugin->gate.pAlphaA = 0.0;
      }
      if (plugin->gate.ReleaseTime != 0.0) {
        plugin->gate.pAlphaR =
            std::exp(-2.1972245773362196 / (plugin->gate.ReleaseTime * Fs));
      } else {
        plugin->gate.pAlphaR = 0.0;
      }
      Fs = plugin->gate.HoldTime;
      F = plugin->gate.pSampleRateDialog;
      plugin->gate.pHoldTimeSamples = std::floor(Fs * F);
    }
    inSize[0] = varargin_1_idx_0_tmp;
    inSize[1] = 1;
    for (i3 = 0; i3 < 6; i3++) {
      inSize[i3 + 2] = 1;
    }
    k = 0;
    exitg1 = false;
    while ((!exitg1) && (k < 8)) {
      if (plugin->gate.inputVarSize[0].f1[k] !=
          static_cast<unsigned int>(inSize[k])) {
        for (i3 = 0; i3 < 8; i3++) {
          plugin->gate.inputVarSize[0].f1[i3] =
              static_cast<unsigned int>(inSize[i3]);
        }
        exitg1 = true;
      } else {
        k++;
      }
    }
    for (k = 0; k < loop_ub; k++) {
      y_data[k] = std::abs(result[k + result.size(0) * ch]);
    }
    Fs = plugin->gate.Threshold / 20.0;
    Fs = rt_powd_snf(10.0, Fs);
    for (b_loop_ub = 0; b_loop_ub < loop_ub; b_loop_ub++) {
      G_data[b_loop_ub] = 0.0;
      if (y_data[b_loop_ub] - Fs > 0.0) {
        G_data[b_loop_ub] = 1.0;
      }
    }
    std::memset(&b_y_data[0], 0,
                static_cast<unsigned int>(loop_ub + 1) * sizeof(double));
    b_y_data[0] = plugin->gate.pLevelDetectionState;
    Fs = plugin->gate.pAlphaA;
    F = plugin->gate.pAlphaR;
    atck_count = plugin->gate.pHoldTimeState;
    lim = plugin->gate.pHoldTimeSamples;
    for (b_loop_ub = 0; b_loop_ub < loop_ub; b_loop_ub++) {
      d = G_data[b_loop_ub];
      d1 = b_y_data[b_loop_ub];
      if (d == d1) {
        b_y_data[b_loop_ub + 1] = b_y_data[b_loop_ub];
      } else if (d < d1) {
        if (atck_count < lim) {
          atck_count++;
          b_y_data[b_loop_ub + 1] = b_y_data[b_loop_ub];
        } else {
          b_y_data[b_loop_ub + 1] = Fs * b_y_data[b_loop_ub] + (1.0 - Fs) * d;
        }
      } else {
        atck_count = 0.0;
        b_y_data[b_loop_ub + 1] = F * b_y_data[b_loop_ub] + (1.0 - F) * d;
      }
      G_data[b_loop_ub] = 0.0;
    }
    for (i3 = 0; i3 < c_loop_ub; i3++) {
      G_data[i3] = b_y_data[i1 + i3];
    }
    plugin->gate.pLevelDetectionState = b_y_data[loop_ub];
    plugin->gate.pHoldTimeState = atck_count;
    if (i2 != 0) {
      b_loop_ub = (result.size(0) != 1);
      i3 = i2 - 1;
      for (k = 0; k <= i3; k++) {
        x_tmp = b_loop_ub * k;
        x_data[k] = result[x_tmp + result.size(0) * ch] * G_data[x_tmp];
      }
    }
    //  Apply Filters
    b_loop_ub = bmfproj1::processBiquad(
        x_data, static_cast<int>(i2), plugin->filter_LPF.w,
        plugin->filter_LPF.a0, plugin->filter_LPF.a1, plugin->filter_LPF.a2,
        plugin->filter_LPF.b0, plugin->filter_LPF.b1, plugin->filter_LPF.b2,
        static_cast<double>(ch) + 1.0, y_data, d);
    x_tmp = bmfproj1::processBiquad(y_data, b_loop_ub, plugin->filter_HS.w,
                                    plugin->filter_HS.a0, plugin->filter_HS.a1,
                                    plugin->filter_HS.a2, plugin->filter_HS.b0,
                                    plugin->filter_HS.b1, plugin->filter_HS.b2,
                                    static_cast<double>(ch) + 1.0, x_data, d1);
    b_loop_ub = bmfproj1::processBiquad(
        x_data, x_tmp, plugin->filter_HF.w, plugin->filter_HF.a0,
        plugin->filter_HF.a1, plugin->filter_HF.a2, plugin->filter_HF.b0,
        plugin->filter_HF.b1, plugin->filter_HF.b2,
        static_cast<double>(ch) + 1.0, y_data, Fs);
    x_tmp = bmfproj1::processBiquad(y_data, b_loop_ub, plugin->filter_MF.w,
                                    plugin->filter_MF.a0, plugin->filter_MF.a1,
                                    plugin->filter_MF.a2, plugin->filter_MF.b0,
                                    plugin->filter_MF.b1, plugin->filter_MF.b2,
                                    static_cast<double>(ch) + 1.0, x_data, F);
    b_loop_ub = bmfproj1::processBiquad(
        x_data, x_tmp, plugin->filter_LF.w, plugin->filter_LF.a0,
        plugin->filter_LF.a1, plugin->filter_LF.a2, plugin->filter_LF.b0,
        plugin->filter_LF.b1, plugin->filter_LF.b2,
        static_cast<double>(ch) + 1.0, y_data, atck_count);
    x_tmp = bmfproj1::processBiquad(y_data, b_loop_ub, plugin->filter_LS.w,
                                    plugin->filter_LS.a0, plugin->filter_LS.a1,
                                    plugin->filter_LS.a2, plugin->filter_LS.b0,
                                    plugin->filter_LS.b1, plugin->filter_LS.b2,
                                    static_cast<double>(ch) + 1.0, x_data, lim);
    bmfproj1::processBiquad(x_data, x_tmp, plugin->filter_HPF.w,
                            plugin->filter_HPF.a0, plugin->filter_HPF.a1,
                            plugin->filter_HPF.a2, plugin->filter_HPF.b0,
                            plugin->filter_HPF.b1, plugin->filter_HPF.b2,
                            static_cast<double>(ch) + 1.0, y_data, d2);
    i3 = ch << 1;
    plugin->filter_LPF.w[i3] = d;
    plugin->filter_HS.w[i3] = d1;
    plugin->filter_HF.w[i3] = Fs;
    plugin->filter_MF.w[i3] = F;
    plugin->filter_LF.w[i3] = atck_count;
    plugin->filter_LS.w[i3] = lim;
    plugin->filter_HPF.w[i3] = d2;
    plugin->filter_LPF.w[i3 + 1] = d;
    plugin->filter_HS.w[i3 + 1] = d1;
    plugin->filter_HF.w[i3 + 1] = Fs;
    plugin->filter_MF.w[i3 + 1] = F;
    plugin->filter_LF.w[i3 + 1] = atck_count;
    plugin->filter_LS.w[i3 + 1] = lim;
    plugin->filter_HPF.w[i3 + 1] = d2;
    for (i3 = 0; i3 < loop_ub; i3++) {
      t1[i3 + t1.size(0) * ch] = y_data[i3];
    }
  }
  //  Check bypass
  a.set_size(1, plugin->BYPASS.size(1));
  loop_ub = plugin->BYPASS.size(1);
  for (i = 0; i < loop_ub; i++) {
    a[i] = plugin->BYPASS[i];
  }
  b_bool = false;
  if (a.size(1) == 2) {
    b_loop_ub = 0;
    int exitg2;
    do {
      exitg2 = 0;
      if (b_loop_ub < 2) {
        if (a[b_loop_ub] != cv[b_loop_ub]) {
          exitg2 = 1;
        } else {
          b_loop_ub++;
        }
      } else {
        b_bool = true;
        exitg2 = 1;
      }
    } while (exitg2 == 0);
  }
  if (b_bool) {
    t1.set_size(i1_size[0], 2);
    loop_ub = result.size(0) << 1;
    for (i = 0; i < loop_ub; i++) {
      t1[i] = result[i];
    }
  }
  if (samplesPerFrame < 1.0) {
    b_loop_ub = 0;
  } else {
    b_loop_ub = static_cast<int>(samplesPerFrame);
  }
  o1_size[0] = b_loop_ub;
  for (i = 0; i < b_loop_ub; i++) {
    o1_data[i] = t1[i];
  }
  o2_size[0] = b_loop_ub;
  for (i = 0; i < b_loop_ub; i++) {
    o2_data[i] = t1[i + t1.size(0)];
  }
}

void resetCImpl(bmfproj1StackData *SD, double rate)
{
  derivedAudioPlugin *plugin;
  double b_rate;
  plugin = getPluginInstance(SD);
  plugin->PrivateSampleRate = rate;
  //  Reset
  b_rate = plugin->PrivateSampleRate;
  plugin->fs = b_rate;
  plugin->numChannels = 2.0;
  //  Reset filter states
  plugin->filter_LPF.w[0] = 0.0;
  plugin->filter_HS.w[0] = 0.0;
  plugin->filter_HF.w[0] = 0.0;
  plugin->filter_MF.w[0] = 0.0;
  plugin->filter_LF.w[0] = 0.0;
  plugin->filter_LS.w[0] = 0.0;
  plugin->filter_HPF.w[0] = 0.0;
  plugin->filter_LPF.w[1] = 0.0;
  plugin->filter_HS.w[1] = 0.0;
  plugin->filter_HF.w[1] = 0.0;
  plugin->filter_MF.w[1] = 0.0;
  plugin->filter_LF.w[1] = 0.0;
  plugin->filter_LS.w[1] = 0.0;
  plugin->filter_HPF.w[1] = 0.0;
  plugin->filter_LPF.w[2] = 0.0;
  plugin->filter_HS.w[2] = 0.0;
  plugin->filter_HF.w[2] = 0.0;
  plugin->filter_MF.w[2] = 0.0;
  plugin->filter_LF.w[2] = 0.0;
  plugin->filter_LS.w[2] = 0.0;
  plugin->filter_HPF.w[2] = 0.0;
  plugin->filter_LPF.w[3] = 0.0;
  plugin->filter_HS.w[3] = 0.0;
  plugin->filter_HF.w[3] = 0.0;
  plugin->filter_MF.w[3] = 0.0;
  plugin->filter_LF.w[3] = 0.0;
  plugin->filter_LS.w[3] = 0.0;
  plugin->filter_HPF.w[3] = 0.0;
}

// End of code generation (bmfproj1.cpp)
