 //--------------------------------------------------------------------------
 // File and Version Information:
 //    $Id: EmcCalibMinuit.cc,v 1.16 2012/01/24 12:27:29 bernhard Exp $
 //
 // Description:
 //    Class EmcCalibMinuit
 //------------------------------------------------------------------------
 
 //-----------------------
 // This Class's Header --
 //-----------------------
 //#include "EmcCalibStudies/EmcCalibration.hh"
 #include "EmcCalibration.hh"
 //---------------
 // C++ Headers --
 //---------------
 #include <iostream>
 #include <math.h>
 #include <string>
 #include <sstream>
 #include <map>
 #include <vector>
 
 #include "TMinuit.h"
 #include "TFitter.h"
 #include "EmcCalibMinuit.hh"
 #include "EmcCalibrationLh.hh"
 
 #include <boost/shared_ptr.hpp>
 
 using namespace std;
 
 static TVirtualFitter *theFitter = 0;
 
 //-----------------------------------------------------------------------
 // Local Macros, Typedefs, Structures, Unions and Forward Declarations --
 //-----------------------------------------------------------------------
 
 //----------------
 // Constructors --
 //----------------
 
 EmcCalibMinuit::EmcCalibMinuit(const char* options) :
             EmcCalibration(options) {
       init();
       theCalibIO->logger(3, "EmcCalibMinuit: EmcCalibration with Minuit initialized");
 }
 
 EmcCalibMinuit::EmcCalibMinuit() :
             EmcCalibration() {
       init();
 }
 
 //--------------
 // Destructor --
 //--------------
 
 EmcCalibMinuit::~EmcCalibMinuit() {
       if (0 != theFitter)
             delete theFitter;
       if (0 != _emcCalibLhPtr)
             delete _emcCalibLhPtr;
 }
 
 void EmcCalibMinuit::init() {
       _maxFCN = -1e30;
 }
 
 void EmcCalibMinuit::printStatus() { //if "p" is set in Constructor
       stringstream sst;
       sst.precision(4);
       sst << "EmcCalibMinuit::printStatus()" << std::endl;
       sst << "calibration value:" << std::endl;
       sst << "_mPi0     |_xmin     |_xmax" << std::endl;
       sst << _toCalib.value << "    |" << _toCalib.min << "    |" << _toCalib.max << std::endl;
 
       sst << "calibration parameters:" << std::endl;
       sst << "_maxIterations|_suppression" << std::endl;
       sst << _maxIterations << "          |" << _suppression << std::endl;
 
       vector<parameter>::iterator it_par;
       sst << "Fit Parameters (#" << _par.size() << "):" << std::endl;
       sst << "name    |    value    |    min    |    max    " << std::endl;
       for (it_par = _par.begin(); it_par != _par.end(); it_par++) {
             sst << (*it_par).name << "  |" << (*it_par).value << "  |" << (*it_par).min << "  |"
                         << (*it_par).max << std::endl;
       }
       theCalibIO->logger(4, sst);
 }
 
 std::vector<boost::shared_ptr<parameter> > EmcCalibMinuit::param() {
       std::vector<boost::shared_ptr<parameter> > theParPtr;
       parameter thePar;
       for (int i = 0; i < _par.size(); i++) {
             boost::shared_ptr<parameter> thePtr(new parameter(_par[i]));
             theParPtr.push_back(thePtr);
       }
       return theParPtr;
 }
 
 //boost::shared_ptr<std::vector<entry> > EmcCalibMinuit::fitVecPtr() {
 //          boost::shared_ptr<std::vector<entry> > theFitPtr(new std::vector<entry>((*fit_vec)));
 //          return theFitPtr;
 //    }
 
 data EmcCalibMinuit::fit(std::vector<entry> &fit_vec) {
       if (_printStatus) {
             printStatus();
             _printStatus = kFALSE;
       }
 
       // Initialize MINUIT
       Int_t const nPar = _par.size();
 
       _emcCalibLhPtr = new EmcCalibrationLh(boost::shared_ptr<parameter>(new parameter(_toCalib)),
                   parPtr(), fitVecPtr(fit_vec));
 
       if (theFitter)
             delete theFitter;
       theFitter = new TFitter(nPar * 2); //WVE Kludge, nPar*2 works around TMinuit memory allocation bug
       theFitter->SetObjectFit(this);
       Double_t arglist[2];
 
       // Shut up for now
       arglist[0] = (Double_t) _printLevel;
       theFitter->ExecuteCommand("SET PRINT", arglist, 1);
       theFitter->Clear();
 
       // Tell MINUIT to use our global glue function
       theFitter->SetFCN(fcn_model);
 
       // Use +0.5 for 1-sigma errors
       arglist[0] = 0.5;
       theFitter->ExecuteCommand("SET ERR", arglist, 1);
 
       // Declare our parameters to MINUIT
       vector<parameter>::iterator it_par;
       Double_t pstep = 0.;
       int index = 0;
       for (it_par = _par.begin(); it_par != _par.end(); it_par++) {
             pstep = ((*it_par).max - (*it_par).min) * 0.1;
             if (pstep < 1e-7)
                   pstep = 1.;
             theFitter->SetParameter(index, (TString) (*it_par).name, (*it_par).value, pstep,
                         (*it_par).min, (*it_par).max);
             index++;
       }
 
       arglist[0] = 0.;
       theFitter->ExecuteCommand("SET STR", arglist, 1);
 
       arglist[0] = 500. * (Double_t) nPar; // maximum iterations
       arglist[1] = 1.0; // tolerance
       theFitter->ExecuteCommand("MIGRAD", arglist, 2);
 
       Double_t val, err, vlo, vhi;
       char buffer[10240];
       theFitter->GetParameter(0, buffer, val, err, vlo, vhi);
 
       data ergebnis(val, err);
 
       return ergebnis;
 }
 
 //defines funktion based on novosibisk + chebychev
 void fcn_model(Int_t &cpnr, Double_t *gin, Double_t &f, Double_t* par, Int_t iflag) {
       EmcCalibMinuit* context = (EmcCalibMinuit*) theFitter->GetObjectFit();
       if (0 == context) {
             f = 0.;
             return;
       }
       std::vector<double> theParVec;
       for (int i = 0; i < context->par_size(); i++) {
             theParVec.push_back(par[i]);
       }
       f = (context->_emcCalibLhPtr->calcLh(theParVec) / 2.);
       return;
 }
