 //--------------------------------------------------------------------------
 // File and Version Information:
 //    $Id: EmcCalibFactory.cc,v 1.10 2012/02/09 15:28:39 bernhard Exp $
 //
 // Description:
 //    Class EmcCalibration - a bare outline of an analysis which should
 //      run on either the mini or micro, suitable for filling in with 
 //      additional code.
 //------------------------------------------------------------------------
 
 //-----------------------
 // This Class's Header --
 //-----------------------
 //#include "EmcCalibStudies/EmcCalibFactory.hh"
 #include "EmcCalibFactory.hh"
 
 //-------------
 // C Headers --
 //-------------
 
 //---------------
 // C++ Headers --
 //---------------
 
 #include <string>
 
 #include "EmcCalibration.hh"
 #include "EmcCalibMinuit.hh"
 #include "EmcCalibMinuit2.hh"
 //#include "EmcCalibGeneva.hh"
 
 using namespace std;
 
 EmcCalibFactory* EmcCalibFactory::_thisEmcCalibFactory = 0;
 
 EmcCalibFactory::EmcCalibFactory() {
 }
 
 EmcCalibFactory::~EmcCalibFactory() {
       if (0 != _thisEmcCalibFactory) {
             delete _thisEmcCalibFactory;
       }
 }
 
 EmcCalibFactory* EmcCalibFactory::instance() {
       if (_thisEmcCalibFactory == 0)
             _thisEmcCalibFactory = new EmcCalibFactory();
       return _thisEmcCalibFactory;
 }
 
 EmcCalibration* EmcCalibFactory::getEmcCalib(const char* calibrator, const char* options) {
       string opts(calibrator);
       EmcCalibration* result = 0;
       if (opts == "Minuit") {
             result = new EmcCalibMinuit(options);
             result->minimizer = 1;
 //    }else if (opts == "Geneva") {
 //          return new EmcCalibGeneva(options);
 //          result->minimizer = 3;
       } else if (opts == "Minuit2") {
             result = new EmcCalibMinuit2(options);
             result->minimizer = 2;
       } else {
             result = new EmcCalibMinuit2(options);
             result->minimizer = 2;
       }
       return result;
 }
 
 EmcCalibration* EmcCalibFactory::getEmcCalib(const char* calibrator) {
       string opts(calibrator);
       EmcCalibration* result = 0;
       if (opts == "Minuit") {
             result = new EmcCalibMinuit();
             result->minimizer = 1;
 //    } else if (opts == "Geneva") {
 //          return new EmcCalibGeneva();
 //          result->minimizer = 3;
       } else if (opts == "Minuit2") {
             result = new EmcCalibMinuit2();
             result->minimizer = 2;
       } else {
             result = new EmcCalibMinuit2();
             result->minimizer = 2;
       }
       return result;
 }
 
