 //--------------------------------------------------------------------------
 
 // File and Version Information:
 //    $Id: EmcCalibration.cc,v 1.28 2012/02/16 15:04:30 bernhard Exp $
 //
 // Description:
 //    Class EmcCalibration - a bare outline of an analysis which should
 //      run on either the mini or micro, suitable for filling in with 
 //      additional code.
 //------------------------------------------------------------------------
 
 //-----------------------
 // This Class's Header --
 //-----------------------
 //#include "EmcCalibStudies/EmcCalibration.hh"
 #include "EmcCalibration.hh"
 
 //---------------
 // C++ Headers --
 //---------------
 #include <math.h>
 #include <fstream>
 #include <string>
 #include <sstream>
 
 //#include "PDT/Pdt.hh"
 
 #include <map>
 #include <vector>
 
 #include <boost/serialization/map.hpp>
 #include <boost/serialization/vector.hpp>
 
 #include <boost/archive/tmpdir.hpp>
 #include <boost/archive/xml_iarchive.hpp>
 #include <boost/archive/xml_oarchive.hpp>
 
 #include <boost/archive/text_oarchive.hpp>
 #include <boost/archive/text_iarchive.hpp>
 
 #include <boost/shared_ptr.hpp>
 
 #include "EmcCalibHelper.hh"
 #include "EmcCalibIO.hh"
 
 #include "TFile.h"
 #include "TNtuple.h"
 
 using namespace std;
 
 //-----------------------------------------------------------------------
 // Local Macros, Typedefs, Structures, Unions and Forward Declarations --
 //-----------------------------------------------------------------------
 
 //----------------
 // Constructors --
 //----------------
 
 EmcCalibration::EmcCalibration(const char* options) {
       TString opts(options);
       init();
       if (opts.Contains("v")) {
             _printLevel = 1;
       }
       if (opts.Contains("p")) {
             _printStatus = true;
       }
       if (opts.Contains("r")) {
             _rootOutput = true;
             initRootOutput();
       }
       if (opts.Contains("t")) {
 //          _timeInfo = true;
       }
       theCalibIO->setOptions(options);
 }
 
 EmcCalibration::EmcCalibration() {
       init();
 }
 
 void EmcCalibration::init() {
       double mPi0 = 0.1349766;
       double range = 0.1;
 
       _toCalib = parameter("Pi0", mPi0, mPi0 - range, mPi0 + range);
 
       _maxIterations = 10; //20
       _suppression = 0.95; 
 
       _varFrac = 1.05; //Fraction of sample mean from iter_(n)/iter_(n-1) for interruption
 
       _meanShift = 0.005; 
       //error affects width of peak, but small error throws away a lot of statistics. Is normalized on sqrt(events) per crystal
       _maxError = 0.028; //0.03 0.04,0.017; 0.002
       _maxStError = 0.00113; //0.03 0.04,0.017; 0.002
 
       _par.push_back(parameter("peak", .135, 0.11, 0.16)); 
       _par.push_back(parameter("tail", -0.01, -0.5, 0.)); 
       _par.push_back(parameter("width", .006, 0., 0.01)); 
       _par.push_back(parameter("fraction", 0.15, 0., 1.)); 
       _par.push_back(parameter("c0", -1.3, -10., 0.)); 
       _par.push_back(parameter("c1", 0.6, 0., 10.)); 
       _par.push_back(parameter("c2", -0.2, -10., 0.)); 
 
       _printStatus = false;
       _printLevel = -1;
       _rootOutput = false;
       _rootfile_path = "root/";
       minimizer = 2;
 
       params[0] = 4.071;
       params[1] = 0.678;
       params[2] = 1.0;

       b_init = false;
 
       _subdetector = all;
 
       theHelper = new EmcCalibHelper();
       theCalibIO = new EmcCalibIO();
 }
 
 //--------------
 //--------------
 
 EmcCalibration::~EmcCalibration() {
       delete theHelper;
       delete theCalibIO;
       if (_rootOutput) {
             delete tuple_corr;
             delete tuple_iter;
             delete tuple_xtal;
             delete f_root;
       }
 }
 
 void EmcCalibration::setFilePath(const char* path) {
       theCalibIO->setEmcCalibrationFile(path);
       return;
 }
 
 void EmcCalibration::setRootFilePath(const char* path) {
       _rootfile_path = path;
       return;
 }
 
 void EmcCalibration::initRootOutput() {
       string file = string(_rootfile_path);
       stringstream file_sst;
       file_sst << "EmcCalibOutput_minimizer" << minimizer;
       if (getenv("run") != NULL) {
             file_sst << "-" << getenv("run");
       }
       file.append(file_sst.str());
       file.append(".root");
       f_root = new TFile(file.c_str(), "RECREATE", "ROOT file of EmcCalibration");
       stringstream o;
       o << "EmcCalibration::initRootOutput(): root output will be written in " << file;
       theCalibIO->logger(3, o);
       tuple_xtal =
                   new TNtuple(
                               "xtal",
                               "xtal",
                               "cpnr:value:error:relerror:sterror:tot_value:nentries:iterations:cal_value:cond_range:cond_prv");
       tuple_iter = new TNtuple("iter", "iter", "iteration:mean:var:n_xtal:varFrac:cond_var");
       tuple_corr = new TNtuple("corr", "corr",
                   "iteration:cpnr:cpnr2:shift:fraction:cpnr2_corr:mgg:corr2_range:subDet:frac2");
       return;
 }
 
 void EmcCalibration::setMeanShift(float meanShift) {
       _meanShift = meanShift;
       return;
 }
 
 void EmcCalibration::setVarFraction(float varFrac) {
       _varFrac = varFrac;
       return;
 }
 
 void EmcCalibration::setMaxIterations(int maxIterations) {
       _maxIterations = maxIterations;
       return;
 }
 
 void EmcCalibration::setSuppression(float suppression) {
       _suppression = suppression;
       return;
 }
 
 void EmcCalibration::setMaxError(float maxError) {
       _maxError = maxError;
       return;
 }
 
 void EmcCalibration::setRange(double &range) {
       _toCalib = parameter("Pi0", _toCalib.value, _toCalib.value - range, _toCalib.value + range);
       return;
 }
 
 void EmcCalibration::setRange(double &min, double &max) {
       _toCalib = parameter("Pi0", _toCalib.value, min, max);
       return;
 }
 
 void EmcCalibration::setPeak(double &val, double &min, double &max) {
       _par[0] = parameter("peak", val, min, max);
       return;
 }
 void EmcCalibration::setTail(double &val, double &min, double &max) {
       _par[1] = parameter("tail", val, min, max);
       return;
 }
 void EmcCalibration::setWidth(double &val, double &min, double &max) {
       _par[2] = parameter("width", val, min, max);
       return;
 }
 void EmcCalibration::setFraction(double &val, double &min, double &max) {
       _par[3] = parameter("fraction", val, min, max);
       return;
 }
 void EmcCalibration::setC0(double &val, double &min, double &max) {
       _par[4] = parameter("c0", val, min, max);
       return;
 }
 void EmcCalibration::setC1(double &val, double &min, double &max) {
       _par[5] = parameter("c1", val, min, max);
       return;
 }
 void EmcCalibration::setC2(double &val, double &min, double &max) {
       _par[6] = parameter("c2", val, min, max);
       return;
 }
 
 void EmcCalibration::setSubdetector(const int detector = all) {
       _subdetector = detector;
       stringstream o;
       o << "EmcCalibration::setSubdetector(): Calibration applied for ";
       switch (detector) {
       case all:
             o << "all EMC subdetectors";
             break;
       case bkw:
             o << "EMC Backward Endcap";
             break;
       case barrel:
             o << "EMC Barrel";
             break;
       case fwd:
             o << "EMC Forward Endcap";
             break;
       case slk:
             o << "Shaslik EMC";
             break;
       default:
             _subdetector = fwd;
             o << "default fallback: EMC Forward Endcap; Value is not valid. Chose all(" << all
                         << ") backward(" << bkw << ") barrel(" << barrel << ") forward(" << fwd
                         << ") or shashlik(" << slk << ") as integer values.";
             break;
       }
 
       theCalibIO->logger(4, o);
       return;
 }
 
 std::vector<boost::shared_ptr<parameter> > EmcCalibration::parPtr() {
       std::vector<boost::shared_ptr<parameter> > theParPtr;
       parameter thePar;
       for (int i = 0; i < _par.size(); i++) {
             boost::shared_ptr<parameter> thePtr(new parameter(_par[i]));
             theParPtr.push_back(thePtr);
       }
       return theParPtr;
 }
 boost::shared_ptr<std::vector<entry> > EmcCalibration::fitVecPtr(std::vector<entry> &fit_vec) {
       boost::shared_ptr<std::vector<entry> > theFitPtr(new std::vector<entry>((fit_vec)));
       return theFitPtr;
 }
 
 inline static double sq(double t) {
       return t * t;
 }
 
 int EmcCalibration::run(const int detector, std::vector<bool> in_secondary_Det) {
       int result = 0;
       theCalibIO->connectDB();
 
       b_init = true;
 
       setSubdetector(detector);
 
       theCalibIO->logger(3, "EmcCalibration::run(): Starting Calibration");
 
       result = calibrate();
 
       theCalibIO->logger(3, "EmcCalibration::run(): Finished Calibration");
 
       theCalibIO->deleteCache();
 
       if (_rootOutput) {
             f_root->Write();
             f_root->Close();
       }
 
       return result;
 }
 
 int EmcCalibration::run(const int detector) {
       int result = 0;
       theCalibIO->connectDB();
 
       b_init = true;
 
       setSubdetector(detector);
 
       theCalibIO->logger(3, "EmcCalibration::run(): Starting Calibration");
 
       result = calibrate();
 
       theCalibIO->logger(3, "EmcCalibration::run(): Finished Calibration");
 
       theCalibIO->deleteCache();
 
       if (_rootOutput) {
             f_root->Write();
             f_root->Close();
       }
 
       return result;
 }
 
 int EmcCalibration::run() {
       int result = 1;
       theCalibIO->connectDB();
 
       b_init = true;
 
       if (b_init) {
             theCalibIO->deleteCache();
             theCalibIO->clearCalibMap();
       }
 
       setSubdetector(fwd);
       theCalibIO->logger(3, "EmcCalibration::run(): Starting Calibration");
 //    result *= calibrate();
       theCalibIO->logger(3, "EmcCalibration::run(): Finished Calibration");
 
       setSubdetector(barrel);
       theCalibIO->logger(3, "EmcCalibration::run(): Starting Calibration");
       result *= calibrate();
       theCalibIO->logger(3, "EmcCalibration::run(): Finished Calibration");
 
       setSubdetector(bkw);
       theCalibIO->logger(3, "EmcCalibration::run(): Starting Calibration");
 //    result *= calibrate();
       theCalibIO->logger(3, "EmcCalibration::run(): Finished Calibration");
 
       setSubdetector(slk);
       theCalibIO->logger(3, "EmcCalibration::run(): Starting Calibration");
 //    result *= calibrate();
       theCalibIO->logger(3, "EmcCalibration::run(): Finished Calibration");
 
       setSubdetector(all);
       theCalibIO->logger(3, "EmcCalibration::run(): Starting Calibration");
 //      result *= calibrate();
       theCalibIO->logger(3, "EmcCalibration::run(): Finished Calibration");
 
 //    theCalibIO->deleteCache();
 
       if (_rootOutput) {
             f_root->Write();
             f_root->Close();
       }
 
       return result;
 }
 
 int EmcCalibration::calibrate() {
       int result = 0;
       int i_gC = 0;
       float mean = 2., var = 2.;
       map<const int, cal_store> calib_map;
       std::vector<int> xtal_index;
 
       theCalibIO->getXtalIndex(_subdetector, xtal_index);
       
       //if (!b_init) { 
       if (b_init) { 
             result = theCalibIO->loadCalibMap(calib_map);
	     if (result == 1) do_correction(0, calib_map, xtal_index);
       }
 
 
       int current_iteration = 0;
       while (current_iteration <= _maxIterations) {
             theCalibIO->timeOutput(1, "EmcCalibration::calibrate() starting iteration at");
             i_gC = getConstants(mean, var, calib_map, xtal_index); 
             current_iteration++;
             if (i_gC == 0) {
                   stringstream o;
                   o << "EmcCalibration::calibrate(): error occurred at iteration = " << current_iteration;
                   theCalibIO->logger(2, o);
                   result = 0;
                   break;
             }
             if (i_gC == 2) {
                   if (fabs(mean - 1.) > _meanShift) {
                         do_correction(current_iteration, calib_map, xtal_index); 
 
                         int gS = globalShift(mean, calib_map, xtal_index); 
 //                      if (gS) {
                         current_iteration++;
                         stringstream o;
                         o << "EmcCalibration::calibrate(): global shift after iteration = "
                                     << current_iteration << " mean = " << mean << ", var = " << var;
                         theCalibIO->logger(3, o);
                         continue;
 //                      }
                   } else {
                         stringstream o;
                         o << "EmcCalibration::calibrate(): done at iteration = " << current_iteration
                                     << ", mean = " << mean << ", var = " << var;
                         theCalibIO->logger(3, o);
                         result = 1;
                         break;
                   }
             }
 
             do_correction(current_iteration, calib_map, xtal_index); 
 
             stringstream o;
             o << "EmcCalibration::calibrate(): iteration = " << current_iteration << ", mean = " << mean
                         << ", var = " << var;
             theCalibIO->logger(3, o);
 
       }
 
       if (current_iteration >= _maxIterations) {
             stringstream o;
             o << "EmcCalibration::calibrate():  maxIterations= " << _maxIterations
                         << " reached. Iteration aborted";
             theCalibIO->logger(2, o);
       }
 
       if (result == 1) {
             stringstream o;
             o << "EmcCalibration::calibrate(): Succeeded. Saving calibration constants.";
             theCalibIO->logger(3, o);
             theCalibIO->saveCalibMap(calib_map);
       }
 
       theCalibIO->dumpDataSQL(); // add by Dong

       theCalibIO->timeOutput(0, "EmcCalibration::calibrate():");
 
       return result;
 }
 
 int EmcCalibration::getConstants(float &mean, float &var, map<const int, cal_store> &calib_map,
 std::vector<int> &xtal_index) {
 
       int result = 0;
       std::vector<int>::iterator it;
 
       data ergebnis(1., 1.);
       int cpnr = 0;
       int n = 0;
       float cal = 0., quad = 0.;
 
       std::vector<entry> fit_vec;
 
 //    for (it = xtal_index.begin(); it != xtal_index.end(); it++) { //loop over all crystals
        stringstream o1;
       o1 << "EmcCalibration::getConstants(): xtalsize = " << xtal_index.size() ;
       theCalibIO->logger(3, o1);
 

       for (int i = 0; i < xtal_index.size(); i++) {
             cpnr = xtal_index[i];
 
             //          fit_vec = &(*it).second;
             int res_lFV = theCalibIO->loadFitVec(b_init, cpnr, fit_vec);
             if (res_lFV == 2 && (!b_init)) { 
                   res_lFV = theCalibIO->loadFitVec(true, cpnr, fit_vec);
                   if (res_lFV == 1)
                         calib_map[cpnr].clear(); 
             }
             if (res_lFV == 0) {
                   continue;
             }
 
             ergebnis = fit(fit_vec); 
 
             stringstream o;
             o << "EmcCalibration::getConstants(): size of vector = " << fit_vec.size() << endl;
             o << "EmcCalibration::getConstants(): cpnr = " << cpnr << " ergebnis.value = "
                         << ergebnis.value << " ergebnis.rel_error() = " << float(ergebnis.rel_error());
             theCalibIO->logger(3, o);
 
             float boarder =(_par[0].max-_par[0].min)/ 50.;// d par=0.05; 50 -> 0.001,  100-> 0.0005
             bool b_range = (ergebnis.value > (_par[0].min + boarder)
                         && ergebnis.value < (_par[0].max - boarder));


             bool b_prv = fabs(sq(_toCalib.value / ergebnis.value) - 1.)
                         > fabs(calib_map[cpnr].back().value - 1.);

             float stError = ergebnis.rel_error() / sqrt(fit_vec.size());
             if (_rootOutput) {
                   int i_prv = 0, i_range = 0;
                   if (b_range)
                         i_range = 1;
                   if (b_prv)
                         i_prv = 1;
                   tuple_xtal->Fill(
                               (float) cpnr, //cpnr
                               (float) ergebnis.value, //value
                               (float) ergebnis.error, //error
                               (float) ergebnis.rel_error(), //relerror
                               (float) stError, //sterror
                               (float) (calib_map[cpnr].total.value
                                           * (sq(_toCalib.value / ergebnis.value) - 1.) * _suppression + 1.),
                               (float) fit_vec.size(), //nentries
                               (float) calib_map[cpnr].iterations, //iterations
                               (float) sq(_toCalib.value) / sq(ergebnis.value), // cal_value
                               i_range, //condition: is in range
                               i_prv); //condition: improve result
             }
             if (ergebnis.value < (_par[0].min + boarder)) {
                   ergebnis.value = _par[0].min + (1.1 * boarder); 
             }
             if (ergebnis.value > (_par[0].max - boarder)) {
                   ergebnis.value = _par[0].max - (1.1 * boarder); 
             }
             if (!b_range) {
                   stringstream o;
                   o << "EmcCalibration::getConstants(): Xtal with cpnr " << cpnr << " out of range, with "
                               << ergebnis.value << " +- " << ergebnis.error;
                   theCalibIO->logger(3, o);
 //                continue;
             }
 
             if (ergebnis.value < 0.001) {
                   stringstream o;
                   o << "EmcCalibration::getConstants(): Xtal with cpnr " << cpnr
                               << " has too less entries, with " << ergebnis.value << " +- " << ergebnis.error;
                   theCalibIO->logger(3, o);
                   continue;
             }
             if (ergebnis.rel_error() > _maxError) {
                   stringstream o;
                   o << "EmcCalibration::getConstants(): Xtal with cpnr " << cpnr << " error of "
                               << ergebnis.value << " +- " << ergebnis.error << " too high - not accepted";
                   theCalibIO->logger(3, o);
                   continue;
             }
 
             if (stError > _maxStError) {
                   stringstream o;
                   o << "EmcCalibration::getConstants(): Xtal with cpnr " << cpnr << " st error of "
                               << ergebnis.value << " +- " << ergebnis.error << " too high - not accepted";
                   theCalibIO->logger(3, o);
                   continue;
             }
 
             ergebnis.value = sq(_toCalib.value / ergebnis.value); 
             ergebnis.error = 2 * sq(_toCalib.value) / (sq(ergebnis.value) * ergebnis.value)
                         * ergebnis.error; 
 
             //          ergebnis.value = _toCalib.value / ergebnis.value;
             //          ergebnis.error = _toCalib.value / sq(ergebnis.value) * ergebnis.error;
 
             if (fabs(ergebnis.value - 1.) > fabs(calib_map[cpnr].back().value - 1.)) {
 
                   stringstream o;
                   o << "EmcCalibration::getConstants(): value = " << ergebnis.value
                               << " worser than last one of = " << calib_map[cpnr].back().value;
                   theCalibIO->logger(3, o);
 //                continue;
             }
 
             if (calib_map[cpnr].size() == 0) {
                   calib_map[cpnr].clear();
             }
 
             calib_map[cpnr].total.value *= ((ergebnis.value - 1.) * _suppression + 1.);
             calib_map[cpnr].total.error *= _suppression * ergebnis.error;
             calib_map[cpnr].push_back(ergebnis);
             //calib_map[cpnr].iterations = calib_map[cpnr].size(); // add by Dong
             cal += ergebnis.value;
             quad += sq(ergebnis.value);
             n++;
 
             stringstream o1;
             o1 << "EmcCalibration::getConstants(): cal = " << ergebnis.value;
             theCalibIO->logger(3, o1);
       } 
 
       float new_mean, new_var;
       new_mean = cal / n; 
       new_var = sqrt((quad - cal * cal / n) / (n - 1.)); 
 
       stringstream o;
       o << "EmcCalibration::getConstants(): Average EmcCalibration mean = " << mean << "; var = "
                   << var << " for n = " << n;
       theCalibIO->logger(3, o);
 
       if (_rootOutput) {
             int i_interrupt = 0;
             if ((new_var / var) > _varFrac)
                   i_interrupt = 1;
             tuple_iter->Fill((float) calib_map[cpnr].iterations, //iteration (just from last cpnr)
                         new_mean, //mean of peak distributions
                         new_var, //Sample mean of peak distributions
                         n, //number of processed crystals
                         (new_var / var), //value for varFrac
                         i_interrupt //Condition for interrupt
                         );
       }
 
       if (n > 0)
             result = 1;
 
       if (fabs(new_mean - 1.) <= _meanShift) { 
             stringstream o;
             o << "EmcCalibration::getConstants(): interrupt: mean = " << new_mean << " < " << _meanShift
                         << " (given condition for interrupt)";
             theCalibIO->logger(3, o);
             result = 2;
       }
       if (fabs(new_mean - 1.) > fabs(mean - 1.)) { 
             stringstream o;
             o << "EmcCalibration::getConstants(): interrupt: new_mean = " << new_mean << " > " << mean
                         << " (old value)";
             theCalibIO->logger(3, o);
             result = 2;
       }
       if ((new_var / var) > _varFrac) { 
             stringstream o;
             o << "EmcCalibration::getConstants(): interrupt: new_var = " << new_var << " > " << var
                         << " (old value)";
             theCalibIO->logger(3, o);
             result = 2;
       }
 
       mean = new_mean;
       var = new_var;
 
       return result;
 }
 
 int EmcCalibration::do_correction(
             int current_iteration,
             std::map<const int, cal_store> &calib_map
             ,std::vector<int> &xtal_index) {
 
       int result = 1;
 
       vector<int>::iterator it;
       vector<entry> fit_vec;
       vector<entry>::iterator it_vec;
       float shift = 1.;
       int cpnr;
 
       for (it = xtal_index.begin(); it != xtal_index.end(); it++) { 
             cpnr = (*it);
 
             if ((abs(calib_map[cpnr].iterations)) > (5 * _maxIterations)) 
                   calib_map[cpnr].iterations = 1;
 
             // comment by Dong, iterations is not a valid variable?
	     //if (calib_map[cpnr].iterations == current_iteration) 
	     { 
 
                   shift = ((calib_map[cpnr].back().value - 1.) * _suppression) + 1.; 
 
                   int res_lFV = theCalibIO->loadFitVec(b_init, cpnr, fit_vec);
                   if (res_lFV == 2 && (!b_init)) { 
                         res_lFV = theCalibIO->loadFitVec(true, cpnr, fit_vec);
                   }
 
                   for (int i = 0; i < fit_vec.size(); i++) {
                         if (theCalibIO->is_centralXtal(fit_vec.at(i).cpnr2)) {
                               //result *= do_reco(shift, cpnr, calib_map, fit_vec.at(i));
                               result *= do_reco(shift, cpnr, calib_map, fit_vec.at(i), true);
                         }
                   }
 
                   theCalibIO->cacheFitVec(cpnr, fit_vec); 
             }
       }
 
       b_init = false; 
 
       return result;
 }
 
 int EmcCalibration::do_reco(
             float &shift,
             int cpnr,
             std::map<const int, cal_store> &calib_map
             ,entry &hist) {
       int result = 0;
 
       float fraction = hist.fraction; 
       float fraction2 = hist.fraction2;
       float corr_g2 = calib_map[hist.cpnr2].total.value; 
 
       int corr2_range = 0;
 
       if (b_init && _rootOutput) {
             tuple_corr->Fill(calib_map[cpnr].iterations - 1, cpnr, hist.cpnr2, shift, fraction, corr_g2,
                         hist.m_gg, corr2_range, _subdetector, fraction2);
       }
 
       if (corr_g2 < _par[0].min || corr_g2 > _par[0].max) { 
             corr_g2 = 1.;
             corr2_range = 1;
       }
 
       corr_g2 = corr_g2 * fraction2 + 1. - fraction2;
 
 //    hist.m_gg = sqrt(shift )  * hist.m_gg;
       hist.m_gg = sqrt(shift * fraction + 1. - fraction) * sqrt(corr_g2) * hist.m_gg;
       result = 1;
 
       if (result == 0) {
             hist.m_gg = sqrt(shift * fraction + 1. - fraction) * sqrt(corr_g2) * hist.m_gg; 
             result = 1;
       }
 
       if (_rootOutput) {
             //if(cpnr==6160611||cpnr==332074||cpnr==6151311||cpnr==6151213||cpnr == 6141208||cpnr==6110407)
             tuple_corr->Fill(calib_map[cpnr].iterations, cpnr, hist.cpnr2, shift, fraction,
                         calib_map[hist.cpnr2].total.value, hist.m_gg, corr2_range, _subdetector, fraction2);
 //          "iteration:cpnr:cpnr2:shift:fraction:cpnr2_corr:mgg:corr2_range:subDet")
       }
 
       return result;
 }
 
 int EmcCalibration::do_reco(
             float &shift,
             int cpnr,
             std::map<const int, cal_store> &calib_map
             ,entry &hist, bool all_hit) {

       if (!all_hit) return do_reco( shift, cpnr, calib_map, hist);

       int result = 0;
 
       int nhit1 = hist.this_bump.size();
       int nhit2 = hist.associated_bump.size();
       double ecls1 = 0.0;
       double ecls2 = 0.0;
       double eseed1 = 0.0;
       double eseed2 = 0.0;
       // update hit energy
       for (int ih=0; ih<nhit1; ih++) {
         int cp = hist.this_bump.at(ih).cpnr;
	 double factor = 1.0;
	 if (calib_map.find(cp)!=calib_map.end()) factor = ( calib_map[cp].back().value - 1.0) * _suppression + 1.0;
	 hist.this_bump.at(ih).E_dep *= factor;
	 ecls1 += hist.this_bump.at(ih).E_dep;
	 if (cp == cpnr) eseed1 = hist.this_bump.at(ih).E_dep;
       }
       for (int ih=0; ih<nhit2; ih++) {
         int cp = hist.associated_bump.at(ih).cpnr;
	 double factor = 1.0;
	 if (calib_map.find(cp)!=calib_map.end()) factor = ( calib_map[cp].back().value - 1.0) * _suppression + 1.0;
	 hist.associated_bump.at(ih).E_dep *= factor;
	 ecls2 += hist.associated_bump.at(ih).E_dep;
	 if (cp == hist.cpnr2) eseed2 = hist.associated_bump.at(ih).E_dep;
       }
       // update hit position
       TVector3 where1, where2;
       liloWhere(where1, hist.this_bump);
       liloWhere(where2, hist.associated_bump);
       hist.angle = where1.Angle(where2);
       hist.m_gg  = sqrt(2.0*ecls1*ecls2*(1-cos(hist.angle)));
       if (eseed1>1e-5) hist.fraction  = eseed1/ecls1;
       else {
             stringstream o;
             o << "EmcCalibration::do_reco(): energy of seed1 is too small" << eseed1 << " < " << 1e-5 ;
             theCalibIO->logger(3, o);
       }
       if (eseed2>1e-5) hist.fraction2 = eseed2/ecls2;
       else {
             stringstream o;
             o << "EmcCalibration::do_reco(): energy of seed2 is too small" << eseed2 << " < " << 1e-5 ;
             theCalibIO->logger(3, o);
       }

       result = 1;
 
 
       return result;
 }
 
 int EmcCalibration::liloWhere(TVector3 &where, std::vector<hit> bump){
	Double_t offsetParmA=params[0];
	Double_t offsetParmB=params[1];
	Double_t offsetParmC=params[2];

	//const double lClusEnergy=Energy();
	
	//assert(lClusEnergy!=0);
	double lClusEnergy = 0.0;
	std::vector<hit>::const_iterator current;
	int nhit = bump.size();
	for (current=bump.begin();current!=bump.end();++current)
	{
	       hit tmphit = *current; 
	       lClusEnergy += tmphit.E_dep;
	}
	const double lOffset=
		offsetParmA-offsetParmB*exp(-offsetParmC*pow(lClusEnergy,1.171)) * pow(lClusEnergy,-0.534);
	
	TVector3 lLiloPoint( 1, 1, 1 );
	
	TVector3 lLinSum( 0, 0, 0 );
	TVector3 lLogSum( 0, 0, 0 );
	
	double lLogWeightSum=0;
	int lLogNum=0;
	
	bool lLogSecondTheta  = false;
	bool lLogSecondPhi    = false;
	int  lLogFirstTheta   = -666;
	int  lLogFirstPhi     = -666;
	
	//std::vector<Int_t>::const_iterator current;
	//std::vector<hit>::const_iterator current;
	//int nhit = bump.size();
	for (current=bump.begin();current!=bump.end();++current)
	{
	        hit tmphit = *current;
		//const TVector3 lDigiWhere=lDigi->where();
		const TVector3 lDigiWhere(tmphit.X, tmphit.Y, tmphit.Z);
		TVector3 pos(tmphit.X, tmphit.Y, tmphit.Z);
		const int lDigiTheta = (int)(pos.Theta()/3.14*360);
		const int lDigiPhi   = (int)(pos.Phi()/3.14*360);
		//const int lDigiTheta=lDigi->GetThetaInt();
		//const int lDigiPhi=lDigi->GetPhiInt();
		const double lDigiEnergy=tmphit.E_dep;
		const double lLinWeight=lDigiEnergy/lClusEnergy;
		const double lLogWeight=lOffset+log(lLinWeight);
		
		lLinSum+=lLinWeight*lDigiWhere;
		if(lLogWeight>0)
		{
			lLogSum+=lLogWeight*lDigiWhere;
			lLogWeightSum+=lLogWeight;
			lLogNum++;
			
			if(lLogNum==1)
			{ 
					lLogFirstTheta=lDigiTheta;
					lLogFirstPhi=lDigiPhi;
			}
			else
			{
				if(!lLogSecondTheta&&lDigiTheta!=lLogFirstTheta) 
					lLogSecondTheta=true;
				if(!lLogSecondPhi&&lDigiPhi!=lLogFirstPhi) 
					lLogSecondPhi=true;
			}
		}
	}
  
	if(lLogNum>0) lLogSum*=1./lLogWeightSum;
	
	
	lLiloPoint.SetTheta(lLogSecondTheta?lLogSum.Theta():lLinSum.Theta());
	lLiloPoint.SetPhi(lLogSecondPhi?lLogSum.Phi():lLinSum.Phi());
	
	
	// First, find out if the point is outside the crystal.
	const TVector3 lLiloVector = lLiloPoint;
	if (lLogNum > 1) {
		// Use logarithmic centroid position
		lLiloPoint.SetMag(lLogSum.Mag());
	} else {
		// Use linear centroid position
		lLiloPoint.SetMag(lLinSum.Mag());
	}
	
        where = lLiloPoint;

	return 1;

 }

 int EmcCalibration::globalShift(
             float &mean,
             std::map<const int, cal_store> &calib_map
             ,std::vector<int> &xtal_index) {
 
       int result = 0;
 
       theCalibIO->logger(3, "EmcCalibration::globalShift(): global shifting");
 
       vector<entry> fit_vec;
       vector<int>::iterator it;
       vector<entry>::iterator it_vec;
       float corr = mean;
       corr = ((corr - 1.) * _suppression) + 1.;
       int n = 1;
       int cpnr;
 
       for (it = xtal_index.begin(); it != xtal_index.end(); it++) { 
             cpnr = (*it);
 
             int res_lFV = theCalibIO->loadFitVec(b_init, cpnr, fit_vec);
             if (res_lFV == 2 && (!b_init)) { 
                   res_lFV = theCalibIO->loadFitVec(true, cpnr, fit_vec);
             }
 
             for (int i = 0; i < fit_vec.size(); i++) { 
                   fit_vec.at(i).m_gg *= corr;
             }
 
             theCalibIO->cacheFitVec(cpnr, fit_vec); 
             calib_map[cpnr].total.value *= corr; 
             calib_map[cpnr].push_back(
                         data(calib_map[cpnr].back().value * corr, calib_map[cpnr].back().error));
             n++;
       }
       if (n == calib_map.size()) {
             result = 1;
       }
       return result;
 }
 
