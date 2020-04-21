 //--------------------------------------------------------------------------
 // File and Version Information:
 //    $Id: EmcCalibWrapper.hh,v 1.1 2012/02/09 15:28:39 bernhard Exp $
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
 
 #ifndef EMCCALIBWRAPPER_HH
 #define EMCCALIBWRAPPER_HH
 
 //#include "EmcCalibContainer.hh"
 #include "EmcCalibDataModel.hh"
 
 class EmcCalibWrapper {
 
       //--------------------
       // Instance Members --
       //--------------------
 
 public:
 
 //    EmcCalibWrapper(const char* calibrator, const char* options);
 //
 //    EmcCalibWrapper(const char* calibrator);
 
       EmcCalibWrapper() ;
 
       virtual ~EmcCalibWrapper();
 
       int connectDB();
       void setDBname(const char* name = "PndECal");
       void setDBhost(const char* url = "localhost");
       void setDBuser(const char* user = "emccalusr");
       void setDBpassword(const char* password = "");
 
       void cacheDataSQL(const int cpnr, entry &eintrag);
       int dumpDataSQL();
       int setupCalibMap(std::map<const int, xtal_def> &def);
 
       float getCorrSQL(int cpnr);
 //
 //    void setRange(double &range);
 //    void setRange(double &min, double &max);
 //    void setFilePath(const char* path);
 //    void setRootFilePath(const char* path);
 
       //Assessors
 //    void setMaxIterations(int maxIterations);
 //    void setSuppression(float suppression);
 //    void setMeanShift(float meanShift);
 //    void setVarFraction(float varFrac);
 //    void setMaxError(float maxError);
 //    void setPeak(double &val, double &min, double &max);
 //    void setTail(double &val, double &min, double &max);
 //    void setWidth(double &val, double &min, double &max);
 //    void setFraction(double &val, double &min, double &max);
 //    void setC0(double &val, double &min, double &max);
 //    void setC1(double &val, double &min, double &max);
 //    void setC2(double &val, double &min, double &max);
 //    bool load(std::map<const int, data> &calib_map);
 //    int calibrate();
 //    void setSubdetector(const char* detector);
 //    void setSubdetector(const int detector);
 //    int run();
 
       double xmin();
       double xmax();
 
 };
 
 #endif
