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
 //
 #ifndef EMCCALIBIO_HH
 #define EMCCALIBIO_HH
 
 #include <vector>
 #include <map>
 #include <string>
 
 //#include "EmcCalibContainer.hh"
 #include "EmcCalibDataModel.hh"
 
 #include "TSQLServer.h"
 
 //#include <mysql++/mysql++.h>
 class TTree;
 
 class EmcCalibIO {
 
       //--------------------
       // Instance Members --
       //--------------------
 
 public:
 
       EmcCalibIO();
       EmcCalibIO(const char* options);
 
       ~EmcCalibIO();
 
 
       void setOptions(const char* options);
 
       void setBackend(const char* bkend);
 
       void setVerbose(bool b) {
             _verbose = b;
       }
 
       void setTimeInfo(bool b);
       void setEmcCalibrationFile(const char *file) {
             _EmcCalibrationFile = file;
       }
 
       void timeOutput(int i, const char *where);
 
       void setMaxColumns(int col) {
             _max_columns = col;
       }
 
       void setDBname(const char* name = "PndECal") {
             db_name = std::string(name);
       }
 
       void setDBhost(const char* url = "localhost") {
             db_url = std::string(url);
       }
 
       void setDBuser(const char* user = "emccalusr") {
             db_user = std::string(user);
       }
 
       void setDBpassword(const char* password = "") {
             db_password = std::string(password);
       }
 
       int connectDB();
 
       int loadCalibMap(std::map<const int, cal_store> &calib_map);
 
       int saveCalibMap(std::map<const int, cal_store> &calib_map);
 
       int setupCalibMap(std::map<const int, xtal_def> &def);
 
       int clearCalibMap(int subDet = all);
 
       int is_centralXtal(const int cpnr);
 
       void logger(int status, const char *o);
       void logger(int status, std::stringstream &o);
 
       void printStatus();
 
       int loadFitVec(int cpnr, std::vector<entry> &fit_vec);
       int loadFitVec(bool is_init, int cpnr, std::vector<entry> &fit_vec); 
 
       int cacheFitVec(int cpnr, std::vector<entry> &fit_vec);
 
       //    int deleteFitVec(int cpnr);
 
       int saveFitVec(int cpnr, std::vector<entry> &fit_vec);
 
       void cacheDataSQL(const int cpnr, entry &eintrag);
 
       int dumpDataSQL();
 
       int deleteDataSQL(std::string &db_table);
 
       int deleteDataSQL(std::string &db_table, int cpnr);
 
       int deleteCache();
 
       int getXtalIndex(std::vector<int> &idx);
 
       int getXtalIndex(int detector, std::vector<int> &idx);
 
       int getDetector(int cpnr);
 
       int getMaxEntries() {
             return (_max_columns * _ent_p_column);
       }
 
       float getCorrSQL(int cpnr);
 
 private:
       void init();
 
       std::vector<int> _edge_xtal;
 
       const char* _edgeXtalFile;
 
       const char* _EmcCalibrationFile;
 
 //    std::vector<int> _xtalIdx;
 
       std::map<const int, xtal_def> _xtalDef;
 
       bool _timeInfo;
       bool _verbose;
       int _max_columns;
       int _ent_p_column;
       int _nr_trials;
 
       std::string cm_table_name;
 
       std::string hist_table_name;
 
       std::string cache_table_name;
 
       std::string db_url;
 
       std::string db_name;
 
       std::string db_user;
 
       std::string db_password;
 
       std::map<const int, std::vector<entry> > *histo_cache;
 
       int backend;
 
       int loadXtalDef();
 
       int loadCMFile(std::map<const int, cal_store> &calib_map);
 
       int saveCMFile(std::map<const int, cal_store> &calib_map);
 
       int loadCMSQL(std::map<const int, cal_store> &calib_map);
 
       int saveCMSQL(std::map<const int, cal_store> &calib_map);
 
       int updateCMSQL(const int cpnr, cal_store &store);
 
       int loadFVSQL(int cpnr, std::vector<entry> &fit_vec);
 
       int saveFVSQL(int cpnr, std::vector<entry> &fit_vec);

       int cacheFromRoot(const char* rootfile);
 
       //mysqlpp::Connection database;
 
       std::vector<struct timeval> time_vec;

       TFile* db_out;
       TTree* tpi0;
       entry* m_entry;
       int m_cpnr;
       bool data_cached;
 
 };
 
 #endif
