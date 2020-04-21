 //----------------------------------------------------------------------------------------------------------------------------------------------------//
 //   EmcCalibFcn
 //   Ecn Functionas implementation of FCNBase
 //----------------------------------------------------------------------------------------------------------------------------------------------------//
 
 //-----------------------
 // This Class's Header --
 //-----------------------
 #include "EmcCalibMinuit2Fcn.hh"
 //---------------
 // C++ Headers --
 //---------------
 #include <iostream>
 #include <math.h>
 #include <string>
 #include <sstream>
 #include <map>
 #include <vector>
 #include <cassert>
 
 #include "Minuit2/MnUserParameters.h"
 #include "Minuit2/MnMigrad.h"
 #include "Minuit2/FunctionMinimum.h"
 #include "Minuit2/MnMinos.h"
 #include "Minuit2/MnStrategy.h"
 #include "EmcCalibrationLh.hh"
 
 using namespace std;
 
 EmcCalibMinuit2Fcn::EmcCalibMinuit2Fcn(boost::shared_ptr<EmcCalibrationLh> theEmcCalibrationLh) :
             _emcCalibrationLh(theEmcCalibrationLh) {
 
       if (0 == _emcCalibrationLh)
             cout << "EmcCalibrationLh pointer is 0 !!!!" << endl;
 }
 
 EmcCalibMinuit2Fcn::~EmcCalibMinuit2Fcn() {
 }
 
 void EmcCalibMinuit2Fcn::initFitParameters(MnUserParameters& upar) {
       //    vector<parameter>::iterator it_par;
       std::vector<boost::shared_ptr<parameter> >::const_iterator it_par;
       std::vector<boost::shared_ptr<parameter> > theParVec = _emcCalibrationLh->param();
       double pstep = 0.;
       for (it_par = theParVec.begin(); it_par != theParVec.end(); it_par++) {
             pstep = (double) ((*it_par)->max - (*it_par)->min) * 0.1;
             if (pstep < 1e-7)
                   pstep = 1.;
             upar.Add((*it_par)->name, (double) (*it_par)->value, pstep, (double) (*it_par)->min,
                         (double) (*it_par)->max);
       }
       return;
 }
 
 double EmcCalibMinuit2Fcn::operator()(const std::vector<double>& par) const {
 
       double result = 0.;
 
       result = _emcCalibrationLh->calcLh(par);
 
       return result;
 }
 
 double EmcCalibMinuit2Fcn::Up() const {
       return .5;
 }
 
