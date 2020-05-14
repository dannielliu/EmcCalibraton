 // Description:
 
 //#include "EmcCalibStandalone.hh"
 
 #include <map>
 #include <vector>
 
 #include <iostream>
 #include <string>
 #include <fstream>
 #include <sstream>
 #include "unistd.h"
 
 #include <boost/serialization/map.hpp>
 #include <boost/serialization/vector.hpp>
 
 #include <boost/archive/text_oarchive.hpp>
 #include <boost/archive/text_iarchive.hpp>
 
 #include <boost/archive/tmpdir.hpp>
 #include <boost/archive/xml_iarchive.hpp>
 #include <boost/archive/xml_oarchive.hpp>
 
 //#include <boost/thread.hpp>
 
 #include "EmcCalibFactory.hh"
 #include "EmcCalibration.hh"
 //#include "EmcCalibContainer.hh"
 #include "EmcCalibDataModel.hh"
 
 using namespace std;
 
 map<const int, std::vector<entry> > _histo;
 
 EmcCalibration* cal;
 
 int dataset = 1;
 int nr_files = 100; //max: 1000
 
 int main() {
 
       cal = EmcCalibFactory::getEmcCalib("Minuit2", "vpt"); //Minuit     Minuit2     Geneva
       //cal = EmcCalibFactory::getEmcCalib("Minuit2", "vprt"); //Minuit     Minuit2     Geneva
 //    cal = EmcCalibFactory::getEmcCalib("Geneva", "vprt");
 
       cal->setFilePath("./EmcCalibStudies/EmcCalibrationData3.dat");
       cal->setRootFilePath("root/");
 //    cal->setMaxIterations(1);
 
       cal->run();
 
       return 0;
 }
