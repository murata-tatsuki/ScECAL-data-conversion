#include <iostream>
#include <TROOT.h>
#include <stdio.h>
#include <stdlib.h>
#include "TrackFit_.h"

using namespace std;

int main(int argc, char* argv[])
{
	double begin = clock();

	trackFit_ tr(argv[1], argv[2], argv[3]);

	double end = clock();

	cout << "total time : " << (end - begin) / CLOCKS_PER_SEC << endl;
	return 0;
}
