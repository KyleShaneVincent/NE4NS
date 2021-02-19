// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

extern "C" {
#include "scratch20.h"
}

int main()
{
	int tsteps = 1000;
	double paad0 = 0.0167;
	int naad0 = 1;
	double paadtrace = 0.5;
	int aadreseeddesign = 1;
	double paadrandom = 0.01;
	int naadtarget = 400;
	int coupons = 0;
	char *outputfilepath[] = { (char*)"c:\\data\\output.txt" };
	char *nodeinputfilepath[] = { (char*)"c:\\data\\nodes.txt" };
	char *nodefiinputfilepath[] = { (char*)"c:\\data\\edges.txt" };
	char *edgeinputfilepath[] = { (char*)"c:\\data\\nodesfi.txt" };

	int a = scratch20(&tsteps, &paad0, &naad0, &paadtrace, &aadreseeddesign, &paadrandom, &naadtarget, &coupons, outputfilepath, nodeinputfilepath, nodefiinputfilepath, edgeinputfilepath);
    return 0;
}
