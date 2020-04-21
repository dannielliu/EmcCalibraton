 //--------------------------------------------------------------------------
 // File and Version Information:
 //    $Id: EmcCalibWrapper.cc,v 1.2 2012/02/16 15:04:30 bernhard Exp $
 //
 // Description:
 //    Class EmcCalibration - a bare outline of an analysis which should
 //      run on either the mini or micro, suitable for filling in with 
 //      additional code.
 //------------------------------------------------------------------------
 
 //-----------------------
 // This Class's Header --
 //-----------------------
 #include "EmcCalibWrapper.hh"
 
 //-------------
 // C Headers --
 //-------------
 
 //---------------
 // C++ Headers --
 //---------------
 
 #include "EmcCalibration.hh"
 #include "EmcCalibIO.hh"
 #include "EmcCalibHelper.hh"
 #include "EmcCalibFactory.hh"
 
 using namespace std;
 
 std::map<const int, std::vector<entry> > histo_cache;
 EmcCalibration* theEmcCalib;
 
 //EmcCalibWrapper::EmcCalibWrapper(const char* calibrator, const char* options) {
 //}
 //
 //EmcCalibWrapper::EmcCalibWrapper(const char* calibrator) {
 //}
 
 EmcCalibWrapper::EmcCalibWrapper() {
       theEmcCalib = EmcCalibFactory::getEmcCalib("Minuit2", "vpt");
 }
 
 EmcCalibWrapper::~EmcCalibWrapper(){
       delete theEmcCalib;
 }
 
 int EmcCalibWrapper::connectDB(){
       int result = 0;
       result = theEmcCalib->theCalibIO->connectDB();
       return result;
 }
 void EmcCalibWrapper::setDBname(const char* name){
       theEmcCalib->theCalibIO->setDBname(name);
 }
 void EmcCalibWrapper::setDBhost(const char* url){
       theEmcCalib->theCalibIO->setDBhost(url);
 }
 void EmcCalibWrapper::setDBuser(const char* user){
       theEmcCalib->theCalibIO->setDBuser(user);
 }
 void EmcCalibWrapper::setDBpassword(const char* password){
       theEmcCalib->theCalibIO->setDBpassword(password);
 }
 
 void EmcCalibWrapper::cacheDataSQL(const int cpnr, entry &eintrag) {
       theEmcCalib->theCalibIO->cacheDataSQL(cpnr, eintrag);
 }
 
 int EmcCalibWrapper::dumpDataSQL() {
       int result = 0;
       result = theEmcCalib->theCalibIO->dumpDataSQL();
       return result;
 }
 
 float EmcCalibWrapper::getCorrSQL(int cpnr){
       float result = 1.;
       result = theEmcCalib->theCalibIO->getCorrSQL(cpnr);
       return result;
 }
 
 int EmcCalibWrapper::setupCalibMap(std::map<const int, xtal_def> &def) {
       int result = 0;
       result = theEmcCalib->theCalibIO->setupCalibMap(def);
       return result;
 }
 
 double EmcCalibWrapper::xmin(){
       double result = 0;
       result =theEmcCalib->xmin();
       return result;
 }
 double EmcCalibWrapper::xmax(){
       double result = 0;
       result =theEmcCalib->xmax();
       return result;
 }
 
 
