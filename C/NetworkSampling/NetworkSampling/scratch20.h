#pragma once

#include "stdafx.h"

#ifdef NETWORKSAMPLING
#define NETWORKSAMPLING_API __declspec(dllexport)
#else
#define NETWORKSAMPLING_API __declspec(dllimport)
#endif

extern __declspec(dllexport) int __cdecl scratch20();
