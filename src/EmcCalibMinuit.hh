 //--------------------------------------------------------------------------
 // File and Version Information:
 //    $Id: EmcCalibMinuit.hh,v 1.9 2012/01/24 12:27:29 bernhard Exp $
 //
 // Description:
 //    Class EmcCalibMinuit - for calibration the EMC during runtime
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
 
 #ifndef EMCCALIBMINUIT_HH
 #define EMCCALIBMINUIT_HH
 
 #include "EmcCalibrationLh.hh"
 #include <boost/shared_ptr.hpp>
 
 class EmcCalibMinuit:
             public EmcCalibration {
 
       //--------------------
       // Instance Members --
       //--------------------
 
 public:
 
       // Constructors
       EmcCalibMinuit(const char* options);
       EmcCalibMinuit();
 
       // Destructor
       virtual ~EmcCalibMinuit();
 
       // Operations
 
       // Assessors
       double maxFCN() {
             return _maxFCN;
       }
 
       EmcCalibrationLh* _emcCalibLhPtr;
       int par_size() {
             return _par.size();
       }
 
 protected:
       virtual data fit(std::vector<entry> &fit_vec);
       virtual void printStatus();
       std::vector<boost::shared_ptr<parameter> > param();
 //    boost::shared_ptr<std::vector<entry> > fitVecPtr();
 
 private:
       double _maxFCN;
       void init();
 };
 
 void fcn_model(Int_t &cpnr, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
 
 #endif
