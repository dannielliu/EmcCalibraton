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
 
 #ifndef EMCCALIBFACTORY_HH
 #define EMCCALIBFACTORY_HH
 
 class EmcCalibration;
 class EmcCalibMinuit;
 
 
 
 class EmcCalibFactory {
 
 public:
       static EmcCalibFactory* instance();
 
       static EmcCalibration* getEmcCalib(const char* calibrator, const char* options);
 
       static EmcCalibration* getEmcCalib(const char* calibrator);
 
       //  Destructor
       virtual ~EmcCalibFactory();
 
 protected:
 
 private:
       EmcCalibFactory();
       static EmcCalibFactory* _thisEmcCalibFactory;
 };
 
 #endif
