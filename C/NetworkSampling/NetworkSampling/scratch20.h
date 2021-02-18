#pragma once

#include "stdafx.h"

#ifdef NETWORKSAMPLING
#define NETWORKSAMPLING_API __declspec(dllexport)
#else
#define NETWORKSAMPLING_API __declspec(dllimport)
#endif

extern __declspec(dllexport) int __cdecl scratch20(int *tsteps_value, double *paad0_value, int *naad0_value, double *paadtrace_value,
												   int *aadreseeddesign_value, int *paadrandom_value, int *naadtarget, int *coupons_value,
												   char *outputfilepath, char *nodeinputfilepath, char *nodefiinputfilepath, char *edgeinputfilepath);
