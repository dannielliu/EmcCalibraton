 //--------------------------------------------------------------------------
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
 
 #ifndef EMCCALIBHELPER_HH
 #define EMCCALIBHELPER_HH
 
 #include <vector>
 #include <map>
 
 //#include "EmcCalibContainer.hh"
 #include "EmcCalibDataModel.hh"
 #include "EmcCalibIO.hh"
 
 class EmcCalibHelper {
 
       //--------------------
       // Instance Members --
       //--------------------
 
 public:
 
       EmcCalibHelper();
       EmcCalibHelper(const char* options);
 
       ~EmcCalibHelper();
 
       void setOptions(const char* options);
       int do_reco(float shift, std::map<const int, cal_store> &calib_map, entry &hist);
 
 
 private:
       void init();
       std::vector<int> _edge_xtal;
       const char* _edgeXtalFile;
 
 };
 
 #endif
