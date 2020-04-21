 //--------------------------------------------------------------------------
 
 // File and Version Information:
 //    $Id: EmcCalibIO.cc,v 1.7 2012/02/16 15:04:30 bernhard Exp $
 //
 // Description:
 //    Class EmcCalibration - a bare outline of an analysis which should
 //      run on either the mini or micro, suitable for filling in with
 //      additional code.
 //------------------------------------------------------------------------
 
 //-----------------------
 // This Class's Header --
 //-----------------------
 #include "EmcCalibIO.hh"
 //---------------
 // C++ Headers --
 //---------------
 
 #include <math.h>
 #include <string>
 #include <fstream>
 #include <sstream>
 #include <istream>
 #include <ostream>
 #include <iostream>
 #include <sys/time.h>
 #include <stdlib.h>
 
 #include <vector>
 #include <map>
 
 #include <boost/serialization/vector.hpp>
 #include <boost/serialization/map.hpp>
 
 #include <boost/archive/text_oarchive.hpp>
 #include <boost/archive/text_iarchive.hpp>
 
 #include <boost/archive/xml_iarchive.hpp>
 #include <boost/archive/xml_oarchive.hpp>
 
 #include <boost/archive/binary_iarchive.hpp>
 #include <boost/archive/binary_oarchive.hpp>
 
 #include <boost/lexical_cast.hpp>
 
