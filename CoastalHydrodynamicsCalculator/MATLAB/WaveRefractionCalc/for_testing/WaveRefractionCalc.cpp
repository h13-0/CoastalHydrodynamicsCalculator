//
// MATLAB Compiler: 7.1 (R2019b)
// Date: Sun May  8 17:39:24 2022
// Arguments:
// "-B""macro_default""-W""cpplib:WaveRefractionCalc,all,version=1.0""-T""link:l
// ib""-d""C:\Users\h13\Desktop\CoastalHydrodynamicsCalculator\CoastalHydrodynam
// icsCalculator\MATLAB\WaveRefractionCalc\for_testing""-v""C:\Users\h13\Desktop
// \CoastalHydrodynamicsCalculator\CoastalHydrodynamicsCalculator\MATLAB\WaveRef
// ractionCalc.m""C:\Users\h13\Desktop\CoastalHydrodynamicsCalculator\CoastalHyd
// rodynamicsCalculator\MATLAB\WaveRefractionDiffFunction.m"
//

#define EXPORTING_WaveRefractionCalc 1
#include "WaveRefractionCalc.h"

static HMCRINSTANCE _mcr_inst = NULL; /* don't use nullptr; this may be either C or C++ */

#if defined( _MSC_VER) || defined(__LCC__) || defined(__MINGW64__)
#ifdef __LCC__
#undef EXTERN_C
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define NOMINMAX
#include <windows.h>
#undef interface

static char path_to_dll[_MAX_PATH];

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, void *pv)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        if (GetModuleFileName(hInstance, path_to_dll, _MAX_PATH) == 0)
            return FALSE;
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
    }
    return TRUE;
}
#endif
#ifdef __cplusplus
extern "C" { // sbcheck:ok:extern_c
#endif

static int mclDefaultPrintHandler(const char *s)
{
    return mclWrite(1 /* stdout */, s, sizeof(char)*strlen(s));
}

#ifdef __cplusplus
} /* End extern C block */
#endif

#ifdef __cplusplus
extern "C" { // sbcheck:ok:extern_c
#endif

static int mclDefaultErrorHandler(const char *s)
{
    int written = 0;
    size_t len = 0;
    len = strlen(s);
    written = mclWrite(2 /* stderr */, s, sizeof(char)*len);
    if (len > 0 && s[ len-1 ] != '\n')
        written += mclWrite(2 /* stderr */, "\n", sizeof(char));
    return written;
}

#ifdef __cplusplus
} /* End extern C block */
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_WaveRefractionCalc_C_API
#define LIB_WaveRefractionCalc_C_API /* No special import/export declaration */
#endif

LIB_WaveRefractionCalc_C_API 
bool MW_CALL_CONV WaveRefractionCalcInitializeWithHandlers(
    mclOutputHandlerFcn error_handler,
    mclOutputHandlerFcn print_handler)
{
    int bResult = 0;
    if (_mcr_inst)
        return true;
    if (!mclmcrInitialize())
        return false;
    if (!GetModuleFileName(GetModuleHandle("WaveRefractionCalc"), path_to_dll, _MAX_PATH))
        return false;
    {
        mclCtfStream ctfStream = 
            mclGetEmbeddedCtfStream(path_to_dll);
        if (ctfStream) {
            bResult = mclInitializeComponentInstanceEmbedded(&_mcr_inst,
                                                             error_handler, 
                                                             print_handler,
                                                             ctfStream);
            mclDestroyStream(ctfStream);
        } else {
            bResult = 0;
        }
    }  
    if (!bResult)
    return false;
    return true;
}

LIB_WaveRefractionCalc_C_API 
bool MW_CALL_CONV WaveRefractionCalcInitialize(void)
{
    return WaveRefractionCalcInitializeWithHandlers(mclDefaultErrorHandler, 
                                                  mclDefaultPrintHandler);
}

LIB_WaveRefractionCalc_C_API 
void MW_CALL_CONV WaveRefractionCalcTerminate(void)
{
    if (_mcr_inst)
        mclTerminateInstance(&_mcr_inst);
}

LIB_WaveRefractionCalc_C_API 
void MW_CALL_CONV WaveRefractionCalcPrintStackTrace(void) 
{
    char** stackTrace;
    int stackDepth = mclGetStackTrace(&stackTrace);
    int i;
    for(i=0; i<stackDepth; i++)
    {
        mclWrite(2 /* stderr */, stackTrace[i], sizeof(char)*strlen(stackTrace[i]));
        mclWrite(2 /* stderr */, "\n", sizeof(char)*strlen("\n"));
    }
    mclFreeStackTrace(&stackTrace, stackDepth);
}


LIB_WaveRefractionCalc_C_API 
bool MW_CALL_CONV mlxWaveRefractionCalc(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                        *prhs[])
{
    return mclFeval(_mcr_inst, "WaveRefractionCalc", nlhs, plhs, nrhs, prhs);
}

LIB_WaveRefractionCalc_C_API 
bool MW_CALL_CONV mlxWaveRefractionDiffFunction(int nlhs, mxArray *plhs[], int nrhs, 
                                                mxArray *prhs[])
{
    return mclFeval(_mcr_inst, "WaveRefractionDiffFunction", nlhs, plhs, nrhs, prhs);
}

LIB_WaveRefractionCalc_CPP_API 
void MW_CALL_CONV WaveRefractionCalc(int nargout, mwArray& boundary_x, mwArray& 
                                     boundary_y, mwArray& boundary_angle, mwArray& 
                                     internal_x, mwArray& internal_y, mwArray& 
                                     internal_angle, const mwArray& incident_angle, const 
                                     mwArray& depth, const mwArray& radius, const 
                                     mwArray& tolerance, const mwArray& period)
{
    mclcppMlfFeval(_mcr_inst, "WaveRefractionCalc", nargout, 6, 5, &boundary_x, &boundary_y, &boundary_angle, &internal_x, &internal_y, &internal_angle, &incident_angle, &depth, &radius, &tolerance, &period);
}

LIB_WaveRefractionCalc_CPP_API 
void MW_CALL_CONV WaveRefractionDiffFunction(int nargout, mwArray& F, const mwArray& 
                                             alpha)
{
    mclcppMlfFeval(_mcr_inst, "WaveRefractionDiffFunction", nargout, 1, 1, &F, &alpha);
}

