 //--------------------------------------------------------------------------
 // File and Version Information:
 //    $Id: EmcCalibMinuit2.hh,v 1.9 2012/01/24 12:27:29 bernhard Exp $
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
 
 #ifndef EMCCALIBMINUIT2_HH
 #define EMCCALIBMINUIT2_HH
 
 #include "Minuit2/FCNBase.h"
 #include "Minuit2/MnUserParameters.h"
 #include "EmcCalibration.hh"
 
 #include <boost/shared_ptr.hpp>
 
 using namespace ROOT::Minuit2;
 
 //class EmcCalibFcn;
 
 class EmcCalibMinuit2:
             public EmcCalibration {
 
       //--------------------
       // Instance Members --
       //--------------------
 
 public:
 
       // Constructors
       EmcCalibMinuit2(const char* options);
       EmcCalibMinuit2();
       //    EmcCalibMinuit2(const EmcCalibMinuit2& theEmcCalibMinuit2);
 
       // Destructor
       virtual ~EmcCalibMinuit2();
 
       // Operations
 
       // Assessors
       double maxFCN() {
             return _maxFCN;
       }
 
 //    boost::shared_ptr<std::vector<entry> > fitVecPtr();
       //    bool initFitParameters(MnUserParameters& minuitParams);
 
 protected:
       virtual data fit(std::vector<entry> &fit_vec);
       virtual void printStatus();
       std::vector<boost::shared_ptr<parameter> > param();
 
 private:
       double _maxFCN;
       void init();
 };
 
 #endif
