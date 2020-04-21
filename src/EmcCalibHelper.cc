 //--------------------------------------------------------------------------
 
 // File and Version Information:
 //    $Id: EmcCalibHelper.cc,v 1.4 2012/01/24 12:27:29 bernhard Exp $
 //
 // Description:
 //    Class EmcCalibration - a bare outline of an analysis which should
 //      run on either the mini or micro, suitable for filling in with
 //      additional code.
 //------------------------------------------------------------------------
 
 //-----------------------
 // This Class's Header --
 //-----------------------
 #include "EmcCalibHelper.hh"
 //---------------
 // C++ Headers --
 //---------------
 
 #include <math.h>
 #include <string>
 #include <fstream>
 #include <sstream>
 #include <sys/time.h>
 
 #include <vector>
 #include <map>
 
 #include <boost/serialization/vector.hpp>
 #include <boost/serialization/map.hpp>
 
 #include <boost/archive/text_oarchive.hpp>
 #include <boost/archive/text_iarchive.hpp>
 
 #include <boost/archive/xml_iarchive.hpp>
 #include <boost/archive/xml_oarchive.hpp>
 
 //#include "EmcRecoSequence.hh"
 
 #include "EmcCalibration.hh"
 #include "EmcCalibIO.hh"
 
 #include "TVector3.h"
 
 #include "TNtuple.h"
 
 using namespace std;
 
 //-----------------------------------------------------------------------
 // Local Macros, Typedefs, Structures, Unions and Forward Declarations --
 //-----------------------------------------------------------------------
 
 //----------------
 // Constructors --
 //----------------
 
 EmcCalibHelper::EmcCalibHelper() {
       init();
 }
 
 EmcCalibHelper::EmcCalibHelper(const char* options) {
       init();
       setOptions(options);
 }
 
 EmcCalibHelper::~EmcCalibHelper() {
 
 }
 
 void EmcCalibHelper::init() {
       _edgeXtalFile = "../EmcCalibStudies/edgeXtals.dat";
 
 }
 
 void EmcCalibHelper::setOptions(const char* options) {
       TString opts(options);
 
 }
 
 int EmcCalibHelper::do_reco(float shift, std::map<const int, cal_store> &calib_map,entry &hist) {
       int result = 0;
 
       float E_dep1 = 0, E_dep2 = 0, E_tmp = 0;
 
       TVector3 vec1, vec2;
 
       float E_depA = 0, E_depB = 0, E_tmpA = 0;
       TVector3 vecA, vecB;
 
       //local bump
       std::vector<hit>::iterator it_this_bump;
       for (it_this_bump = hist.this_bump.begin(); it_this_bump != hist.this_bump.end();
                   it_this_bump++) {
             E_depA += (*it_this_bump).E_dep;
             if ((*it_this_bump).E_dep > E_tmpA) {
                   vecA = (*it_this_bump).getVec();
             }
             E_tmpA = (*it_this_bump).E_dep;
 
             (*it_this_bump).E_dep *= shift;
 
             E_dep1 += (*it_this_bump).E_dep;
             if ((*it_this_bump).E_dep > E_tmp) {
                   vec1 = (*it_this_bump).getVec(); //workaround: vector from xtal with highest edep -> no cluster finding, no bump splitting
             }
             E_tmp = (*it_this_bump).E_dep;
       }
 
       E_tmp = 0.;
       E_tmpA = 0.;
 
       //associated bump
       std::vector<hit>::iterator it_associated_bump;
       for (it_associated_bump = hist.associated_bump.begin();
                   it_associated_bump != hist.associated_bump.end(); it_associated_bump++) {
             E_depB += (*it_associated_bump).E_dep;
             if ((*it_associated_bump).E_dep > E_tmpA) {
                   vecB = (*it_associated_bump).getVec();
             }
             E_tmpA = (*it_associated_bump).E_dep;
 
             (*it_associated_bump).E_dep *= shift * calib_map[(*it_associated_bump).cpnr].total.value;
 
             E_dep2 += (*it_associated_bump).E_dep;
             if ((*it_associated_bump).E_dep > E_tmp) {
                   vec2 = (*it_associated_bump).getVec(); //workaround: vector from xtal with highest edep -> no cluster finding, no bump splitting
             }
             E_tmp = (*it_associated_bump).E_dep;
       }
 
       cout << "EmcCalibHelper::do_reco saved m_gg = " << hist.m_gg << endl;
 
       float m_ggA = sqrt(2. * E_depA * E_depB * (1. - cos(vecA.Angle(vecB))));
       cout << "EmcCalibHelper::do_reco old calc m_gg = " << m_ggA << endl;
 
       float m_gg = sqrt(2. * E_dep1 * E_dep2 * (1. - cos(vec1.Angle(vec2))));
       cout << "EmcCalibHelper::do_reco new calc m_gg = " << m_gg << endl;
 
       return result;
 }
 
