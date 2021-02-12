#pragma once

#include "stdafx.h"

#ifdef NETWORKSAMPLING
#define NETWORKSAMPLING_API __declspec(dllexport)
#else
#define NETWORKSAMPLING_API __declspec(dllimport)
#endif

extern NETWORKSAMPLING_API int scratch20();
