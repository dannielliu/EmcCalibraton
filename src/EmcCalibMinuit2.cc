 //--------------------------------------------------------------------------
 // File and Version Information:
 //    $Id: EmcCalibMinuit2.cc,v 1.12 2012/01/24 12:27:29 bernhard Exp $
 //
 // Description:
 //    Class EmcCalibMinuit2
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
 #include <sstream>
 #include <math.h>
 #include <string>
 #include <map>
 #include <vector>
 
 #include "Minuit2/MnUserParameters.h"
 #include "Minuit2/MnMigrad.h"
 #include "Minuit2/FunctionMinimum.h"
 #include "Minuit2/MnMinos.h"
 #include "Minuit2/MnStrategy.h"
 #include "EmcCalibMinuit2.hh"
 
 #include "Minuit2/MnContours.h"
 #include "Minuit2/MnPlot.h"
 
 #include "EmcCalibrationLh.hh"
 #include "EmcCalibMinuit2Fcn.hh"
 
 using namespace std;
 
 //-----------------------------------------------------------------------
 // Local Macros, Typedefs, Structures, Unions and Forward Declarations --
 //-----------------------------------------------------------------------
 
 //----------------
 // Constructors --
 //----------------
 
 EmcCalibMinuit2::EmcCalibMinuit2(const char* options) :
             EmcCalibration(options) {
       init();
 
 }
 
 EmcCalibMinuit2::EmcCalibMinuit2() :
             EmcCalibration() {
       init();
 }
 
 //--------------
 // Destructor --
 //--------------
 
 EmcCalibMinuit2::~EmcCalibMinuit2() {
       return;
 }
 
 void EmcCalibMinuit2::init() {
       _maxFCN = -1e30;
       theCalibIO->logger(3,"EmcCalibMinuit2: EmcCalibration with Minuit2 initialized");
 }
 
 void EmcCalibMinuit2::printStatus() { //if "p" is set in Constructor
       if (!_printStatus)
             return;
       _printStatus = false;
       stringstream sst;
       sst.precision(4);
       cout.precision(4);
       sst << "EmcCalibMinuit::printStatus()" << std::endl;
       sst << "calibration value:" << std::endl;
       sst << "mPi0   |xmin     |xmax" << std::endl;
       sst << _toCalib.value << " |" << _toCalib.min << "    |" << _toCalib.max << std::endl;
 
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
       theCalibIO->logger(4,sst);
 
 }
 
 std::vector<boost::shared_ptr<parameter> > EmcCalibMinuit2::param() {
       std::vector<boost::shared_ptr<parameter> > theParPtr;
       parameter thePar;
       for (int i = 0; i < _par.size(); i++) {
             boost::shared_ptr<parameter> thePtr(new parameter(_par[i]));
             theParPtr.push_back(thePtr);
       }
       return theParPtr;
 }
 
 data EmcCalibMinuit2::fit(std::vector<entry> &fit_vec) {
       printStatus();
 
       boost::shared_ptr<EmcCalibrationLh> theEmcCalibrationLhPtr(
                   new EmcCalibrationLh(boost::shared_ptr<parameter>(new parameter(_toCalib)), parPtr(),
                               fitVecPtr(fit_vec)));
 
       EmcCalibMinuit2Fcn theFCN(theEmcCalibrationLhPtr);
 
       MnUserParameters upar;
       theFCN.initFitParameters(upar);
 
       std::vector<double> para;
       std::vector<double> para_err;
       for (int i = 0; i < _par.size(); i++) {
             para.push_back(_par[i].value);
             para_err.push_back(0.5);
       }
 
       MnMigrad migrad(theFCN, upar);
 //    VariableMetricMinimizer theMinimizer;
       // minimize
 //    FunctionMinimum min =theMinimizer.Minimize(theFCN,para,para_err);
 
       theCalibIO->logger(3,"start migrad");
 
       FunctionMinimum min = migrad();
 
       if (!min.IsValid()) {
             theCalibIO->logger(2,"FM is invalid, try with strategy = 2.");
             MnMigrad migrad2(theFCN, min.UserState(), MnStrategy(2));
             min = migrad2();
       }
 
       double val = min.UserState().Value(_par[0].name);
       double err = min.UserState().Error(_par[0].name);
       data ergebnis(val, err);
 
       return ergebnis;
 }
