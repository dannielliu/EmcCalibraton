 //--------------------------------------------------------------------------
 // File and Version Information:
 //    $Id: EmcCalibrationLh.hh,v 1.4 2011/10/14 12:22:11 bernhard Exp $
 //
 //
 // Environment:
 //    Software developed for the PANDA Detector.
 //
 // Author List:
 //      Bernhard Roth                    Original author
 //
 // Copyright Information:
 //    Copyright (C) 2010            Ruhr University Bochum
 //
 //------------------------------------------------------------------------
 
 #ifndef EMCCALIBMINUIT2LH_HH
 #define EMCCALIBMINUIT2LH_HH
 
 #include "Minuit2/FCNBase.h"
 #include "Minuit2/MnUserParameters.h"
 #include "EmcCalibration.hh"
 #include "EmcCalibMinuit2.hh"
 
 #include <boost/shared_ptr.hpp>
 
 using namespace ROOT::Minuit2;
 
 class EmcCalibration;
 
 class EmcCalibrationLh {
 
       //--------------------
       // Instance Members --
       //--------------------
 
 public:
 
       EmcCalibrationLh();
       EmcCalibrationLh(boost::shared_ptr<EmcCalibrationLh> emcCalibrationLhPtr);
       EmcCalibrationLh(
                   boost::shared_ptr<parameter> toCalib,
                   std::vector<boost::shared_ptr<parameter> > par,
                   boost::shared_ptr<std::vector<entry> > fitVec);
 
       virtual ~EmcCalibrationLh();
 
       EmcCalibrationLh* clone_() const {
             return new EmcCalibrationLh(_toCalib, _par, fit_vec);
       }
 
 
       double maxFCN() {
             return _maxFCN;
       }
       //    bool initFitParameters(MnUserParameters& minuitParams);
 
       std::vector<boost::shared_ptr<parameter> > param() {
             return _par;
       }
 
       double const calcLh(const std::vector<double>& par);
 //    double const calcLh(const std::vector<boost::shared_ptr<double> > par);
 
 protected:
       double const normalize(const std::vector<double>& par);
       double const getVal(double &x, const std::vector<double>& par);
       double const background(double &x, const std::vector<double>& par);
       double const background_int(const std::vector<double>& par);
       double const peak(double &x, const std::vector<double>& par);
       double const peak_int(const std::vector<double>& par);
 
       const boost::shared_ptr<parameter> _toCalib;
       const std::vector<boost::shared_ptr<parameter> > _par;
       const boost::shared_ptr<std::vector<entry> > fit_vec;
 
 private:
       double _maxFCN;
       void init();
 
 };
 
 #endif
