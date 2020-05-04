 //--------------------------------------------------------------------------
 // File and Version Information:
 //    $Id: EmcCalibration.hh,v 1.28 2012/02/16 15:04:30 bernhard Exp $
 //
 // Description:
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
 
 #ifndef EMCCALIBRATION_HH
 #define EMCCALIBRATION_HH
 
 #include <TROOT.h>
 #include <map>
 #include <vector>
 #include <string>
 
 #include "TFile.h"
 #include "TNtuple.h"
 
 #include "EmcCalibHelper.hh"
 //#include "EmcCalibContainer.hh"
 #include "EmcCalibDataModel.hh"
 #include "EmcCalibIO.hh"
 
 #include <boost/archive/text_oarchive.hpp>
 #include <boost/archive/text_iarchive.hpp>
 
 #include <boost/serialization/vector.hpp>
 
 #include <boost/shared_ptr.hpp>
 
 class EmcCalibration:
             public TObject {
 
       //--------------------
       //--------------------
 
 public:
 
       EmcCalibration(const char* options);
       EmcCalibration();
 
       virtual ~EmcCalibration();
 
 
       void setRange(double &range);
       void setRange(double &min, double &max);
 
       void setFilePath(const char* path);
 
       void setRootFilePath(const char* path);
 
       void setMaxIterations(int maxIterations);
 
       void setSuppression(float suppression);
 
       void setMeanShift(float meanShift);
 
       void setVarFraction(float varFrac);
 
       void setMaxError(float maxError);
 
       void setPeak(double &val, double &min, double &max);
 
       void setTail(double &val, double &min, double &max);
 
       void setWidth(double &val, double &min, double &max);
 
       void setFraction(double &val, double &min, double &max);
 
       void setC0(double &val, double &min, double &max);
 
       void setC1(double &val, double &min, double &max);
 
       void setC2(double &val, double &min, double &max);
 
       double xmin() {
             return _toCalib.min;
       }
 
       double xmax() {
             return _toCalib.max;
       }
 
       std::vector<parameter> GetParams() {
             return _par;
       }
 
       int minimizer;
 
       int run();
       int run(const int detector);
       int run(const int detector, std::vector<bool> secondary_Det);
 
       EmcCalibIO* theCalibIO;
       EmcCalibHelper* theHelper;
 
 protected:
 
       bool b_init;
       int _maxIterations;
       float _suppression;
       float _maxError;
       float _maxStError;
       float _varFrac;
       float _meanShift;
       int _subdetector;
 
       bool _printStatus;
       int _printLevel;
       bool _rootOutput;
 
       const char* _rootfile_path;
       std::vector<parameter> _par;
       parameter _toCalib;

       double params[3]; // parameters for the offset of log weight where
 
 
 
       boost::shared_ptr<std::vector<entry> > fitVecPtr(std::vector<entry> &fit_vec);
       std::vector<boost::shared_ptr<parameter> > parPtr();
 
       void setSubdetector(const int detector);
 
       int calibrate();
 
       virtual data fit(std::vector<entry> &fit_vec)=0;
 
       int
       getConstants(
                   float &mean,
                   float &var,
                   std::map<const int, cal_store> &calib_map,
                   std::vector<int> &xtal_index); 
 
       int do_correction(
                   int current_iteration,
                   std::map<const int, cal_store> &calib_map,
                   std::vector<int> &xtal_index);
 
       int do_reco(float &shift, int cpnr, std::map<const int, cal_store> &calib_map, entry &hist);
       int do_reco(float &shift, int cpnr, std::map<const int, cal_store> &calib_map, entry &hist, bool allhit);

       // recalculate where
       int liloWhere(TVector3 &where, std::vector<hit> bump);

       int globalShift(
                   float &mean,
                   std::map<const int, cal_store> &calib_map,
                   std::vector<int> &xtal_index);
 
 private:
       void init();
 
       void initRootOutput();
 
       TFile* f_root;
       TNtuple* tuple_xtal;
       TNtuple* tuple_iter;
       TNtuple* tuple_corr;
 };
 
 #endif
