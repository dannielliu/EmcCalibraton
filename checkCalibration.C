#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"
#include "EmcCalibDataModel.hh"

//int checkCalibration()
int main()
{
  entry* pi0 = new entry;
  TFile* f1 = new TFile("preparedSample.root");
  if (f1==0) return -1;
  TTree* tpi0 = (TTree*)f1->Get("candidates");
  if (tpi0==0) return -2;
  tpi0->SetBranchAddress("entry", &pi0);

  TFile* f2 = new TFile("calibratedSample.root");
  if (f2==0) return -1;
  TTree* tpi02 = (TTree*)f2->Get("candidates");
  if (tpi02==0) return -2;
  tpi02->SetBranchAddress("entry", &pi0);

  TH1D* hm_raw = new TH1D("hm_raw","m(#pi^{0})",100, 0.05, 0.2);
  TH1D* hm_cor = new TH1D("hm_cor","m(#pi^{0})",100, 0.05, 0.2);
  
  int nraw = tpi0->GetEntries();
  for (int ie=0; ie<nraw; ie++) {
    tpi0->GetEntry(ie);
    hm_raw->Fill(pi0->m_gg);
  }
  int ncor = tpi02->GetEntries();
  for (int ie=0; ie<ncor; ie++) {
    tpi02->GetEntry(ie);
    hm_cor->Fill(pi0->m_gg);
  }

  TCanvas* c1 = new TCanvas();
  hm_raw->SetLineColor(1);
  hm_cor->SetLineColor(2);
  hm_raw->Draw();
  hm_cor->Draw("same");
  c1->Print("mpi0_rawVscor.pdf");
  
  return 0;
}
