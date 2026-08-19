#include <iostream>
#include <TROOT.h>
#include<stdio.h>
#include<stdlib.h>
#include"InterCalib.h"

using namespace std;

Int_t main(int argc,char *argv[])
{
    double start = clock();
    double end = clock();
  
    interCalib calib(argv[1],argv[2]);
    
    end = clock();
    cout<<"end of selectSPS : Time : "<<(end-start)/CLOCKS_PER_SEC<<endl;
  return 0;
}