// #include "TSQLServer.h"
// #include "TSQLResult.h"
// #include "TSQLRow.h"
 
 //#include <mysql++/mysql++.h>
 
 //#include "EmcRecoSequence.hh"
 
 #include "EmcCalibration.hh"
 
 #include "TVector3.h"
 
 #include "TNtuple.h"
 
 #include "TTree.h"
 #include "TEventList.h"
 
 using namespace std;
 //using namespace mysqlpp;
 
 //-----------------------------------------------------------------------
 // Local Macros, Typedefs, Structures, Unions and Forward Declarations --
 //-----------------------------------------------------------------------
 
 //----------------
 // Constructors --
 //----------------
 
 EmcCalibIO::EmcCalibIO() {
       init();
 }
 
 EmcCalibIO::EmcCalibIO(const char* options) {
       init();
       setOptions(options);
 }
 
 EmcCalibIO::~EmcCalibIO() {
 //          database.~Connection();
       if (0 != histo_cache) {
             delete histo_cache;
       }

       if (db_out!=0){ 
             tpi0->Write();
	     db_out->Close();
       }
 }
 
 void EmcCalibIO::init() {
       _edgeXtalFile = "./EmcCalibStudies/edgeXtals.dat";
       //_edgeXtalFile = "./EmcCalibStudies/xtal_def.dat";
       _EmcCalibrationFile = "./EmcCalibStudies/EmcCalibrationData.dat";
       _verbose = false;
       _timeInfo = false;
       backend = 2;
 
       _max_columns = 6; //maximal colums filles - other events are rejected
       _ent_p_column = 100; //entries per column
       _nr_trials = 6;
 
       cm_table_name = "calValues";
       //hist_table_name = "xtalHistograms";
       //cache_table_name = "cacheHistograms";
       hist_table_name = "./preparedSample.root";
       cache_table_name = "./cachedSample.root";
 
 //    db_url = "mysql://localhost/PndECal";
       db_url = "pc9.ep1.rub.de";
       db_name = "PndECalPPPE"; //PndECal, ecal_test
       db_user = "emccalusr";
       db_password = "";
 
       data_cached = false;
       histo_cache = new std::map<const int, std::vector<entry> >;
 
       logger(3, "EmcCalibIO: EmcCalibIO initialized");
 
       //database = mysqlpp::Connection();
       // create output file
       
       db_out = new TFile("calibratedSample.root","recreate");
       tpi0 = new TTree("candidates","pi0Candidates");
       tpi0->Branch("cpnr", &m_cpnr, 64000, 1);
       tpi0->Branch("entry", &m_entry, 64000, 1);
 
 }
 
 void EmcCalibIO::setOptions(const char* options) {
       TString opts(options);
 
       if (opts.Contains("v")) {
             setVerbose(true);
       }
       if (opts.Contains("t")) {
             setTimeInfo(true);
       }
 }
 
 void EmcCalibIO::printStatus() {
       string db_status = "not connected";
     //if (database.connected()) {
     //      db_status = "connected";
     //}
       stringstream o;
       o << "EmcCalibIO::printStatus(): \n" << "database:    " << db_url << "    db user:  " << db_user
                   << "\n" << "used tables: " << cm_table_name << " (calibration values) and "
                   << hist_table_name << " (histograms)\n" << "status: " << db_status << "\n";
       logger(4, o);
 }
 
 int EmcCalibIO::connectDB() {
       int result = 0;
       setBackend("File"); // add by liudong
 
 //    database.connect(db_url.c_str(),db_name.c_str(), db_user.c_str(), db_password.c_str());
 
     //if (database.connected()) {
     //      return 1;
     //}
 
     //if (database.connect(db_name.c_str(), db_url.c_str(), db_user.c_str(), db_password.c_str())) {
     //      result = 1;
     //      stringstream o;
     //      o << "EmcCalibIO::connectDB(): successfully connected to database " << db_url << "/"
     //                  << db_name << " as user " << db_user;
     //      logger(3, o);
     //} else {
     //      stringstream o;
     //      o << "EmcCalibIO::connectDB(): unable to connect to database " << db_url << "/" << db_name
     //                  << " as user " << db_user;
     //      logger(1, o);
     //}
 
       //if (result == 1)
             result = loadXtalDef();
 
       return result;
 }
 
 void EmcCalibIO::setBackend(const char* bkend) {
       TString opts(bkend);
       backend = 2;
       if (opts.Contains("File")) {
             backend = 1;
       } else if (opts.Contains("MySQL")) {
             backend = 2;
       }
 }
 
 void EmcCalibIO::setTimeInfo(bool b) {
       _timeInfo = b;
       if (_timeInfo) {
             struct timeval tv;
             gettimeofday(&tv, 0);
             time_vec.assign(1, tv);
       }
       return;
 }
 
 void EmcCalibIO::timeOutput(int i = 0, const char *where = "") {
       if (!_timeInfo)
             return;
 
       struct timeval tv;
       gettimeofday(&tv, 0);
       int n = time_vec.size();
       float sec = 0;
       string out;
 
       switch (i) {
       case (1):
             sec = tv.tv_sec - time_vec[n - 1].tv_sec;
             out = " run time (dT) = ";
             break;
       case (0):
       default:
             sec = tv.tv_sec - time_vec[0].tv_sec;
             out = " total run time = ";
             break;
       }
 
       int min = (int) (sec / 60.);
       if (min > 60.) {
             int hour = (int) (sec / (60. * 60.));
             min -= (hour * 60);
             sec -= ((min * 60) + (hour * 60 * 60));
 
             stringstream o;
             o << where << out << hour << " h " << min << " min " << sec << " sec";
             logger(4, o);
       } else {
             if (sec > 60) {
                   sec -= (min * 60);
             }
             stringstream o;
             o << where << out << min << " min " << sec << " sec";
             logger(4, o);
       }
 
       time_vec.push_back(tv);
 
       return;
 }
 
 void EmcCalibIO::logger(int status, stringstream &o) {
       logger(status, o.str().data());
       o << endl;
       return;
 }
 
 //status: 1=fatal error; 2=error; 3=trace; 4=internal status
 void EmcCalibIO::logger(int status, const char *o) {
       if (!_verbose && o != 0)
             return;
       string s_status;
       switch (status) {
       case 1:
             s_status = "fatal Error";
             break;
       case 2:
             s_status = "Error";
             break;
       case 3:
             s_status = "Trace";
             break;
       case 4:
             s_status = "Status";
             break;
       }
       if (status == 3 || status == 4) {
             std::cout << o << " (" << s_status << ")" << endl;
       }
       if (status == 1 || status == 2) {
             std::cerr << o << " (" << s_status << ")" << endl;
       }
       if (status == 1) {
             exit(1);
       }
       return;
 }
 
 int EmcCalibIO::loadCalibMap(map<const int, cal_store> &calib_map) {
       int result = 0;
       switch (backend) {
       case (1):
             result = loadCMFile(calib_map);
             break;
       case (2):
             result = loadCMSQL(calib_map);
             break;
       }

       if (result == 0) {
             std::map<const int, xtal_def>::iterator iter_Def;
             int cpnr, det;
             for (iter_Def = _xtalDef.begin(); iter_Def != _xtalDef.end(); iter_Def++) { 
	        cpnr = iter_Def->first;
		det = iter_Def->second.subdetector;
		cal_store store;
		data tmpdata(1.0, 1.0);
		store.total = tmpdata;
		store.data_vec.push_back(tmpdata);
		store.iterations = 0;
                calib_map[cpnr] = store;
	     }
	     result = 3;
       }

       return result;
 }
 
 int EmcCalibIO::saveCalibMap(map<const int, cal_store> &calib_map) {
       int result = 0;
       switch (backend) {
       case (1):
             result = saveCMFile(calib_map);
             break;
       case (2):
             result = saveCMSQL(calib_map);
             break;
       }
 
       return result;
 }
 
 int EmcCalibIO::loadCMFile(std::map<const int, cal_store> &calib_map) {
       std::ifstream ifs(_EmcCalibrationFile);
       if (ifs.fail()) {
             //cal_store store;
	     //data d(1.0, 1.0);
	     //store.total = d;
	     //store.data_vec.push_back(d);
	     //store.iterations = 0;
             //
	     //std::vector<int> xtal_index;
	     ////getXtalIndex(_subdetector, xtal_index);
	     //std::map<const int, xtal_def>::iterator it;
	     //for (it=_xtalDef.begin(); it!=_xtalDef.end(); it++) {
             //  int cpnr = it->first;
	     //  calib_map[cpnr] = store;
	     //}

             //return 0;
             return 0;
       }
 
       stringstream o;
       o << "EmcCalibIO::load(): " << _EmcCalibrationFile;
       logger(3, o);
       boost::archive::xml_iarchive in_a(ifs);
 
       std::map<const int, data> calib_map_data;

       //       read class state from archive;
       in_a >> BOOST_SERIALIZATION_NVP(calib_map_data);
       o << "EmcCalibIO::load(): " << calib_map_data.size() << " entries successfully read in from "
                   << _EmcCalibrationFile;
       logger(3, o);

        map<const int, data>::iterator it;
       for (it = calib_map_data.begin(); it != calib_map_data.end(); it++) {
             cal_store store;
	     store.total = it->second;
	     store.iterations = 0;
	     store.data_vec.push_back(it->second);
	     
	     calib_map[it->first] = store;


           //val = (*it).second.total;
           //if (isnan(val.error)) { //for serialization: set NaN results of error to -1
           //      val.error = -1.;
           //}
           //if (!isnan(val.value)) { //save only if value is not NaN
           //      tmp_map[(*it).first] = val;
           //}
       }
 


       return 1;
 }
 
 int EmcCalibIO::saveCMFile(std::map<const int, cal_store> &calib_map) {
       map<const int, data> tmp_map;
       data val(1, 1);
 
       map<const int, cal_store>::iterator it;
       for (it = calib_map.begin(); it != calib_map.end(); it++) {
             val = (*it).second.total;
             if (isnan(val.error)) { //for serialization: set NaN results of error to -1
                   val.error = -1.;
             }
             if (!isnan(val.value)) { //save only if value is not NaN
                   tmp_map[(*it).first] = val;
             }
       }
 
       std::ofstream ofs(_EmcCalibrationFile);
       assert(ofs.good());
       boost::archive::xml_oarchive out_a(ofs);
       //    out_a << BOOST_SERIALIZATION_NVP(tmp_map);
       out_a << boost::serialization::make_nvp("calib_map", tmp_map);
 
       stringstream o;
       o << "EmcCalibIO::saveCMFile(): " << calib_map.size() << " entries successfully saved in "
                   << _EmcCalibrationFile;
       logger(3, o);
       return 1;
 }
 
 int EmcCalibIO::setupCalibMap(std::map<const int, xtal_def> &def) {
       int result = 0;
// TODO
/*
       mysqlpp::StoreQueryResult res;
       int cpnr, subdet;
       int edge;
       float default_calibration = 1.;
 
       std::map<const int, xtal_def>::iterator it;
 
       for (it = def.begin(); it != def.end(); it++) {
 
             cpnr = it->first;
             subdet = it->second.subdetector;
             edge = it->second.isAtEdge;
 
             mysqlpp::Query query = database.query();
             query << "SELECT calibration FROM " << cm_table_name << " WHERE cpnr = " << cpnr;
             res = query.store();
 
             mysqlpp::Query query1 = database.query();
             if (res) {
                   if (res.size() == 0) {
                         query1 << "INSERT INTO " << cm_table_name
                                     << " (cpnr,calibration,subDetector,atEdge) VALUES (" << cpnr << ","
                                     << default_calibration << "," << subdet << "," << edge << ")";
 
                   } else {
                         query1 << "UPDATE " << cm_table_name << " SET calibration=" << default_calibration
                                     << ",subDetector=" << subdet << ",atEdge=" << edge << " WHERE cpnr = "
                                     << cpnr;
                   }
                   if (cpnr == 6011109) {
                         cout << "EmcCalibIO::setupCalibMap for cpnr=6011109 : " << query1.str().data()
                                     << endl;
                   }
                   mysqlpp::SimpleResult sres = query1.execute();
                   if (sres) {
                         result = 1;
                   }
             }
       }
 
 //    stringstream sql2;
 //
 //    sql2 << "CREATE INDEX cv_index ON " << cm_table_name << " (cpnr)";
 //    mysqlpp::Query query = database.query(sql2.str().data());
 
       if (result == 1) {
             stringstream o;
             o << "EmcCalibIO::setupCalibMap(): setup of database successful";
             logger(3, o);
       }
*/ 
       return result;
 }
 
 int EmcCalibIO::is_centralXtal(const int cpnr) {
       int result = 0;
 
       if (_xtalDef.size() == 0) {
             logger(2, "EmcCalibIO::is_centralXtal(): _xtalDef map not filled");
             return result;
       }
 
       int at_Edge = _xtalDef[cpnr].isAtEdge;
 
       if (at_Edge == 1) {
             result = 0;
       }
       if (at_Edge == 0) {
             result = 1;
       }
 
       return result;
 }
 
 int EmcCalibIO::loadXtalDef() {
       int result = 0;
 
       int cpnr;
 
       _xtalDef.clear();

       std::ifstream ifs(_edgeXtalFile);
       if (ifs.fail())
             return 0;

       boost::archive::xml_iarchive in_a(ifs); 
       in_a >> BOOST_SERIALIZATION_NVP(_xtalDef);

 //    _xtalIdx.clear();
/*
       mysqlpp::StoreQueryResult res;
 
       mysqlpp::Query query = database.query();
       query << "SELECT * FROM " << cm_table_name << " WHERE cpnr > " << 0;
       res = query.store();
 
       int nrows = res.size();
 
       for (int i = 0; i < nrows; i++) {
 
 //          cpnr = atoi(res[i]["cpnr"]);
             cpnr = boost::lexical_cast<int>(res[i]["cpnr"]);
 //          _xtalIdx.push_back(cpnr);
 
             xtal_def def;
             def.subdetector = boost::lexical_cast<int>(res[i]["subDetector"]);
 
             def.isAtEdge = boost::lexical_cast<int>(res[i]["atEdge"]);
 
             _xtalDef[cpnr] = def;
 
       }
 
       if (nrows > 0)
             result = 1;
*/ 
       return result;
 }
 
 int EmcCalibIO::getXtalIndex(int detector, std::vector<int> &idx) {
       int result = 0;
 
       std::map<const int, xtal_def>::iterator iter_Def;
       int cpnr, det;
 
       stringstream o1;
       o1 << "EmcCalibIO::getXtalIndex(): xdef isze "<<_xtalDef.size() ;
       logger(3, o1);
       for (iter_Def = _xtalDef.begin(); iter_Def != _xtalDef.end(); iter_Def++) {
             cpnr = iter_Def->first;
             det = iter_Def->second.subdetector;
             if (detector == all) {
                   idx.push_back(cpnr);
             } else {
                   if (detector == det) {
                         idx.push_back(cpnr);
                   }
             }
       }
 
       if (idx.size() > 0) {
             result = 1;
       }
       return result;
 }
 
 //int EmcCalibIO::getXtalIndex(std::vector<int> &idx) {
 //    int result = 0;
 //    idx = _xtalIdx;
 //    if (idx.size() > 0) {
 //          result = 1;
 //    }
 //    return result;
 //}
 
 float EmcCalibIO::getCorrSQL(int cpnr) {
       float result = 1.;
       
       // TODO
       /*
       mysqlpp::StoreQueryResult res;
 
       mysqlpp::Query query = database.query();
       query << "SELECT calibration FROM " << cm_table_name << " WHERE cpnr = " << cpnr;
       res = query.store();
 
       result = boost::lexical_cast<float>(res[0][0]);
 */
       return result;
 }
 
 int EmcCalibIO::getDetector(int cpnr) {
       int result = 0;
 
       result = _xtalDef[cpnr].subdetector;
 
       return result;
 }
 
 int EmcCalibIO::loadCMSQL(std::map<const int, cal_store> &calib_map) {
       int result = 0;
       /* TODO
       mysqlpp::StoreQueryResult res;
       int cpnr;
       int n = 0;
       cal_store cal;
 
       mysqlpp::Query query = database.query();
       query << "SELECT cpnr FROM " << cm_table_name << " WHERE cpnr > " << 0;
       res = query.store();
       //while ((row = res->Next())) {
 
 //    cpnr = atoi(res[0][0]);
       for (int i = 0; i < res.size(); i++) {
 
             cpnr = boost::lexical_cast<int>(res[i][0]);
 
             stringstream ifs;
             mysqlpp::StoreQueryResult res1;
 
             mysqlpp::Query query1 = database.query();
             query1 << "SELECT cal_store FROM " << cm_table_name << " WHERE cpnr = " << cpnr;
             res1 = query1.store();
             ifs << res1[0][0];
 
             try {
                   boost::archive::text_iarchive in_a(ifs);
                   in_a & BOOST_SERIALIZATION_NVP(cal);
 
                   calib_map[cpnr] = cal;
                   n++;
             } catch (boost::archive::archive_exception&) {
                   stringstream o;
                   o << "EmcCalibIO::loadCMSQL(): boost::archive::archive_exception stream error at cpnr="
                               << cpnr << ". Can't load  calibration values.";
                   logger(2, o);
                   result = 0;
             } catch (std::bad_alloc&) {
                   stringstream o;
                   o << "EmcCalibIO::loadCMSQL(): std::bad_alloc stream error at cpnr=" << cpnr;
                   logger(2, o);
                   result = 0;
             }
       }
 
       if (n > 0) {
             result = 1;
 
             stringstream o;
             o << "EmcCalibIO::loadCMSQL(): Map with " << n
                         << " calibration values successfully loaded from database";
             logger(3, o);
       }
*/ 
       return result;
 }
 
 int EmcCalibIO::saveCMSQL(std::map<const int, cal_store> &calib_map) {
       int result = 0;
 
       std::map<const int, cal_store>::iterator it_map;
 
       for (it_map = calib_map.begin(); it_map != calib_map.end(); it_map++) {
             result = updateCMSQL((*it_map).first, (*it_map).second);
             if (result == 0) {
                   stringstream o;
                   o << "EmcCalibIO::saveCMSQL(): error at cpnr " << (*it_map).first
                               << " can not be written to database";
                   logger(2, o);
                   return result;
             }
       }
 
       stringstream o;
       o << "EmcCalibIO::saveCMSQL(): Map with calibration constants successfully saved to database";
       logger(3, o);
 
       return result;
 }
 
 int EmcCalibIO::updateCMSQL(const int cpnr, cal_store &store) {
       int result = 0;
 
       if (store.total.value != store.total.value) { 
             return result;
       }
 
       stringstream ofs;
       boost::archive::text_oarchive out_a(ofs);
 
       out_a & BOOST_SERIALIZATION_NVP(store);
 /* TODO
       mysqlpp::Query query = database.query();
       query << "UPDATE " << cm_table_name << " SET calibration=" << store.total.value << ",cal_store="
                   << "'" << ofs.str().data() << "' WHERE cpnr=" << cpnr;
       mysqlpp::SimpleResult sres = query.execute();
       if (sres) {
             result = 1;
       }
*/
       return result;
 }
 
 int EmcCalibIO::clearCalibMap(int subDet) {
       int result = 0;
       cal_store store;
       int n = 0;
 
       std::map<const int, xtal_def>::iterator iter_Def;
       int cpnr;
 
       for (iter_Def = _xtalDef.begin(); iter_Def != _xtalDef.end(); iter_Def++) {
             if (subDet >= all) {
                   cpnr = iter_Def->first;
                   store.clear();
                   result = updateCMSQL(cpnr, store);
                   n++;
             } else if (subDet == iter_Def->second.subdetector) {
                   cpnr = iter_Def->first;
                   store.clear();
                   result = updateCMSQL(cpnr, store);
                   n++;
             }
       }
       if (n > 0) {
             result = 1;
       }
 
       if (result == 1) {
             stringstream o;
             o
                         << "EmcCalibIO::clearCalibMap(): Map with calibration constants cleared for subdetector = "
                         << subDet << " (" << n << " total entries).";
             logger(3, o);
       }
 
       return result;
 }
 
 int EmcCalibIO::loadFitVec(int cpnr, std::vector<entry> &fit_vec) {
       int result = 0;
       result = loadFitVec(true, cpnr, fit_vec);
       return result;
 }
 
 int EmcCalibIO::loadFitVec(bool is_init, int cpnr, std::vector<entry> &fit_vec) {
       int result = 0;
       int col = 0;

       string db_table = hist_table_name;
       //if (!is_init) 
       if (data_cached)
       {
             //db_table = cache_table_name;
             if (histo_cache->find(cpnr)!=histo_cache->end()) {
	          fit_vec.assign((*histo_cache)[cpnr].begin(), (*histo_cache)[cpnr].end());
		  stringstream o;
		  o << "EmcCalibIO::loadFitVec(): from cached vector for cpnr "<<cpnr ;
		  logger(3, o);
 
		  return 3;
	     }
	     else return 0;
	     //else db_table = cache_table_name;
       }

       result = cacheFromRoot(db_table.c_str());

       if (result==1) result = loadFitVec(false, cpnr, fit_vec);
       else result = 0;

       //return result;
/*
       mysqlpp::Query query = database.query();
       query << "SELECT * FROM " << db_table << " WHERE cpnr=" << cpnr;
 
 //    cout << "EmcCalibIO::loadFitVec query= " << query.str().data() << endl;
 
       mysqlpp::StoreQueryResult res = query.store();
 
       if (res.size() == 0) {
             stringstream o;
             o << "EmcCalibIO::loadFitVec(): no data for cpnr=" << cpnr << " stored. Fill in first.";
             logger(3, o);
             result = 2;
             return result;
       }
 
       col = boost::lexical_cast<int>(res[0]["columns"]);
       if (col >= _max_columns) {
             col = _max_columns - 1;
       }
 
       for (int i_try = 1; i_try <= _nr_trials; i_try++) {
 
             std::vector<entry> vec_data;
             fit_vec.clear();
 
             for (int i = 0; i <= col; i++) {
  
                   stringstream ss_column;
                   ss_column << "entries_" << i;
 
                   std::vector<entry> vec;
                   stringstream ifs;
                   ifs << res[0][ss_column.str().data()];
 
                   try {
                         boost::archive::binary_iarchive in_a(ifs);
                         in_a & vec;
                         vec_data.insert(vec_data.begin(), vec.begin(), vec.end());
                         result = 1;
                   } catch (boost::archive::archive_exception&) {
                         stringstream o;
                         o
                                     << "EmcCalibIO::loadFitVec(): boost::archive::archive_exception stream error at cpnr="
                                     << cpnr << ". Can't load fit_vec with data to fit.";
                         logger(2, o);
                         result = 0;
                   } catch (std::bad_alloc&) {
                         stringstream o;
                         o << "EmcCalibIO::loadFitVec(): std::bad_alloc stream error at cpnr=" << cpnr;
                         logger(2, o);
                         result = 0;
                   }
             }
             if (result == 1) {
                   fit_vec = vec_data;
                   stringstream o;
                   o << "EmcCalibIO::loadFitVec(): successfully loaded for cpnr=" << cpnr << " with "
                               << vec_data.size() << " entries";
                   logger(3, o);
                   break;
             }
       }
*/ 
       return result;
 }
 
 int EmcCalibIO::cacheFromRoot(const char* rootfile) {
 
       int result = 0;

       // from root file
       // cache data to memory
       int cpnr;
       entry* m_entry = new entry;
       TFile* file = new TFile(rootfile);
       if (file==0) return -1;
       TTree* tree = (TTree*)file->Get("candidates");
       if (tree==0) return -2;
       TEventList* m_event_list;
       
       //std::vector<entry> fit_vec;
       //std::map<const int, xtal_def>::iterator it = _xtalDef.begin();

       //for (; it!=_xtalDef.end(); it++) {
               //int cpnr = it->first;
	       //TString cut = Form("cpnr == %d", cpnr);
	       //int nevt = tree->Draw(">>elist",cut,"",1000000, 0);
	       //result = nevt;
	       //m_event_list = (TEventList*)gDirectory->Get("elist");
	       tree->SetBranchAddress("entry", &m_entry);
	       tree->SetBranchAddress("cpnr", &cpnr);
	       int nevt = tree->GetEntries();
	       //nevt = nevt>1000000?1000000:nevt;

	       //fit_vec.clear();

	       stringstream o1;
	       o1.clear();
	       o1 << "EmcCalibIO::loadFitVec(): from root file, " << nevt <<" events in list for cpnr "<<cpnr ;
	       logger(3, o1);

	       for (int ievt = 0; ievt<nevt; ievt++) {
		       //int treei = m_event_list->GetEntry(ievt);
		       //tree->GetEntry(treei);
		       if (ievt%100000==0) {
			       stringstream o;
			       o << "EmcCalibIO::cacheFromRoot(): " << ievt <<"/"<<nevt;
			       logger(3, o);
		       
		       } 
		       tree->GetEntry(ievt);
		       
		       //cout<<"ievt "<<ievt<<", ptr="<<m_entry<<", mgg "<<m_entry->m_gg<<endl;
		       if (histo_cache->find(cpnr)==histo_cache->end()) (*histo_cache)[cpnr] = std::vector<entry>();
		       //fit_vec.push_back(*m_entry);
		       (*histo_cache)[cpnr].push_back(*m_entry);
	       }

	       //cacheFitVec(cpnr, fit_vec);

       //}
       data_cached = true;

       delete m_entry;
       file->Close();

       return 1;
 }


 void EmcCalibIO::cacheDataSQL(const int cpnr, entry &eintrag) {
       (*histo_cache)[cpnr].push_back(eintrag);
       return;
 }
 
 int EmcCalibIO::dumpDataSQL() {
       int result = 0;
       int size = histo_cache->size();
 
       if (size == 0) {
             logger(2,
                         "EmcCalibIO::dumpDataSQL(): cache not filled, use cacheDataSQL(int, entry) first.");
             return result;
       }
 
       logger(3, "EmcCalibIO::dumpDataSQL(): Saving cached data to database.");
       timeOutput(1, "start saving at");
 
       std::map<const int, std::vector<entry> >::iterator it_map;
 
       int pos = 0;
       bool b_out = false;
 
       for (it_map = histo_cache->begin(); it_map != histo_cache->end(); it_map++) {
             int cpnr = it_map->first;
 
             result = saveFitVec(cpnr, it_map->second);
 
             int prog = (pos * 100) / size;
             if ((prog % 5) == 0) {
                   if (b_out) {
                         stringstream o;
                         o << "EmcCalibIO::dumpDataSQL(): " << prog << "% saved.";
                         logger(4, o);
                         b_out = false;
                   }
             } else {
                   b_out = true;
             }
 
             pos++;
       }
 
       logger(4, "EmcCalibIO::dumpDataSQL(): 100% saved.");
       timeOutput(1, "saved everything at");
       return result;
 }
 
 int EmcCalibIO::saveFitVec(int cpnr, std::vector<entry> &fit_vec) {
       int result = 0;
       //int cpnr;
       //TFile* fout = new TFile("calibratedSample.root","recreate");
       int size = fit_vec.size();
       m_cpnr = cpnr;
       for (int i=0; i<size; i++) {
	    m_entry = &fit_vec.at(i);
	    tpi0->Fill();
       }
       //tpi0->Write();
 
/*       
       mysqlpp::StoreQueryResult res;
       int size = fit_vec.size();
       int size1 = size;
       std::vector<std::vector<entry> > tmp_entries;
       mysqlpp::Query query = database.query();
       int col = 0;
 
       tmp_entries.push_back((fit_vec));
       std::vector<entry> in_vec;
       int i_fv = loadFitVec(cpnr, in_vec);
       switch (i_fv) {
       case 0:
             return result;
             break;
       case 2: 
             if ((size) > (_max_columns * _ent_p_column)) { 
                   stringstream o;
                   o << "EmcCalibIO::saveFitVec(): available storage space exceeded, at cpnr=" << cpnr
                               << " with " << size << " entries. Further events are rejected.";
                   logger(3, o);
                   result = 1;
                   return result;
             }
             while (size1 > _ent_p_column) {
                   std::vector<entry> vec;
                   vec.assign(tmp_entries[col].begin() + _ent_p_column, tmp_entries[col].end());
                   tmp_entries[col].erase(tmp_entries[col].begin() + _ent_p_column,
                               tmp_entries[col].end());
                   tmp_entries.push_back(vec);
                   size1 = vec.size();
                   col++;
             }
             query << "INSERT INTO " << hist_table_name << " SET cpnr=" << cpnr << ", columns=" << col;
             for (int i = 0; i <= col; i++) {
                   try {
                         stringstream ofs;
                         boost::archive::binary_oarchive out_a(ofs);
                         out_a << tmp_entries[i];
                         query << ", entries_" << i << "=" << quote << ofs.str();
                   } catch (boost::archive::archive_exception&) {
                         stringstream o;
                         o
                                     << "EmcCalibIO::saveFitVec(): at insert: boost::archive::archive_exception stream error at cpnr="
                                     << cpnr;
                         logger(2, o);
                         result = 0;
                   }
             }
             break;
       case 1: 
             size += in_vec.size();
             if ((size) > (_max_columns * _ent_p_column)) { 
                   stringstream o;
                   o << "EmcCalibIO::saveFitVec(): available storage space exceeded, at cpnr=" << cpnr
                               << " with " << size << " entries. Further events are rejected.";
                   logger(3, o);
                   result = 1;
                   return result;
             }
             tmp_entries[0].insert(tmp_entries[0].begin(), in_vec.begin(), in_vec.end());
             size1 = size;
             while (size1 > _ent_p_column) {
                   std::vector<entry> vec;
                   vec.assign(tmp_entries[col].begin() + _ent_p_column, tmp_entries[col].end());
                   tmp_entries[col].erase(tmp_entries[col].begin() + _ent_p_column,
                               tmp_entries[col].end());
                   tmp_entries.push_back(vec);
                   size1 = vec.size();
                   col++;
             }
             query << "UPDATE " << hist_table_name << " SET columns=" << col;
             for (int i = 0; i <= col; i++) {
                   try {
                         stringstream ofs;
                         boost::archive::binary_oarchive out_a(ofs);
                         out_a << tmp_entries[i];
                         query << ", entries_" << i << "=" << quote << ofs.str();
                   } catch (boost::archive::archive_exception&) {
                         stringstream o;
                         o
                                     << "EmcCalibIO::saveFitVec(): at update boost::archive::archive_exception stream error at cpnr="
                                     << cpnr;
                         logger(2, o);
                         result = 0;
                   }
             }
             query << " WHERE cpnr=" << cpnr;
             break;
       } 
 
       try {
             bool bres = query.execute();
             std::vector<entry> fit_vec_tmp;
             if (bres && loadFitVec(cpnr, fit_vec_tmp) && fit_vec_tmp.size() == size) {
                   result = 1;
             }
       } catch (const Exception& er) {
             logger(2, "EmcCalibIO::saveFitVec(): Sleep 1sec");
             sleep(1); 
       }
 
       if (result == 1) {
             stringstream o;
             o << "EmcCalibIO::saveFitVec(): successfully ";
             switch (i_fv) {
             case 2:
                   o << "inserted ";
                   break;
             case 1:
                   o << "updated ";
                   break;
             }
             o << "for cpnr=" << cpnr << " with " << size << " entries.";
             logger(3, o);
       }
*/
       return result;
 }
 
 int EmcCalibIO::cacheFitVec(int cpnr, std::vector<entry> &fit_vec) {
       int result = 0;

       if (histo_cache->find(cpnr)==histo_cache->end()) (*histo_cache)[cpnr] = std::vector<entry>();
       (*histo_cache)[cpnr].assign(fit_vec.begin(), fit_vec.end());

 /*
       mysqlpp::StoreQueryResult res;
       int size = fit_vec.size();
       int size1 = size;
       std::vector<std::vector<entry> > tmp_entries;
       mysqlpp::Query query = database.query();
       int col = 0;
 
       if ((size) > (_max_columns * _ent_p_column)) { 
             stringstream o;
             o << "EmcCalibIO::cacheFitVec(): available storage space exceeded, at cpnr=" << cpnr
                         << " with " << size << " entries. Further events are rejected.";
             logger(3, o);
             result = 1;
             return result;
       }
 
       result = deleteDataSQL(cache_table_name, cpnr); 
       tmp_entries.push_back(fit_vec);
 
       while (size1 > _ent_p_column) {
             std::vector<entry> vec;
             vec.assign(tmp_entries[col].begin() + _ent_p_column, tmp_entries[col].end());
             tmp_entries[col].erase(tmp_entries[col].begin() + _ent_p_column, tmp_entries[col].end());
             tmp_entries.push_back(vec);
             size1 = vec.size();
             col++;
       }
       query << "INSERT INTO " << cache_table_name << " SET cpnr=" << cpnr << ", columns=" << col;
       for (int i = 0; i <= col; i++) {
             try {
                   stringstream ofs;
                   boost::archive::binary_oarchive out_a(ofs);
                   out_a << tmp_entries[i];
                   query << ", entries_" << i << "=" << quote << ofs.str();
             } catch (boost::archive::archive_exception&) {
                   stringstream o;
                   o
                               << "EmcCalibIO::cacheFitVec(): at insert: boost::archive::archive_exception stream error at cpnr="
                               << cpnr;
                   logger(2, o);
                   result = 0;
             } catch (std::bad_alloc&) {
                   stringstream o;
                   o << "EmcCalibIO::cacheFitVec():840 at insert: std::bad_alloc stream error at cpnr="
                               << cpnr;
                   logger(2, o);
                   result = 0;
                   return result;
             }
       }
       try {
             bool bres = query.execute();
             std::vector<entry> fit_vec_tmp;
             if (bres && loadFitVec(false, cpnr, fit_vec_tmp) && fit_vec_tmp.size() == size) {
                   result = 1;
             }
       } catch (const Exception& er) {
             logger(2, "EmcCalibIO::cacheFitVec(): Sleep 1sec");
 //          sleep(1); //give it some time to relax
             result = 0;
             return result;
       } catch (std::bad_alloc&) {
             stringstream o;
             o << "EmcCalibIO::cacheFitVec():859 at insert: std::bad_alloc stream error at cpnr="
                         << cpnr;
             logger(2, o);
             result = 0;
             return result;
       }
 
       if (result == 1) {
             stringstream o;
             o << "EmcCalibIO::cacheFitVec(): cache successfully updated for cpnr=" << cpnr << " with "
                         << size << " entries.";
             logger(3, o);
       }
*/ 
       return result;
 }
 
 int EmcCalibIO::deleteDataSQL(string &db_table) {
       int result = 0;
/*
       mysqlpp::Query query = database.query();
       query << "DELETE FROM " << db_table;
       mysqlpp::SimpleResult sres = query.execute();
       if (sres) {
             result = 1;
 
             stringstream o;
             o << "EmcCalibIO::deleteDataSQL(): content of table " << db_table << " deleted.";
             logger(3, o);
       }
*/ 
       return result;
 }
 
 int EmcCalibIO::deleteCache() {
       int result = 0;
       result = deleteDataSQL(cache_table_name);
       return result;
 }
 
 int EmcCalibIO::deleteDataSQL(string &db_table, int cpnr) {
       int result = 0;
       
/*       
       mysqlpp::SimpleResult sres;
 
       mysqlpp::Query query = database.query();
 //    query << "DELETE FROM " << hist_table_name << " WHERE cpnr = " << cpnr;
       query << "DELETE FROM " << db_table << " WHERE cpnr = " << cpnr;
 
       for (int i = 0; i < _nr_trials; i++) {
             try {
                   sres = query.execute();
                   break;
             } catch (const Exception& er) {
                   logger(2, "EmcCalibIO::deleteDataSQL(): Sleep 1sec");
                   sleep(1); 
             }
       }
 
       if (sres) {
             result = 1;
 
             stringstream o;
             o << "EmcCalibIO::deleteDataSQL(): row of cpnr=" << cpnr << " in table " << db_table
                         << " removed.";
             logger(3, o);
       }
*/
       return result;
 }
 
