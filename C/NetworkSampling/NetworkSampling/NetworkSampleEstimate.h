#pragma once

#include "stdafx.h"

#ifdef NETWORKSAMPLING
#define NETWORKSAMPLING_API __declspec(dllexport)
#else
#define NETWORKSAMPLING_API __declspec(dllimport)
#endif

extern __declspec(dllexport) int __cdecl NetworkSampleEstimates(int *tsteps_value, double *paad0_value, int *naad0_value, double *paadtrace_value,
												   int *aadreseeddesign_value, double *paadrandom_value, int *naadtarget_value, int *coupons_value,
												   char **outputfilepath, char **nodeinputfilepath, char **nodefioutputputfilepath, char **edgeinputfilepath);

extern __declspec(dllexport) int __cdecl NetworkSampleEstimatesConsoleOutput(int *tsteps_value, double *paad0_value, int *naad0_value, double *paadtrace_value,
													int *aadreseeddesign_value, double *paadrandom_value, int *naadtarget_value, int *coupons_value,
													char **outputfilepath, char **nodeinputfilepath, char **nodefioutputputfilepath, char **edgeinputfilepath, char **outputbuffer);

													