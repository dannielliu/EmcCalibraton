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
 
 #ifndef EMCCALIBCONTAINER_HH
 #define EMCCALIBCONTAINER_HH
 
 #include <TROOT.h>
 #include <map>
 #include <vector>
 #include <string>
 
 #include "TVector3.h"
 
 #include <boost/archive/text_oarchive.hpp>
 #include <boost/archive/text_iarchive.hpp>
 
 #include <boost/serialization/vector.hpp>
 //
 //#include <boost/archive/xml_iarchive.hpp>
 //#include <boost/archive/xml_oarchive.hpp>
 #include <boost/shared_ptr.hpp>
 
 enum subdetectors_t {
       bkw = 1, barrel = 2, fwd = 3, slk = 4, all
 };
 
 class data {
       friend class boost::serialization::access;
       template<class Archive>
       void serialize(Archive & ar, const unsigned int version) {
             ar & BOOST_SERIALIZATION_NVP(value);
             ar & BOOST_SERIALIZATION_NVP(error);
             //          ar & value;
             //          ar & error;
       }
 public:
       float value;
       float error;
       float rel_error() {
             return error / value;
       }
 //    data() {
 //    }
       data(float val = 1., float err = 1.) :
                   value(val), error(err) {
       }
 };
 
 class xtal_def {
       friend class boost::serialization::access;
       template<class Archive>
       void serialize(Archive & ar, const unsigned int version) {
             ar & BOOST_SERIALIZATION_NVP(isAtEdge) & BOOST_SERIALIZATION_NVP(subdetector);
       }
 public:
       xtal_def() {
       }
       xtal_def(int edg, int sd) :
                   isAtEdge(edg), subdetector(sd) {
       }
       int isAtEdge; 
       int subdetector; 
 //    float calibration;
 };
 
 class cal_store {
       friend class boost::serialization::access;
       template<class Archive>
       void serialize(Archive & ar, const unsigned int version) {
 //          ar & BOOST_SERIALIZATION_NVP(total) & BOOST_SERIALIZATION_NVP(data_vec); //data_vec is not needed to be persistent
             ar & BOOST_SERIALIZATION_NVP(total);
             iterations = 0;
       }
 public:
       data total;
       std::vector<data> data_vec;
       int iterations;
       cal_store() {
       }
       ;
       cal_store(data val) :
                   total(val) {
       }
       cal_store(std::vector<data> iter) :
                   data_vec(iter) {
       }
       cal_store(data val, std::vector<data> iter) :
                   total(val), data_vec(iter) {
       }
       int size() {
             return data_vec.size();
       }
       void push_back(const data val) {
             data_vec.push_back(val);
             iterations++;
       }
       data &back() {
             return data_vec.back();
       }
       void clear() {
             data_vec.clear();
             total = data(1., 1.);
             iterations = 0;
       }
 
 };
 
 class parameter {
       friend class boost::serialization::access;
       template<class Archive>
       void serialize(Archive & ar, const unsigned int version) {
             ar & BOOST_SERIALIZATION_NVP(name) & BOOST_SERIALIZATION_NVP(value)
                         & BOOST_SERIALIZATION_NVP(min) & BOOST_SERIALIZATION_NVP(max);
       }
 public:
       std::string name;
       float value;
       float min;
       float max;
 
       parameter() {
       }
       parameter(std::string n, float val, float mi, float ma) :
                   name(n), value(val), min(mi), max(ma) {
       }
       parameter(float val) :
                   value(val) {
       }
 };
 
 class hit {
       friend class boost::serialization::access;
       template<class Archive>
       void serialize(Archive & ar, const unsigned int version) {
             ar & BOOST_SERIALIZATION_NVP(cpnr) & BOOST_SERIALIZATION_NVP(E_dep)
                         & BOOST_SERIALIZATION_NVP(X) & BOOST_SERIALIZATION_NVP(Y)
                         & BOOST_SERIALIZATION_NVP(Z);
       }
 
 public:
       hit() {
       }
       hit(int cp, float e, float fx, float fy, float fz) :
                   cpnr(cp), E_dep(e), X(fx), Y(fy), Z(fz) {
       }
 
       TVector3 getVec() {
             return TVector3(X, Y, Z);
       }
 
       int cpnr;
       float E_dep;
       float X;
       float Y;
       float Z;
 };
 
 class entry {
       friend class boost::serialization::access;
       template<class Archive>
       void serialize(Archive & ar, const unsigned int version) {
             ar & BOOST_SERIALIZATION_NVP(m_gg) & BOOST_SERIALIZATION_NVP(cpnr2)
                         & BOOST_SERIALIZATION_NVP(fraction) & BOOST_SERIALIZATION_NVP(fraction2)
                         & BOOST_SERIALIZATION_NVP(angle) & BOOST_SERIALIZATION_NVP(this_bump)
                         & BOOST_SERIALIZATION_NVP(associated_bump); 
       }
 public:
       entry() {
             m_gg = 0.;
             cpnr2 = 0;
             fraction = 1.;
             fraction2 = 1.;
             angle = 0.;
       }
       entry(float val, int cp2, float frac) :
                   m_gg(val), cpnr2(cp2), fraction(frac) {
       }
 
       entry(
                   float val,
                   int cp2,
                   float frac,
                   float frac2,
                   std::vector<hit> &ts_bump,
                   std::vector<hit> &ac_bump) :
                   m_gg(val), cpnr2(cp2), fraction(frac), fraction2(frac2), this_bump(ts_bump), associated_bump(
                               ac_bump) {
       }
       entry(
                   float val,
                   int cp2,
                   float frac,
                   float frac2,
                   float ang,
                   std::vector<hit> &ts_bump,
                   std::vector<hit> &ac_bump) :
                   m_gg(val), cpnr2(cp2), fraction(frac), fraction2(frac2), angle(ang), this_bump(ts_bump), associated_bump(
                               ac_bump) {
       }
 
       float m_gg;
       int cpnr2; 
       float fraction;
       float fraction2;
       float angle;
 
       std::vector<hit> this_bump;
 
       std::vector<hit> associated_bump;
 
       void clear() {
             m_gg = 0.;
             cpnr2 = 0;
             fraction = 1.;
             fraction2 = 1.;
             angle = 0.;
             this_bump.clear();
             associated_bump.clear();
       }
 
 };
 
 #endif
 
