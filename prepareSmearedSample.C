/* here is the pi0 selction algorithm, 
 * and save the sample in the format used in analysis
 */

#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"
#include "TClonesArray.h"
#include "PndEmcDigi.h"
#include "PndEmcCluster.h"
#include "PndEmcMapper.h"
#include "PndEmcStructure.h"
//#include "EmcCalibContainer.hh"
#include "EmcCalibDataModel.hh"
#include <iostream>
#include <fstream>
#include <vector>
 #include <boost/serialization/vector.hpp>
 #include <boost/serialization/map.hpp>
 
 #include <boost/archive/text_oarchive.hpp>
 #include <boost/archive/text_iarchive.hpp>
 
 #include <boost/archive/xml_iarchive.hpp>
 #include <boost/archive/xml_oarchive.hpp>
 
using namespace std;
//#ifndef __CINT__
//ClassImp(entry);
//#endif

 int liloWhere(TVector3 &where, std::vector<hit> bump);

//int prepareData()
int main()
{
  cout<<"Start"<<endl;
  double Eclscut = 0.1;

  string dir="../From0ToBarrel_test/From0To_barrel_all_1";
  
  // read data
  char line[1000];
  sprintf(line, "%s/evtcomplete_digi_nonuniform.root", dir.c_str());
  TFile *inputFile = new TFile(line);
  TTree* inputTree = (TTree*)inputFile->Get("pndsim");
  if (!inputTree) {
	  cout<<"Error: cannot find Input TTree"<<endl;
	  return -1;
  }
  TClonesArray* fClusterArray = new TClonesArray("PndEmcCluster");
  inputTree->SetBranchAddress("EmcCluster", &fClusterArray);
  TClonesArray* fDigiArray = new TClonesArray("PndEmcDigi");
  inputTree->SetBranchAddress("EmcDigi", &fDigiArray);

  cout<<"prepare EMC structure ..."<<endl;
  // map of emc structure
  char namestr[1000];
  sprintf(namestr,"%s/evtcomplete_par.root", dir.c_str());
  TFile* inputfile3 = new TFile(namestr);
  inputfile3->Get("FairGeoParSet");
  PndEmcMapper::Init(1);
  PndEmcMapper* fMapper = PndEmcMapper::Instance();
  PndEmcStructure* fEmcStr=PndEmcStructure::Instance();
  const PndEmcTciXtalMap &XtalMap = fEmcStr->GetTciXtalMap();
  PndEmcTwoCoordIndex *tmpTCI; //= fMapper->GetTCI(crystalid);
  PndEmcXtal *tmpXtal; // = XtalMap.find(tmpTCI)->second;
  
  // save xtal_def
  ofstream xtaldef("xtal_def.dat");
  std::map<const int, xtal_def> _xtalDef;
  PndEmcTciXtalMap::const_iterator it = XtalMap.begin();
  for (; it!=XtalMap.end(); it++){
    int xtalid = it->first->Index();
    int xcoord = it->first->XCoord();
    //int ycoord = it->first->YCoord();
    int subd = xtalid/100000000;
    int subdetector=0;
    if (subd==1||subd==2) subdetector = 2;
    if (subd==3) subdetector = 3;
    if (subd==4) subdetector = 1;
    if (subd==5) subdetector = 5;
    int edge = 0;
    if (subd==1 || subd==2) if (xcoord==1 || xcoord==71) edge = 1;
    //int nneighbor = it->first->GetNeighbours().size();
    //if (nneighbor<7) edge = 1;
    xtaldef << xtalid << "\t" << edge << "\t" << subd <<endl;
    xtal_def xdef;
    xdef.subdetector = subdetector;
    xdef.isAtEdge = edge;
    _xtalDef[xtalid] = xdef;
  }
  ofstream ofs("./EmcCalibStudies/edgeXtals.dat");
  //std::ofstream ofs(_EmcCalibrationFile);
  assert(ofs.good());
  boost::archive::xml_oarchive out_a(ofs);
  out_a << boost::serialization::make_nvp("xtalmap", _xtalDef);

  //return 0;
  std::map<const int, data> calib_map_data;
  std::map<const int, xtal_def>::iterator xtalit;
  for (xtalit = _xtalDef.begin(); xtalit!=_xtalDef.end(); xtalit++) {
    int cpnr = xtalit->first;
    int subd = xtalit->second.subdetector;
    data dat(1.0, 1.0);
    if (subd==2) dat.value = gRandom->Uniform(0.9, 1.1);
    calib_map_data[cpnr] = dat; 
  }
  ofstream ofss("./EmcCalibStudies/InputCalibMap.dat");
  //std::ofstream ofs(_EmcCalibrationFile);
  assert(ofss.good());
  boost::archive::xml_oarchive out_b(ofss);
  out_b << boost::serialization::make_nvp("CalibMap", calib_map_data);




  cout<<"create root files and tree ."<<endl;
  entry* tmppi0 = new entry;
//vector<hit>* hits = new vector<hit>;
  hit tmphit;
  int cpnr;
  cout<<"create root files and tree .."<<endl;
  // output data
  TFile* fout = new TFile("prepareSmearedSample.root","recreate");
  //cout<<"create root files and tree ..."<<endl;
  TTree* tpi0 = new TTree("candidates","pi0Candidates");
  //cout<<"create root files and tree ...."<<endl;
  tpi0->Branch("cpnr", &cpnr, 64000, 1);
  tpi0->Branch("entry", &tmppi0, 64000, 1);
  //tpi0->Branch("entry", "entry", &tmppi0, 64000, 1);
//TTree* thit = new TTree("hits","hits");
//thit->SetBranch("photons",hits);

  // loop event
  // loop clusters
  cout<<"loop events ..."<<endl;
  // loop events
  int nevents = inputTree->GetEntriesFast();
  // first loop, construct pion list for each event
  
  for (int ievt=0; ievt<nevents; ievt++) {
	  tmppi0->clear();
	  inputTree->GetEntry(ievt);
          
	  if (ievt%100000==0) cout<<ievt<<"/"<<nevents<<endl;
	  // loop clusters
	  int nclss = fClusterArray->GetEntriesFast();
	  //cout<<"cls size is "<<nclss<<endl;
	  if (nclss<2) continue;
	  vector<int> clsids;
	  for (int icls=0; icls<nclss; icls++) {
		  PndEmcCluster* cluster = (PndEmcCluster*)fClusterArray->At(icls);
		  //PndEmcDigi* maxDigi = cluster->Maxima(fDigiArray);
		  double Ecls = cluster->energy();
		  if (Ecls < Eclscut) continue;
		  clsids.push_back(icls);
	  }
	  int clssize = clsids.size();
	  if (clssize<2) continue;

	  // construct pion list
	  // firstly, check the distribution of m(gg)
	  //vector<PionCandidate> pionList;
	  for (int i=0; i<clssize-1; i++) {
		  for (int j=i+1; j<clssize; j++) {
			  int icls = clsids[i];
			  int jcls = clsids[j];
			  PndEmcCluster* clsi = (PndEmcCluster*)fClusterArray->At(icls);
			  PndEmcCluster* clsj = (PndEmcCluster*)fClusterArray->At(jcls);
			  double Ei = 0; //clsi->energy();
			  double Ej = 0; //clsj->energy();
			  //TVector3 v3i = clsi->where();
			  //TVector3 v3j = clsj->where();
			  //double angle = v3i.Angle(v3j);
			  //double invmass = sqrt(2*Ei*Ej*(1-cos(angle)));

			  //h1->Fill(invmass);
			  // fill the map of histogram
			  PndEmcDigi* maxDigi1 = clsi->Maxima(fDigiArray);
			  int id1 = maxDigi1->GetDetectorId();
			  int mod1 = maxDigi1->GetModule();
			  PndEmcDigi* maxDigi2 = clsj->Maxima(fDigiArray);
			  int id2 = maxDigi2->GetDetectorId();

			  cpnr = id1;
			  //tmppi0->m_gg = invmass;
			  //tmppi0->cpnr2 = id2;
			  //tmppi0->angle = angle;
			  //tmppi0->fraction = maxDigi1->GetEnergy()/Ei;
			  //tmppi0->fraction2 = maxDigi2->GetEnergy()/Ej;
			  for (int idigi=0; idigi<clsi->DigiList().size(); idigi++) {
			    int id = clsi->DigiList().at(idigi);
	 		    PndEmcDigi* digi =  (PndEmcDigi*)fDigiArray->At(id);
			    tmphit.cpnr = digi->GetDetectorId();
			    tmphit.E_dep = digi->GetEnergy()*calib_map_data[tmphit.cpnr].value;
			    tmphit.X = digi->where().X();
			    tmphit.Y = digi->where().Y();
			    tmphit.Z = digi->where().Z();
			    Ei += tmphit.E_dep;
			    tmppi0->this_bump.push_back(tmphit);
			    //tmppi0.this_bump.push_back(idigi);
			  }
			  for (int idigi2=0; idigi2<clsj->DigiList().size(); idigi2++) {
			    int id = clsj->DigiList().at(idigi2);
	 		    PndEmcDigi* digi = (PndEmcDigi*) fDigiArray->At(id);
			    tmphit.cpnr = digi->GetDetectorId();
			    tmphit.E_dep = digi->GetEnergy()*calib_map_data[tmphit.cpnr].value;
			    tmphit.X = digi->where().X();
			    tmphit.Y = digi->where().Y();
			    tmphit.Z = digi->where().Z();
			    Ej += tmphit.E_dep;
			    tmppi0->associated_bump.push_back(tmphit);
	
			    //tmppi0.associated_bump.push_back(idigi2);
			  }
			  TVector3 where1, where2;
			  liloWhere(where1, tmppi0->this_bump);
			  liloWhere(where2, tmppi0->associated_bump);
			  double angle = where1.Angle(where2);
			  double mpi0 = sqrt(2.0*Ei*Ej*(1-cos(angle)));
			  
			  tmppi0->m_gg = mpi0;
			  tmppi0->cpnr2 = id2;
			  tmppi0->angle = angle;
			  tmppi0->fraction  = maxDigi1->GetEnergy()*calib_map_data[id1].value/Ei;
			  tmppi0->fraction2 = maxDigi2->GetEnergy()*calib_map_data[id2].value/Ej;
			  tpi0->Fill();

			  }
	  }
	  // create pion list for this event
	  //mapEvtPions[ievt] = pionList;

	  // save hits for each event
	//int ndigi = fDigiArray->GetEntriesFast();
	//hits.clear();
	//for (int idigi=0; idigi<ndigi; idigi++) {
	//   PndEmcDigi* digi = (PndEmcDigi*) fDigiArray->At(idigi);
	//   tmphit.cpnr = digi->GetDetectorId();
	//   tmphit.E_dep = digi->GetEnergy();
	//   tmphit.X = digi->where().X();
	//   tmphit.Y = digi->where().Y();
	//   tmphit.Z = digi->where().Z();
	//   hits.push_back(tmphit);
	//}
	//thit->Fill();
  }



  return 0;
}

 int liloWhere(TVector3 &where, std::vector<hit> bump){
       double params[3] = {4.071, 0.678, 1.0};


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
