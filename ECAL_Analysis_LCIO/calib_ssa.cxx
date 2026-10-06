#include <iostream>
#include<stdio.h>
#include<stdlib.h>
//#include "Pedestal.h"
//#include "Calibration.h"
#include "CalibStripSplit.h"
//#include "Clustering.h"
//#include "Shower.h"

using namespace std;

int main(int argc,char *argv[])
{
    double start = clock();
    cout<<"start of main program"<<endl;
/*
    treePedestal p;
    p.Pedestal(argv[1],argv[2],argv[6],argv[7]);
*
    treeCalibration t;
    t.Calibration(argv[1],argv[2],argv[3],argv[4],argv[5],argv[6],argv[8],argv[9],argv[13]);
*/
    treeCalibStripSplit st;
    st.StripSplit(argv[1],argv[2],argv[3]);
//    st.StripSplit(argv[2],argv[9],argv[10]);
/*
    treeClustering c;
    c.Clustering(argv[2],argv[10],argv[11]);
/
    treeShower sh;
    sh.Shower(argv[2],argv[11],argv[12]);
*/
    cout<<"end of main program"<<endl;
    double end = clock();
    cout<<"end of RawToRoot : Time : "<<(end-start)/CLOCKS_PER_SEC<<endl;
    return 0;
}
