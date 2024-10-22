//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// bmfproj1_types.h
//
// Code generation for function 'onParamChangeCImpl'
//

#ifndef BMFPROJ1_TYPES_H
#define BMFPROJ1_TYPES_H

// Include files
#include "bmfproj1.h"
#include "rtwtypes.h"

// Type Definitions
struct bmfproj1PersistentData {
  derivedAudioPlugin plugin;
  boolean_T plugin_not_empty;
  boolean_T thisPtr_not_empty;
};

struct bmfproj1StackData {
  bmfproj1PersistentData *pd;
};

#endif
// End of code generation (bmfproj1_types.h)
