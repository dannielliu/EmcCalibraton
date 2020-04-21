 //--------------------------------------------------------------------------
 // File and Version Information:
 //    $Id: EmcCalibrationLh.cc,v 1.5 2012/01/24 12:27:29 bernhard Exp $
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
 #include <math.h>
 #include <string>
 #include <map>
 #include <vector>
 #include <float.h>
 
 #include "Minuit2/MnUserParameters.h"
 #include "Minuit2/MnMigrad.h"
 #include "Minuit2/FunctionMinimum.h"
 #include "Minuit2/MnMinos.h"
 #include "Minuit2/MnStrategy.h"
 #include "EmcCalibrationLh.hh"
 
 using namespace std;
 
 //-----------------------------------------------------------------------
 // Local Macros, Typedefs, Structures, Unions and Forward Declarations --
 //-----------------------------------------------------------------------
 
 //----------------
 // Constructors --
 //----------------
 
 EmcCalibrationLh::EmcCalibrationLh() {
       init();
 }
 
 EmcCalibrationLh::EmcCalibrationLh(boost::shared_ptr<EmcCalibrationLh> emcCalibrationLhPtr) :
             _toCalib(emcCalibrationLhPtr->_toCalib), _par(emcCalibrationLhPtr->_par), fit_vec(
                         emcCalibrationLhPtr->fit_vec) {
       init();
 }
 
 EmcCalibrationLh::EmcCalibrationLh(
             boost::shared_ptr<parameter> thetoCalib,
             std::vector<boost::shared_ptr<parameter> > the_par,
             boost::shared_ptr<std::vector<entry> > thefit_vec) :
             _toCalib(thetoCalib), _par(the_par), fit_vec(thefit_vec) {
       init();
 }
 
 //--------------
 // Destructor --
 //--------------
 
 EmcCalibrationLh::~EmcCalibrationLh() {
 }
 
 void EmcCalibrationLh::init() {
       _maxFCN = -1e30;
 }
 
 double const EmcCalibrationLh::normalize(const std::vector<double>& par) {
       double norm = par[3] * peak_int(par) + (1. - par[3]) * background_int(par);
       //    if(norm<1e-10) norm=1e-10;
       return norm;
 }
 
 double const EmcCalibrationLh::getVal(double &x, const std::vector<double>& par) {
       double f = par[3] * peak(x, par) + (1. - par[3]) * background(x, par);
       return f;
 }
 
 inline static double sq(double t) {
       return t * t;
 }
 
 double const EmcCalibrationLh::peak(double &x, const std::vector<double>& par) {
 
       double peak = par[0];
       double tail = par[1];
       double width = par[2];
 
       double f = 0.;
 
       double qa = 0., qb = 0., qc = 0., qx = 0., qy = 0.;
 
       if (fabs(tail) < 1e-7)
             qc = 0.5 * sq((x - peak) / width);
       else {
             qa = tail * sqrt(log(4.));
             qb = sinh(qa) / qa;
             qx = (x - peak) / width * qb;
             qy = 1. + tail * qx;
 
             //---- Cutting curve from right side
             if (qy > 1e-7)
                   qc = 0.5 * (sq(log(qy) / tail) + tail * tail);
             else
                   qc = 15.0;
       }
 
       f = exp(-qc);
       return f;
 }
 
 double const EmcCalibrationLh::peak_int(const std::vector<double>& par) {
       double width = par[2];
 
       double f = 0.;
 
       //Novosibirsk Integral if whole peak is in range
       f = width * sqrt(2. * M_PI); //total integral
       f -= (peak((double&) _toCalib->min, par) + peak((double&) _toCalib->max, par))
                   * ((double) _toCalib->max - (double) _toCalib->min) / 4.; //estimation of correction for borders of integral
       return f;
 }
 
 inline static double p1(double t, double a, double b) {
       return a * t + b;
 }
 
 double const EmcCalibrationLh::background(double& xval, const std::vector<double>& par) {
 
       double x = (-1. + 2. * (xval - _toCalib->min) / (_toCalib->max - _toCalib->min));
       double x2 = sq(x);
       double sum = 0.;
       sum += par[6] * x * p1(x2, 4., -3.);
       sum += par[5] * p1(x2, 2., -1.);
       sum += par[4] * x + 1.;
       return sum;
 }
 
 double const EmcCalibrationLh::background_int(const std::vector<double>& par) {
 
       double norm = par[5] * (-1. + 2. / 3.) + 1.;
 
       norm *= _toCalib->max - _toCalib->min;
       return norm;
 }
 
 double const EmcCalibrationLh::calcLh(const std::vector<double>& par) {
       boost::shared_ptr<std::vector<entry> > theFitVec = fit_vec;
       boost::shared_ptr<parameter> theToCalib = _toCalib;
 
       double logl = 0.;
       vector<entry>::iterator it;
       for (it = (*theFitVec).begin(); it != (*theFitVec).end(); it++) { //loop over the histogram
             double x = (*it).m_gg;
             if (theToCalib->min < x && x < theToCalib->max) {
                   double val = getVal(x, par) / normalize(par);
                   if (val <= 0.) {
 //                      return 0.;
                         return DBL_MAX;
                   }
                   logl -= log(val); //log likelihood
             }
       }
 
       return logl;
 
 }
 
 //double const EmcCalibrationLh::calcLh(const std::vector<boost::shared_ptr<double> > par) {
 //    std::vector<double> thePar;
 //
 //    std::vector<boost::shared_ptr<double> >::const_iterator it_par;
 //
 //    for (it_par = par.begin(); it_par != par.end(); it_par++) {
 //          thePar.push_back((*(*it_par).get()));
 //    }
 //
 //    return calcLh(thePar);
 //
 //}
