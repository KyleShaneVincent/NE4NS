// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>

extern "C" {
#include "NetworkSampleEstimate.h"
}

int main()
{
	int tsteps = 1;
	double paad0 = 0.0167;
	int naad0 = 1;
	double paadtrace = 0.5;
	int aadreseeddesign = 1;
	double paadrandom = 0.01;
	int naadtarget = 400;
	int coupons = 0;
	char *outputfilepath[] = { (char*)"c:\\data\\output.txt" };
	char *nodeinputfilepath[] = { (char*)"c:\\data\\nodes.txt" };
	char *nodefioutputputfilepath[] = { (char*)"c:\\data\\nodesfi.txt" };
	char *edgeinputfilepath[] = { (char*)"c:\\data\\edges.txt" };
	char *buffer[] = { (char*) "" };

	int a = NetworkSampleEstimates(&tsteps, &paad0, &naad0, &paadtrace, &aadreseeddesign, &paadrandom, &naadtarget, &coupons, outputfilepath, nodeinputfilepath, nodefioutputputfilepath, edgeinputfilepath);
	int b = NetworkSampleEstimates(&tsteps, &paad0, &naad0, &paadtrace, &aadreseeddesign, &paadrandom, &naadtarget, &coupons, outputfilepath, nodeinputfilepath, nodefioutputputfilepath, edgeinputfilepath);
	int c = NetworkSampleEstimates(&tsteps, &paad0, &naad0, &paadtrace, &aadreseeddesign, &paadrandom, &naadtarget, &coupons, outputfilepath, nodeinputfilepath, nodefioutputputfilepath, edgeinputfilepath);
	//int a = NetworkSampleEstimatesConsoleOutput(&tsteps, &paad0, &naad0, &paadtrace, &aadreseeddesign, &paadrandom, &naadtarget, &coupons, nodeinputfilepath, nodefioutputputfilepath, edgeinputfilepath, buffer);

	//std::cout << "fsadf"; //buffer[0];

    return 0;
}