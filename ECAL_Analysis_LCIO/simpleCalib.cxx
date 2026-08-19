#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "SimpleCalibration.h"
#include "TROOT.h" //https://root-forum.cern.ch/t/trees-with-vectors/8276/2

using namespace std;

int main(int argc, char* argv[])
{
	double start = clock();
	gROOT->ProcessLine("#include <vector>"); //https://root-forum.cern.ch/t/trees-with-vectors/8276/2
	cout << "start of main program" << endl;

// 	treeCalibration t;
// 	t.Calibration(argv[1], argv[2], argv[3]);

	treeCalibration cali;
	cali.Calibration(argv[1],argv[2],argv[3]);

	cout << "end of main program" << endl;
	double end = clock();
	cout << "end of RawToRoot : Time : " << (end - start) / CLOCKS_PER_SEC << endl;
	return 0;
}
