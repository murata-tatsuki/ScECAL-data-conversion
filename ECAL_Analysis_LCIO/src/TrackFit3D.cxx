#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <TFile.h>
#include <TTree.h>
#include <vector>
#include <TROOT.h>
#include "TMinuit.h"
#include <TStyle.h>
#include "TrackFit3D.h"
#include "EBUdecode.h"
#include "line3Dfit.cpp"

using namespace std;

vector<double> xPosition3D;
vector<double> yPosition3D;
vector<double> zPosition3D;
vector<float> zxPosition3D;
vector<float> zyPosition3D;
//----------------------------------------------------
void trackFit3D::graphInit()
{
  // char gchargeName[200];
  Glist1 = new TObjArray(10);

  // for(int l=0; l!=nentry; ++l)
  // {
  //   sprintf(gchargeName,"Charges_layer%d_chip%d_chan%d",l,i,j);
  //   hCharge2[l][i][j] = new TH1F(hChargeName,hChargeName,4096,0,4095);
  //   Glist1->Add(hCharge2[l][i][j]);                
  //   sprintf(gchargeName,"Times_layer%d_chip%d_chan%d",l,i,j);
  //   hTime2[l][i][j] = new TH1F(hChargeName,hChargeName,4096,0,4095);
  //   Glist1->Add(hTime2[l][i][j]);
  // }
}
void trackFit3D::Clear()
{
	_trackFitPars.clear();
	_residualX.clear();
	_residualY.clear();
  // _residualTag.clear();
	_chargeCor.clear();
	_timeCor.clear();
  _energyCor.clear();

	_hitCycleID = 0;
  _hitTriggerID = 0;
  _hitCell.clear();
	_hitBCIDs.clear();
  _bcid_cali.clear();
	_hitPosX.clear();
	_hitPosY.clear();
	_hitPosZ.clear();
	_hitCharge.clear();
  _hitTime.clear();

	_hitCellnew.clear();
	_hitPosXnew.clear();
	_hitPosYnew.clear();
	_hitPosZnew.clear();
	_layer_efficiency.clear();

	xPosition3D.clear();
	yPosition3D.clear();
  zPosition3D.clear();
	zxPosition3D.clear();
	zyPosition3D.clear();
}
//----------------------------------------------------
void trackFit3D::Reset()
{
	xPosition3D.clear();
	yPosition3D.clear();
  zPosition3D.clear();
	zxPosition3D.clear();
	zyPosition3D.clear();
}
//----------------------------------------------------
// definition of function of Track Fitting
//----------------------------------------------------
void fcnX3d(Int_t& npar, Double_t* gin, Double_t& f, Double_t* par, Int_t iflag)
{
	const Int_t nbins = xPosition3D.size();
	//calculate chisquare
	Double_t chisq = 0;
	Double_t delta;
	for (int i = 0; i < nbins; i++) {
		delta = (xPosition3D[i] - (zxPosition3D[i] * par[0] + par[1]));
		chisq += delta * delta;
	}
	f = chisq;
}
void fcnY3d(Int_t& npar, Double_t* gin, Double_t& f, Double_t* par, Int_t iflag)
{
	const Int_t nbins = yPosition3D.size();
	//calculate chisquare
	Double_t chisq = 0;
	Double_t delta;
	for (int i = 0; i < nbins; i++) {
		delta = (yPosition3D[i] - (zyPosition3D[i] * par[0] + par[1]));
		chisq += delta * delta;
	}
	f = chisq;
}
//----------------------------------------------------
double* trackFit3D::TrackFit3D(int slience)
{
	Double_t arglist[10];
	Int_t ierflg = 0;
	static Double_t vstart[2];
	static Double_t step[2];
	static double fitpar[8] = { 0 };
	if (slience)
		cout << "====================================================================" << endl;
	//initialize TMinuit with a maximum of 2 params for XZ
	gMinuitX = new TMinuit();
	gMinuitX->SetFCN(fcnX3d);
	gMinuitX->SetPrintLevel(-1);

	arglist[0] = 1;
	gMinuitX->mnexcm("SET ERR", arglist, 1, ierflg);

	Int_t nbins = xPosition3D.size();
	//for(int i=0; i!=nbins; ++i){ cout<<xPosition[i]<<" , "<<endl;}
	double zLeft = 291.55;
	double zRight = 1;
	for (int i = 0; i != nbins; ++i) {
		if (zxPosition3D[i] < zLeft)
			zLeft = zxPosition3D[i];
		if (zxPosition3D[i] > zRight)
			zRight = zxPosition3D[i];
	}
	int itXleft, itXright;
	for (int i = 0; i != nbins; ++i) {
		if (zxPosition3D[i] == zLeft)
			itXleft = i;
		if (zxPosition3D[i] == zRight)
			itXright = i;
	}
	double xLeft = xPosition3D[itXleft];
	double xRight = xPosition3D[itXright];
	if ((zRight - zLeft) == 0)
		vstart[0] = 0;
	else
		vstart[0] = (xRight - xLeft) / (zRight - zLeft);
	//vstart[0] = (xPosition[nbins-1]-xPosition[0])/(zxPosition[nbins-1]-zxPosition[0]);
	vstart[1] = xPosition3D[0] - vstart[0] * zxPosition3D[0];
	step[0] = fabs(0.05 * vstart[0]);
	step[1] = fabs(0.05 * vstart[1]);
	//cout<<"Initial Parameter x : "<<vstart[0]<<" "<<vstart[1]<<" "<<step[0]<<" "<<step[1]<<endl;
	gMinuitX->mnparm(0, "Xslope", vstart[0], step[0], -11000, 11000, ierflg);
	gMinuitX->mnparm(1, "Xintercept", vstart[1], step[1], -10000, 10000, ierflg);

	arglist[0] = 500;
	arglist[1] = 1.;
	gMinuitX->mnexcm("MIGRAD", arglist, 0, ierflg);

	gMinuitX->GetParameter(0, fitpar[0], fitpar[1]);
	gMinuitX->GetParameter(1, fitpar[2], fitpar[3]);

	if (slience)
		cout << "====================================================================" << endl;
	//initialize TMinuit with a maximum of 2 params for YZ
	gMinuitY = new TMinuit();
	gMinuitY->SetFCN(fcnY3d);
	gMinuitY->SetPrintLevel(-1);

	arglist[0] = 1;
	gMinuitY->mnexcm("SET ERR", arglist, 1, ierflg);

	nbins = yPosition3D.size();
	//for(int i=0; i!=nbins; ++i){ cout<<yPosition[i]<<" , "<<endl;}
	zLeft = 291.55;
	zRight = 1;
	for (int i = 0; i != nbins; ++i) {
		if (zyPosition3D[i] < zLeft)
			zLeft = zyPosition3D[i];
		if (zyPosition3D[i] > zRight)
			zRight = zyPosition3D[i];
	}
	int itYleft, itYright;
	for (int i = 0; i != nbins; ++i) {
		if (zyPosition3D[i] == zLeft)
			itYleft = i;
		if (zyPosition3D[i] == zRight)
			itYright = i;
	}
	double yLeft = yPosition3D[itYleft];
	double yRight = yPosition3D[itYright];
	if ((zRight - zLeft) == 0)
		vstart[0] = 0;
	else
		vstart[0] = (yRight - yLeft) / (zRight - zLeft);
	//vstart[0] = (yPosition[nbins-1]-yPosition[0])/(zyPosition[nbins-1]-zyPosition[0]);
	vstart[1] = yPosition3D[0] - vstart[0] * zyPosition3D[0];
	step[0] = fabs(0.05 * vstart[0]);
	step[1] = fabs(0.05 * vstart[1]);
	//cout<<"Initial Parameter y : "<<vstart[0]<<" "<<vstart[1]<<" "<<step[0]<<" "<<step[1]<<endl;
	gMinuitY->mnparm(0, "Yslope", vstart[0], step[0], -11000, 11000, ierflg);
	gMinuitY->mnparm(1, "Yintercept", vstart[1], step[1], -10000, 10000, ierflg);

	arglist[0] = 500;
	arglist[1] = 1.;
	gMinuitY->mnexcm("MIGRAD", arglist, 0, ierflg);

	gMinuitY->GetParameter(0, fitpar[4], fitpar[5]);
	gMinuitY->GetParameter(1, fitpar[6], fitpar[7]);

	if (slience) {
		cout << "Fitting Pars XZ slope :" << fitpar[0] << " +- " << fitpar[1] << " intercept : " << fitpar[2] << " +- " << fitpar[3] << endl;
		cout << "Fitting Pars YZ slope :" << fitpar[4] << " +- " << fitpar[5] << " intercept : " << fitpar[6] << " +- " << fitpar[7] << endl;
		cout << "====================================================================" << endl;
	}

	delete gMinuitX;
	gMinuitX = NULL;
	delete gMinuitY;
	gMinuitY = NULL;
	return fitpar;
}
//----------------------------------------------------
trackFit3D::trackFit3D(string root_name, string track_name, string layer)
{
  graphInit();

	cout << " ----------- " << root_name << " ------------" << endl;
	string rootname(root_name);
	rootname.erase(0, rootname.find_last_of("/") + 1);
	cout << " ----------- " << rootname << " ------------" << endl;
	//txtname.insert( txtname.length(), ".root");

	string txtname(root_name);
	rootfile = new TFile(txtname.c_str());
	if (!rootfile) {
		cout << " open align file falied" << endl;
		return;
	}
	fNtuple1 = (TTree*)rootfile->Get("T_Event");
	int Entries = fNtuple1->GetEntries();
	cout << " ------------ Total Events = " << Entries << " ---------------" << endl;

	fNtuple1->SetBranchAddress("cycleID", &_cycleID);
	fNtuple1->SetBranchAddress("triggerID", &_triggerID);
	fNtuple1->SetBranchAddress("cellIDs", &_cellID);
	fNtuple1->SetBranchAddress("BCIDs", &_bcid);
	fNtuple1->SetBranchAddress("hitTags", &_hitTag);
	fNtuple1->SetBranchAddress("gainTags", &_gainTag);
	fNtuple1->SetBranchAddress("charges", &_charge);
	fNtuple1->SetBranchAddress("times", &_time);
	fNtuple1->SetBranchAddress("posX", &_posX);
	fNtuple1->SetBranchAddress("posY", &_posY);
	fNtuple1->SetBranchAddress("posZ", &_posZ);
	fNtuple1->SetBranchAddress("ssaTag", &_ssaTag);

	string trackname(track_name);
	trackname.insert(trackname.length(), rootname);
	fitfile = new TFile(trackname.c_str(), "RECREATE");
	fTree = new TTree("T_Event", "");
	// fTree = fNtuple1->CloneTree(0);
	fTree->Branch("cycleID", &_cycleID_cali);
	fTree->Branch("triggerID", &_triggerID_cali);
	fTree->Branch("cellIDs", &_cellID_cali);
	fTree->Branch("BCIDs", &_bcid_cali);
	fTree->Branch("hitTags", &_hitTag_cali);
	fTree->Branch("gainTags", &_gainTag_cali);
	fTree->Branch("charges", &_charge_cali);
	fTree->Branch("times", &_time_cali);
	fTree->Branch("trackFitPars", &_trackFitPars);
	fTree->Branch("residualX", &_residualX);
	fTree->Branch("residualY", &_residualY);
	// fTree->Branch("residualTag", &_residualTag);
	fTree->Branch("chargenew", &_chargeCor);
	fTree->Branch("timenew", &_timeCor);
	fTree->Branch("energyDep", &_energyCor);
	fTree->Branch("realLength", &_realLength);
	//fTree ->Branch("cellIDs",&_hitCellnew);
	fTree->Branch("hitCellnew", &_hitCellnew);
	fTree->Branch("hitPosXnew", &_hitPosXnew);
	fTree->Branch("hitPosYnew", &_hitPosYnew);
	fTree->Branch("hitPosZnew", &_hitPosZnew);
	fTree->Branch("layer_efficiency", &_layer_efficiency);

	// ========================= Total hit events for each channel ===================

	int _totalEntries1 = 0;
	int _totalEntries2 = 0;
	int _totalEntries3 = 0;
	int _totalEntries4 = 0;
	int _selectedLayerID = 0; // stoi(event);
	stringstream degree(layer);
	degree >> _selectedLayerID;

  const int itNo = 9;
  // TGraph2D * gr[fNtuple1->GetEntries()][itNo+1];  // 2D graph for fitting track
  TGraph2DErrors * gr[fNtuple1->GetEntries()];  // 2D graph for fitting track

	for (int entry = 0; entry != fNtuple1->GetEntries(); entry++) {
		fNtuple1->GetEntry(entry);
		Clear();
		if (entry % 1000 == 0) cout << " Process : Events = " << entry << endl;
		for (unsigned hit = 0; hit != _bcid->size(); ++hit) {
			int _layerID = _cellID->at(hit) / 1e6;
			int _chipID = _cellID->at(hit) % 1000000 / 1e4;
			//  int _cell = _cellID->at(hit)%10000/1e2;
			int _chnID = _cellID->at(hit) % 100;
			//  int gainTag = _gainTag->at(hit);
			double charge = _charge->at(hit);
			double time = _time->at(hit);
			double posX = _posX->at(hit);
			double posY = _posY->at(hit);
			double posZ = _posZ->at(hit);
			int ssaTag = _ssaTag->at(hit);
			//int time = _time->at(hit);
			if (_layerID >= 30)
				continue;
      if(ssaTag==0) continue;

			// int scintillatorID = _chipID * chnNu + _chnID;

			_hitCycleID = _cycleID;
      _hitTriggerID = _triggerID;
			_hitCell.push_back(_cellID->at(hit));
			_hitBCIDs.push_back(_bcid->at(hit));
			// _position = EBUdecode(_layerID, _chipID, _chnID);
			// no aligngment for now (jan. 19, 2023)
			// // alignment for each EBU based on the track fitting
			// if (_layerID % 2 == 1)
			// 	*_position = *(_position) - EBUposX[_layerID / 2] - EBUposX2[_layerID / 2];
			// if (_layerID % 2 == 0)
			// 	*(_position + 1) = *(_position + 1) - EBUposY[_layerID / 2] - EBUposY2[_layerID / 2];
			_hitPosX.push_back(posX);
			_hitPosY.push_back(posY);
			_hitPosZ.push_back(posZ);
			_hitCharge.push_back(charge);
			_hitTime.push_back(time);
			//cout<<_layerID<<" : "<<hit<<" : "<<*_position<<" : "<<*(_position+1)<<" : "<<*(_position+2)<<endl;
		}

		// ---------------------- iteration fitting 1 ---------------------
		double* _fitPar;
		for (unsigned _chanl = 0; _chanl != _hitCell.size(); ++_chanl) {
			int layerID = _hitCell.at(_chanl) / 1e6;
			double _xPosition = _hitPosX.at(_chanl);
			double _yPosition = _hitPosY.at(_chanl);
			double _zPosition = _hitPosZ.at(_chanl);
			double _chargeSignal = _hitCharge.at(_chanl);
			double _timeSignal = _hitTime.at(_chanl);
      xPosition3D.push_back(_xPosition);
      yPosition3D.push_back(_yPosition);
			zPosition3D.push_back(_zPosition);
		}
		if (xPosition3D.size() < 5 || yPosition3D.size() < 5)
			continue;
    gr[entry] = new TGraph2DErrors();
    gr[entry]->SetTitle(Form("event number %d iteration%d ; x [mm]; y [mm]; x [mm]",entry,0));
    gr[entry]->SetName(Form("graph_event_%d_itr_%d",entry,0));
    Glist1->Add(gr[entry]);
		// _fitPar = TrackFit3D(0);
		// _fitPar = line3Dfit(xPosition3D, yPosition3D, zPosition3D, gr[entry][0]);
		_fitPar = line3Dfit(xPosition3D, yPosition3D, zPosition3D, gr[entry]);
    vector<double> trackFitPars_0{*(_fitPar), *(_fitPar+1), *(_fitPar+2), *(_fitPar+3), *(_fitPar+4), *(_fitPar+5), *(_fitPar+6), *(_fitPar+7), *(_fitPar+8), *(_fitPar+9)};
    _trackFitPars.push_back(trackFitPars_0);
		_totalEntries1++;
		// ---------------------- iteration fitting 2 ---------------------
		double step = 5;
		double start = 47.5;
		for (int it = 0; it != itNo; it++) {
			Reset();
			for (unsigned _chanl = 0; _chanl != _hitCell.size(); ++_chanl) {
				int layerID = _hitCell.at(_chanl) / 1e6;
				double _xPosition = _hitPosX.at(_chanl);
				double _yPosition = _hitPosY.at(_chanl);
				double _zPosition = _hitPosZ.at(_chanl);
				double xSlope = *(_fitPar);
				double xIntercept = *(_fitPar + 2);
				double ySlope = *(_fitPar + 4);
				double yIntercept = *(_fitPar + 6);
				double _xFitting = xSlope * _zPosition + xIntercept;
				double _yFitting = ySlope * _zPosition + yIntercept;
				
				if (fabs(_yFitting - _yPosition) > start - step * it) continue;
        if (fabs(_xFitting - _xPosition) > start - step * it) continue;
				xPosition3D.push_back(_xPosition);
				yPosition3D.push_back(_yPosition);
				zPosition3D.push_back(_zPosition);
				zyPosition3D.push_back(_zPosition);
        zxPosition3D.push_back(_zPosition);
			}
			if (xPosition3D.size() < 5 || yPosition3D.size() < 5)
				continue;
			// _fitPar = TrackFit3D(0);
      // gr[entry][it+1] = new TGraph2D();
      // gr[entry][it+1]->SetTitle(Form("event number %d iteration%d ; x [mm]; y [mm]; x [mm]",entry,it+1));
      // gr[entry][it+1]->SetName(Form("graph_event_%d_itr_%d",entry,it+1));
      // Glist1->Add(gr[entry][it+1]);
			// _fitPar = line3Dfit(xPosition3D, yPosition3D, zPosition3D, gr[entry][it+1]);
			_fitPar = line3Dfit(xPosition3D, yPosition3D, zPosition3D);
      vector<double> trackFitPars_itr{*(_fitPar), *(_fitPar+1), *(_fitPar+2), *(_fitPar+3), *(_fitPar+4), *(_fitPar+5), *(_fitPar+6), *(_fitPar+7), *(_fitPar+8), *(_fitPar+9)};
      _trackFitPars.push_back(trackFitPars_itr);
			// if(*(_fitPar+8)>0) cout << "chi^2 : " << *(_fitPar+8) << ", ndf : " << *(_fitPar+9) << endl;;
			// if(*(_fitPar+8)>0) for(int iii=0;iii<xPosition3D.size();iii++) cout << "   " << xPosition3D[iii] << ", " << yPosition3D[iii] << ", " << zPosition3D[iii] << endl;
		}

		// ====================== selection of track fitting 1  ================================
		double xSlope = *(_fitPar);
		double xIntercept = *(_fitPar + 2);
		double ySlope = *(_fitPar + 4);
		double yIntercept = *(_fitPar + 6);
		double xLeft = xIntercept + xSlope * (12.95);
		double yLeft = yIntercept + ySlope * (12.95);
		double xRitgh = xIntercept + xSlope * (12.95 + 28 / 2 * 19.9);
		double yRitgh = yIntercept + ySlope * (12.95 + 28 / 2 * 19.9);
		double xPhi = TMath::ATan(xSlope);
		double yPhi = TMath::ATan(ySlope);
		if (fabs(xRitgh) > 114 || fabs(xLeft) > 114 || fabs(yLeft) > 114 || fabs(yRitgh) > 114 || fabs(xPhi) > 0.7 || fabs(yPhi) > 0.7)
			continue;
		_totalEntries2++;

		// cout<<" ============================= finish track fitting =================================="<<endl;
		vector<vector<double> > minHit;
		minHit.resize(layerNu);
		vector<vector<double> > minCell;
		minCell.resize(layerNu);
		vector<vector<double> > hitCellX;
		hitCellX.resize(layerNu);
		vector<vector<double> > hitCellY;
		hitCellY.resize(layerNu);
		for (unsigned hit = 0; hit != _hitCell.size(); ++hit) {
			int layerID = _hitCell.at(hit) / 1e6;
			double _xPosition = _hitPosX.at(hit);
			double _yPosition = _hitPosY.at(hit);
			double _zPosition = _hitPosZ.at(hit);
			double _xFitting = xSlope * _zPosition + xIntercept;
			double _yFitting = ySlope * _zPosition + yIntercept;
			double _xResidual = fabs(_xPosition - _xFitting);
			double _yResidual = fabs(_yPosition - _yFitting);

			if (layerID % 2 == 0) {
				if (minHit[layerID].size() == 0) {
					minHit[layerID].push_back(_yResidual);
					minCell[layerID].push_back(_hitCell.at(hit));
				} else if (_yResidual < minHit[layerID].at(0)) {
					minHit[layerID].at(0) = _yResidual;
					minCell[layerID].at(0) = _hitCell.at(hit);
				}
			}
			if (layerID % 2 == 1) {
				if (minHit[layerID].size() == 0) {
					minHit[layerID].push_back(_xResidual);
					minCell[layerID].push_back(_hitCell.at(hit));
				} else if (_xResidual < minHit[layerID].at(0)) {
					minHit[layerID].at(0) = _xResidual;
					minCell[layerID].at(0) = _hitCell.at(hit);
				}
			}
		}
		Reset();

		int _realHitX = 0;
		int _realHitY = 0;
		for (unsigned _chanl = 0; _chanl != _hitCell.size(); ++_chanl) {
			int layerID = _hitCell.at(_chanl) / 1e6;
			double _xPosition = _hitPosX.at(_chanl);
			double _yPosition = _hitPosY.at(_chanl);
			double _zPosition = _hitPosZ.at(_chanl);
			double _xFitting = xSlope * _zPosition + xIntercept;
			double _yFitting = ySlope * _zPosition + yIntercept;
			double _xResidual = fabs(_xPosition - _xFitting);
			double _yResidual = fabs(_yPosition - _yFitting);
			int trueHit = 0;
			//if(layerID%2==0&&fabs(_yFitting-_yPosition)<=7.5) {trueHit++;_realHitY++;}
			//if(layerID%2==1&&fabs(_xFitting-_xPosition)<=7.5) {trueHit++;_realHitX++;}
			if (layerID % 2 == 0 && _yResidual <= 7.5 && minCell[layerID].at(0) == _hitCell.at(_chanl)) {
				trueHit++;
				_realHitY++;
			}
			if (layerID % 2 == 1 && _xResidual <= 7.5 && minCell[layerID].at(0) == _hitCell.at(_chanl)) {
				trueHit++;
				_realHitX++;
			}
			if (trueHit == 0)
				continue;
			//_hitCellnew.push_back(layerID);
      _cycleID_cali = _hitCycleID;
      _triggerID_cali = _hitTriggerID;
      _bcid_cali.push_back(_hitBCIDs.at(_chanl));
			_hitCellnew.push_back(_hitCell.at(_chanl));
			_hitPosXnew.push_back(_xPosition);
			_hitPosYnew.push_back(_yPosition);
			_hitPosZnew.push_back(_zPosition);
      _chargeCor.push_back(_hitCharge.at(_chanl));
      _timeCor.push_back(_hitTime.at(_chanl));
			if (layerID % 2 == 0) {
				xPosition3D.push_back(_xPosition);
				yPosition3D.push_back(_yPosition);
				zPosition3D.push_back(_zPosition);
				zyPosition3D.push_back(_zPosition);
        zxPosition3D.push_back(_zPosition);
				hitCellY[layerID].push_back(_hitCell.at(_chanl));
			}
			if (layerID % 2 == 1) {
				xPosition3D.push_back(_xPosition);
				yPosition3D.push_back(_yPosition);
				zPosition3D.push_back(_zPosition);
        zyPosition3D.push_back(_zPosition);
				zxPosition3D.push_back(_zPosition);
				hitCellX[layerID].push_back(_hitCell.at(_chanl));
			}
		}
		if (_realHitX < 5 || _realHitY < 5)
			continue;
		_fitPar = line3Dfit(xPosition3D, yPosition3D, zPosition3D);
    vector<double> trackFitPars_final{*(_fitPar), *(_fitPar+1), *(_fitPar+2), *(_fitPar+3), *(_fitPar+4), *(_fitPar+5), *(_fitPar+6), *(_fitPar+7), *(_fitPar+8), *(_fitPar+9)};
    _trackFitPars.push_back(trackFitPars_final);
		// for (int ii = 0; ii != 8; ++ii) {
		// 	// _trackFitPars.push_back(*(_fitPar + ii));
		// 	_trackFitPars.push_back((_fitPar));
		// }

    // it is not self-trigger at the beam test, so comment out this
		// // ---------------------- coincidence of trigger layers ---------------------
		// double _hitLayer[layerNu] = { 0 };
		// for (unsigned _chanl = 0; _chanl != _hitCellnew.size(); ++_chanl) {
		// 	int layerID = _hitCellnew.at(_chanl) / 1e6;
		// 	_hitLayer[layerID] = 1;
		// }
		// if (_hitLayer[1] == 0 || _hitLayer[29] == 0)
		// 	continue;

		// ====================== selection of track fitting 2 ================================
		double xNDF = 0;
		double yNDF = 0;
		vector<int> totalHitLayerX;
		vector<int> totalHitLayerY;
		for (unsigned _chanl = 0; _chanl != _hitCellnew.size(); ++_chanl) {
			int layerID = _hitCellnew.at(_chanl) / 1e6;
			double _xPosition = _hitPosXnew.at(_chanl);
			double _yPosition = _hitPosYnew.at(_chanl);
			double _zPosition = _hitPosZnew.at(_chanl);
			double _xFitting = xSlope * _zPosition + xIntercept;
			double _yFitting = ySlope * _zPosition + yIntercept;
			double _xResidual = _xPosition - _xFitting;
			double _yResidual = _yPosition - _yFitting;
			if (layerID % 2 == 0) {
				yNDF += pow(_yResidual, 2);
			}
			if (layerID % 2 == 1) {
				xNDF += pow(_xResidual, 2);
			}
			if (layerID % 2 == 1) {
				vector<int>::iterator result = find(totalHitLayerX.begin(), totalHitLayerX.end(), layerID);
				if (result == totalHitLayerX.end())
					totalHitLayerX.push_back(layerID);
			} else {
				vector<int>::iterator result = find(totalHitLayerY.begin(), totalHitLayerY.end(), layerID);
				if (result == totalHitLayerY.end())
					totalHitLayerY.push_back(layerID);
			}
		}
		xNDF = (xNDF) / (_hitCellnew.size() - 2);
		yNDF = (yNDF) / (_hitCellnew.size() - 2);
		//if(xNDF>=9.6 || yNDF>=9.6) continue;
		if (totalHitLayerX.size() <= 6 || totalHitLayerY.size() <= 6)
			continue;
		_totalEntries3++;

		// ====================== ADC correction by track fitting ==============================
		const double thickness = 2; // thickness of scintillator strip
		//double xSlope = *(_fitPar);
		//double ySlope = *(_fitPar+4);
		double length = thickness * sqrt(pow(xSlope, 2) + pow(ySlope, 2) + pow(1, 2));
		for (unsigned _chanl = 0; _chanl != _hitCellnew.size(); ++_chanl) {
			double _signal = _timeCor.at(_chanl);
			double _signalCor = _signal * thickness / length;
			_energyCor.push_back(_signalCor);
		}
		_realLength = length;
		if (length > 5)
			cout << " 111111111111 " << entry << " : " << length << endl;

		// ====================== position residual calculation ================================
		vector<vector<double> > _trackPar;
		_trackPar.resize(layerNu);
		double* _trackFit;
		for (int s_layer = 0; s_layer != layerNu; ++s_layer) {
			// cout<<" --------- _selectedLayerID : "<<s_layer<<endl;
			xPosition3D.clear();
			yPosition3D.clear();
			zPosition3D.clear();
			zxPosition3D.clear();
			zyPosition3D.clear();
			for (unsigned _chanl = 0; _chanl != _hitCellnew.size(); ++_chanl) {
				int layerID = _hitCellnew.at(_chanl) / 1e6;
				if (layerID == s_layer)
					continue;
				double _xPosition = _hitPosXnew.at(_chanl);
				double _yPosition = _hitPosYnew.at(_chanl);
				double _zPosition = _hitPosZnew.at(_chanl);
				xPosition3D.push_back(_xPosition);
				yPosition3D.push_back(_yPosition);
				zPosition3D.push_back(_zPosition);
        zyPosition3D.push_back(_zPosition);
				zxPosition3D.push_back(_zPosition);
			}
			if (xPosition3D.size() < 5 || yPosition3D.size() < 5) {
				cout << xPosition3D.size() << " : " << yPosition3D.size() << " The number of real hits too few to fitting" << endl;
				continue;
			}
			_trackFit = line3Dfit(xPosition3D, yPosition3D, zPosition3D);
			for (int ii = 0; ii != 8; ++ii)
				_trackPar[s_layer].push_back(*(_trackFit + ii));
			// cout<<" ====================== position residual calculation ================================"<<endl;
		}

		for (unsigned _chanl = 0; _chanl != _hitCellnew.size(); ++_chanl) {
			int i_layer = _hitCellnew.at(_chanl) / 1e6;
			double _xPosition = _hitPosXnew.at(_chanl);
			double _yPosition = _hitPosYnew.at(_chanl);
			double _zPosition = _hitPosZnew.at(_chanl);
			double xSlope = _trackPar[i_layer].at(0);
			double xIntercept = _trackPar[i_layer].at(2);
			double ySlope = _trackPar[i_layer].at(4);
			double yIntercept = _trackPar[i_layer].at(6);
			double _xFitting = xSlope * _zPosition + xIntercept;
			double _yFitting = ySlope * _zPosition + yIntercept;
			double _xResidual = _xPosition - _xFitting;
			double _yResidual = _yPosition - _yFitting;
			_residualX.push_back(_xResidual);
			_residualY.push_back(_yResidual);
		}

		// detection efficiency analysis
		for (int s_layer = 0; s_layer != layerNu; ++s_layer) {
			int result = 0;

			double xSlope = _trackPar[s_layer].at(0);
			double xIntercept = _trackPar[s_layer].at(2);
			double ySlope = _trackPar[s_layer].at(4);
			double yIntercept = _trackPar[s_layer].at(6);

			double _zPosition = 0;
			if(s_layer%2==0) _zPosition = 1+s_layer/2*19.9;
    	//else layerZ = 12.95+(LayerIDs-1)/2*19.9;
    	else _zPosition = 12.2+(s_layer-1)/2*19.9;

			double _xFitting = xSlope * _zPosition + xIntercept;
			double _yFitting = ySlope * _zPosition + yIntercept;

			for (unsigned _chanl = 0; _chanl != _hitCellnew.size(); ++_chanl) {
				int i_layer = _hitCellnew.at(_chanl) / 1e6;
				if(i_layer != s_layer) continue;

				double _xPosition = _hitPosXnew.at(_chanl);
				double _yPosition = _hitPosYnew.at(_chanl);

				if(_xPosition-2.5<=_xFitting && _xFitting<=_xPosition+2.5 && _yPosition-2.5<=_yFitting && _yFitting<=_yPosition+2.5) result = 1;
			}
			_layer_efficiency.push_back(result);
		}

		_totalEntries4++;
		fTree->Fill();
	}
	cout << _totalEntries1 << " : " << _totalEntries2 << " : " << _totalEntries3 << " : " << _totalEntries4 << endl;

	// *********************************************************************************
	// *********************************************************************************

	fitfile->cd();
	fTree->Write("", TObject::kOverwrite);
  fitfile->Close();

  string trackgraphname(track_name);
	trackgraphname.insert(trackgraphname.length(), "graph/");
	trackgraphname.insert(trackgraphname.length(), rootname);
  TFile *graphfile = new TFile(trackgraphname.c_str(),"RECREATE");
  graphfile->mkdir("graph");
  graphfile->cd("graph");
  Glist1->Write();
  graphfile->Close();

}
