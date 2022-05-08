//
// MATLAB Compiler: 8.2 (R2021a)
// Date: Sun May  8 00:04:20 2022
// Arguments:
// "-B""macro_default""-W""cpplib:WaveRefractionCalc,all,version=1.0""-T""link:l
// ib""-d""C:\Users\h13\Desktop\CoastalHydrodynamicsCalculator\CoastalHydrodynam
// icsCalculator\MATLAB\WaveRefractionCalc\for_testing""-v""C:\Users\h13\Desktop
// \CoastalHydrodynamicsCalculator\CoastalHydrodynamicsCalculator\MATLAB\WaveRef
// ractionCalc.m""C:\Users\h13\Desktop\CoastalHydrodynamicsCalculator\CoastalHyd
// rodynamicsCalculator\MATLAB\WaveRefractionDiffFunction.m"
//

#ifndef WaveRefractionCalc_h
#define WaveRefractionCalc_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" { // sbcheck:ok:extern_c
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_WaveRefractionCalc_C_API 
#define LIB_WaveRefractionCalc_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_WaveRefractionCalc_C_API 
bool MW_CALL_CONV WaveRefractionCalcInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_WaveRefractionCalc_C_API 
bool MW_CALL_CONV WaveRefractionCalcInitialize(void);

extern LIB_WaveRefractionCalc_C_API 
void MW_CALL_CONV WaveRefractionCalcTerminate(void);

extern LIB_WaveRefractionCalc_C_API 
void MW_CALL_CONV WaveRefractionCalcPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_WaveRefractionCalc_C_API 
bool MW_CALL_CONV mlxWaveRefractionCalc(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                        *prhs[]);

extern LIB_WaveRefractionCalc_C_API 
bool MW_CALL_CONV mlxWaveRefractionDiffFunction(int nlhs, mxArray *plhs[], int nrhs, 
                                                mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_WaveRefractionCalc
#define PUBLIC_WaveRefractionCalc_CPP_API __declspec(dllexport)
#else
#define PUBLIC_WaveRefractionCalc_CPP_API __declspec(dllimport)
#endif

#define LIB_WaveRefractionCalc_CPP_API PUBLIC_WaveRefractionCalc_CPP_API

#else

#if !defined(LIB_WaveRefractionCalc_CPP_API)
#if defined(LIB_WaveRefractionCalc_C_API)
#define LIB_WaveRefractionCalc_CPP_API LIB_WaveRefractionCalc_C_API
#else
#define LIB_WaveRefractionCalc_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_WaveRefractionCalc_CPP_API void MW_CALL_CONV WaveRefractionCalc(int nargout, mwArray& result, const mwArray& incident_angle, const mwArray& depth, const mwArray& radius, const mwArray& tolerance, const mwArray& cycleTime);

extern LIB_WaveRefractionCalc_CPP_API void MW_CALL_CONV WaveRefractionDiffFunction(int nargout, mwArray& F, const mwArray& alpha);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
