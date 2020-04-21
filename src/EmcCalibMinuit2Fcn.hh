 //--------------------------------------------------------------------------
 // File and Version Information:
 //    $Id: EmcCalibMinuit2Fcn.hh,v 1.5 2011/10/14 12:22:11 bernhard Exp $
 //
 // Description:
 //    Class EmcCalibMinuit2 - for calibration the EMC during runtime
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
 
 #ifndef EMCCALIBMINUIT2FCN_HH
 #define EMCCALIBMINUIT2FCN_HH
 
 #include "Minuit2/FCNBase.h"
 #include "Minuit2/MnUserParameters.h"
 #include "EmcCalibrationLh.hh"
 
 #include <boost/shared_ptr.hpp>
 #include <string>
 
 using namespace ROOT::Minuit2;
 
 class EmcCalibMinuit2Fcn:
             public FCNBase {
 public:
       // Constructors
       EmcCalibMinuit2Fcn(boost::shared_ptr<EmcCalibrationLh> theEmcCalibrationLh);
 
       // Destructor
       virtual ~EmcCalibMinuit2Fcn();
 
       void initFitParameters(MnUserParameters& upar);
 
       virtual double operator()(const std::vector<double>& par) const;
 
       virtual double Up() const;
 
 private:
       boost::shared_ptr<EmcCalibrationLh> _emcCalibrationLh;
 };
 
 #endif
 
