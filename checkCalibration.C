#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"
#include "EmcCalibDataModel.hh"
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
 #include <boost/archive/xml_iarchive.hpp>
 #include <boost/archive/xml_oarchive.hpp>
#include <iostream>
#include <fstream>
using namespace std;

//int checkCalibration()
int main()
{
  entry* pi0 = new entry;
  TFile* f1 = new TFile("preparedSample.root");
  if (f1==0) return -1;
  TTree* tpi0 = (TTree*)f1->Get("candidates");
  if (tpi0==0) return -2;
  tpi0->SetBranchAddress("entry", &pi0);
  
  TFile* f12 = new TFile("preparedSample2.root");
  if (f12==0) return -1;
  TTree* tpi012 = (TTree*)f12->Get("candidates");
  if (tpi012==0) return -2;
  tpi012->SetBranchAddress("entry", &pi0);


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
  boost::archive::xml_iarchive in_a(ifs);
  in_a >> BOOST_SERIALIZATION_NVP(calib_map_data);


  TH1D* hm_raw = new TH1D("hm_raw","m(#pi^{0}) raw",100, 0.05, 0.2);
  TH1D* hm_crw = new TH1D("hm_crw","m(#pi^{0}) correct hits",100, 0.05, 0.2);
  TH1D* hm_cor = new TH1D("hm_cor","m(#pi^{0}) correct seeds",100, 0.05, 0.2);
  
  int nraw = tpi0->GetEntries();
  for (int ie=0; ie<nraw; ie++) {
    if (ie%100000==0) cout<<"analysis event "<<ie<<"/"<<nraw<<endl;
    tpi0->GetEntry(ie);
    hm_raw->Fill(pi0->m_gg);
  }
  int nraw12 = tpi012->GetEntries();
  for (int ie=0; ie<nraw12; ie++) {
    if (ie%100000==0) cout<<"analysis event "<<ie<<"/"<<nraw12<<" for all hit"<<endl;
    tpi012->GetEntry(ie);
    double e1=0; double e2=0;
    int n1 = pi0->this_bump.size();
    for (int ih=0; ih<n1; ih++) {
      double fcor = 1.0;
      int cpnr = pi0->this_bump.at(ih).cpnr;
      if (calib_map_data.find(cpnr)!=calib_map_data.end()) fcor = calib_map_data[cpnr].value;
      e1 += pi0->this_bump.at(ih).E_dep*fcor;
    }
    int n2 = pi0->associated_bump.size();
    for (int ih=0; ih<n2; ih++) {
      double fcor = 1.0;
      int cpnr = pi0->associated_bump.at(ih).cpnr;
      if (calib_map_data.find(cpnr)!=calib_map_data.end()) fcor = calib_map_data[cpnr].value;
      e2 += pi0->associated_bump.at(ih).E_dep*fcor;
    }
    double mpi0 = sqrt(2.0*e1*e2*(1-cos(pi0->angle)));
    hm_crw->Fill(mpi0);
  }
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
