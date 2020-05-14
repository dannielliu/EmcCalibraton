#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TVector3.h"
#include "EmcCalibDataModel.hh"
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
 #include <boost/archive/xml_iarchive.hpp>
 #include <boost/archive/xml_oarchive.hpp>
#include <iostream>
#include <fstream>
using namespace std;

 int liloWhere(TVector3 &where, std::vector<hit> bump);

//int checkCalibration()
int main()
{
  entry* pi0 = new entry;
  TFile* f1 = new TFile("preparedSample.root");
  if (f1==0) return -1;
  TTree* tpi0 = (TTree*)f1->Get("candidates");
  if (tpi0==0) return -2;
  tpi0->SetBranchAddress("entry", &pi0);
  
//TFile* f12 = new TFile("preparedSample2.root");
//if (f12==0) return -1;
//TTree* tpi012 = (TTree*)f12->Get("candidates");
//if (tpi012==0) return -2;
//tpi012->SetBranchAddress("entry", &pi0);


  TFile* f2 = new TFile("calibratedSample.root");
  if (f2==0) return -1;
  TTree* tpi02 = (TTree*)f2->Get("candidates");
  if (tpi02==0) return -2;
  tpi02->SetBranchAddress("entry", &pi0);

 
  std::map<const int, data> calib_map_data;
  //       read class state from archive;
  const char* _EmcCalibrationFile = "./EmcCalibStudies/EmcCalibrationData3.dat";
  std::ifstream ifs(_EmcCalibrationFile);
  if (ifs.fail()) {
    cout<<"cannot open file "<<_EmcCalibrationFile<<endl;
	  return -1;
  }
    cout<<"test1 "<<_EmcCalibrationFile<<endl;
  boost::archive::xml_iarchive in_a(ifs);
    cout<<"test2 "<<_EmcCalibrationFile<<endl;
  in_a >> BOOST_SERIALIZATION_NVP(calib_map_data);
    cout<<"test3 "<<_EmcCalibrationFile<<endl;


  TH1D* hm_raw = new TH1D("hm_raw","m(#pi^{0}) raw",100, 0.05, 0.2);
  TH1D* hm_crw = new TH1D("hm_crw","m(#pi^{0}) correct hits",100, 0.05, 0.2);
  TH1D* hm_cor = new TH1D("hm_cor","m(#pi^{0}) corrected entries",100, 0.05, 0.2);

  int nraw = tpi0->GetEntries();
  for (int ie=0; ie<nraw; ie++) {
    if (ie%100000==0) cout<<"analysis event "<<ie<<"/"<<nraw<<endl;
    tpi0->GetEntry(ie);
    hm_raw->Fill(pi0->m_gg);
  }
  // 
  for (int ie=0; ie<nraw; ie++) {
    if (ie%100000==0) cout<<"analysis event "<<ie<<"/"<<nraw<<" for all hit"<<endl;
    tpi0->GetEntry(ie);
    double e1=0; double e2=0;
    //
    int n1 = pi0->this_bump.size();
    vector<hit> thisbump = pi0->this_bump;
    for (int ih=0; ih<n1; ih++) {
      double fcor = 1.0;
      int cpnr = thisbump.at(ih).cpnr;
      if (calib_map_data.find(cpnr)!=calib_map_data.end()) fcor = calib_map_data[cpnr].value;
      thisbump.at(ih).E_dep *= fcor;
      e1 += thisbump.at(ih).E_dep;
      //e1 += pi0->this_bump.at(ih).E_dep*fcor;
    }
    //
    int n2 = pi0->associated_bump.size();
    vector<hit> associatedbump = pi0->associated_bump;
    for (int ih=0; ih<n2; ih++) {
      double fcor = 1.0;
      int cpnr = associatedbump.at(ih).cpnr;
      if (calib_map_data.find(cpnr)!=calib_map_data.end()) fcor = calib_map_data[cpnr].value;
      //e2 += pi0->associated_bump.at(ih).E_dep*fcor;
      associatedbump.at(ih).E_dep *= fcor;
      e2 += associatedbump.at(ih).E_dep;
    }
    TVector3 where1, where2;
    liloWhere(where1, thisbump);
    liloWhere(where2, associatedbump);
    double angle = where1.Angle(where2);
    double mpi0 = sqrt(2.0*e1*e2*(1-cos(angle)));
    hm_crw->Fill(mpi0);
  }
  //
  int ncor = tpi02->GetEntries();
  for (int ie=0; ie<ncor; ie++) {
    if (ie%100000==0) cout<<"analysis event "<<ie<<"/"<<ncor<<endl;
    tpi02->GetEntry(ie);
    hm_cor->Fill(pi0->m_gg);
  }

  TCanvas* c1 = new TCanvas();
  hm_raw->SetLineColor(1);
  hm_crw->SetLineColor(2);
  hm_cor->SetLineColor(3);
  hm_raw->GetXaxis()->SetTitle("m(#gamma#gamma) (GeV/c^{2})");
  hm_raw->GetYaxis()->SetTitle("Events");
  hm_raw->Draw();
  hm_crw->Draw("same");
  hm_cor->Draw("same");
  c1->BuildLegend(0.15, 0.7, 0.5, 0.85);
  c1->Print("mpi0_rawVscor.pdf");
  
  return 0;
}

 
 int liloWhere(TVector3 &where, std::vector<hit> bump){
       Double_t params[3];
       params[0] = 4.071;
       params[1] = 0.678;
       params[2] = 1.0;

	Double_t offsetParmA=params[0];
	Double_t offsetParmB=params[1];
	Double_t offsetParmC=params[2];

	//const double lClusEnergy=Energy();
	
	//assert(lClusEnergy!=0);
	double lClusEnergy = 0.0;
	std::vector<hit>::const_iterator current;
	int nhit = bump.size();
	for (current=bump.begin();current!=bump.end();++current)
	{
	       hit tmphit = *current; 
	       lClusEnergy += tmphit.E_dep;
	}
	const double lOffset=
		offsetParmA-offsetParmB*exp(-offsetParmC*pow(lClusEnergy,1.171)) * pow(lClusEnergy,-0.534);
	
	TVector3 lLiloPoint( 1, 1, 1 );
	
	TVector3 lLinSum( 0, 0, 0 );
	TVector3 lLogSum( 0, 0, 0 );
	
	double lLogWeightSum=0;
	int lLogNum=0;
	
	bool lLogSecondTheta  = false;
	bool lLogSecondPhi    = false;
	int  lLogFirstTheta   = -666;
	int  lLogFirstPhi     = -666;
	
	//std::vector<Int_t>::const_iterator current;
	//std::vector<hit>::const_iterator current;
	//int nhit = bump.size();
	for (current=bump.begin();current!=bump.end();++current)
	{
	        hit tmphit = *current;
		//const TVector3 lDigiWhere=lDigi->where();
		const TVector3 lDigiWhere(tmphit.X, tmphit.Y, tmphit.Z);
		TVector3 pos(tmphit.X, tmphit.Y, tmphit.Z);
		const int lDigiTheta = (int)(pos.Theta()/3.14*360);
		const int lDigiPhi   = (int)(pos.Phi()/3.14*360);
		//const int lDigiTheta=lDigi->GetThetaInt();
		//const int lDigiPhi=lDigi->GetPhiInt();
		const double lDigiEnergy=tmphit.E_dep;
		const double lLinWeight=lDigiEnergy/lClusEnergy;
		const double lLogWeight=lOffset+log(lLinWeight);
		
		lLinSum+=lLinWeight*lDigiWhere;
		if(lLogWeight>0)
		{
			lLogSum+=lLogWeight*lDigiWhere;
			lLogWeightSum+=lLogWeight;
			lLogNum++;
			
			if(lLogNum==1)
			{ 
					lLogFirstTheta=lDigiTheta;
					lLogFirstPhi=lDigiPhi;
			}
			else
			{
				if(!lLogSecondTheta&&lDigiTheta!=lLogFirstTheta) 
					lLogSecondTheta=true;
				if(!lLogSecondPhi&&lDigiPhi!=lLogFirstPhi) 
					lLogSecondPhi=true;
			}
		}
	}
  
	if(lLogNum>0) lLogSum*=1./lLogWeightSum;
	
	
	lLiloPoint.SetTheta(lLogSecondTheta?lLogSum.Theta():lLinSum.Theta());
	lLiloPoint.SetPhi(lLogSecondPhi?lLogSum.Phi():lLinSum.Phi());
	
	
	// First, find out if the point is outside the crystal.
	const TVector3 lLiloVector = lLiloPoint;
	if (lLogNum > 1) {
		// Use logarithmic centroid position
		lLiloPoint.SetMag(lLogSum.Mag());
	} else {
		// Use linear centroid position
		lLiloPoint.SetMag(lLinSum.Mag());
	}
	
        where = lLiloPoint;

	return 1;

 }


