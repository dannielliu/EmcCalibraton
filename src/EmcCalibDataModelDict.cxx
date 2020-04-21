// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME EmcCalibDataModelDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "EmcCalibDataModel.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_data(void *p = 0);
   static void *newArray_data(Long_t size, void *p);
   static void delete_data(void *p);
   static void deleteArray_data(void *p);
   static void destruct_data(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::data*)
   {
      ::data *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::data >(0);
      static ::ROOT::TGenericClassInfo 
         instance("data", ::data::Class_Version(), "EmcCalibDataModel.hh", 38,
                  typeid(::data), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::data::Dictionary, isa_proxy, 4,
                  sizeof(::data) );
      instance.SetNew(&new_data);
      instance.SetNewArray(&newArray_data);
      instance.SetDelete(&delete_data);
      instance.SetDeleteArray(&deleteArray_data);
      instance.SetDestructor(&destruct_data);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::data*)
   {
      return GenerateInitInstanceLocal((::data*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::data*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_xtal_def(void *p = 0);
   static void *newArray_xtal_def(Long_t size, void *p);
   static void delete_xtal_def(void *p);
   static void deleteArray_xtal_def(void *p);
   static void destruct_xtal_def(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::xtal_def*)
   {
      ::xtal_def *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::xtal_def >(0);
      static ::ROOT::TGenericClassInfo 
         instance("xtal_def", ::xtal_def::Class_Version(), "EmcCalibDataModel.hh", 62,
                  typeid(::xtal_def), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::xtal_def::Dictionary, isa_proxy, 4,
                  sizeof(::xtal_def) );
      instance.SetNew(&new_xtal_def);
      instance.SetNewArray(&newArray_xtal_def);
      instance.SetDelete(&delete_xtal_def);
      instance.SetDeleteArray(&deleteArray_xtal_def);
      instance.SetDestructor(&destruct_xtal_def);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::xtal_def*)
   {
      return GenerateInitInstanceLocal((::xtal_def*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::xtal_def*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_cal_store(void *p = 0);
   static void *newArray_cal_store(Long_t size, void *p);
   static void delete_cal_store(void *p);
   static void deleteArray_cal_store(void *p);
   static void destruct_cal_store(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cal_store*)
   {
      ::cal_store *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cal_store >(0);
      static ::ROOT::TGenericClassInfo 
         instance("cal_store", ::cal_store::Class_Version(), "EmcCalibDataModel.hh", 81,
                  typeid(::cal_store), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::cal_store::Dictionary, isa_proxy, 4,
                  sizeof(::cal_store) );
      instance.SetNew(&new_cal_store);
      instance.SetNewArray(&newArray_cal_store);
      instance.SetDelete(&delete_cal_store);
      instance.SetDeleteArray(&deleteArray_cal_store);
      instance.SetDestructor(&destruct_cal_store);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cal_store*)
   {
      return GenerateInitInstanceLocal((::cal_store*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cal_store*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_parameter(void *p = 0);
   static void *newArray_parameter(Long_t size, void *p);
   static void delete_parameter(void *p);
   static void deleteArray_parameter(void *p);
   static void destruct_parameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::parameter*)
   {
      ::parameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::parameter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("parameter", ::parameter::Class_Version(), "EmcCalibDataModel.hh", 124,
                  typeid(::parameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::parameter::Dictionary, isa_proxy, 4,
                  sizeof(::parameter) );
      instance.SetNew(&new_parameter);
      instance.SetNewArray(&newArray_parameter);
      instance.SetDelete(&delete_parameter);
      instance.SetDeleteArray(&deleteArray_parameter);
      instance.SetDestructor(&destruct_parameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::parameter*)
   {
      return GenerateInitInstanceLocal((::parameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::parameter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_hit(void *p = 0);
   static void *newArray_hit(Long_t size, void *p);
   static void delete_hit(void *p);
   static void deleteArray_hit(void *p);
   static void destruct_hit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::hit*)
   {
      ::hit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::hit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("hit", ::hit::Class_Version(), "EmcCalibDataModel.hh", 150,
                  typeid(::hit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::hit::Dictionary, isa_proxy, 4,
                  sizeof(::hit) );
      instance.SetNew(&new_hit);
      instance.SetNewArray(&newArray_hit);
      instance.SetDelete(&delete_hit);
      instance.SetDeleteArray(&deleteArray_hit);
      instance.SetDestructor(&destruct_hit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::hit*)
   {
      return GenerateInitInstanceLocal((::hit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::hit*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_entry(void *p = 0);
   static void *newArray_entry(Long_t size, void *p);
   static void delete_entry(void *p);
   static void deleteArray_entry(void *p);
   static void destruct_entry(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::entry*)
   {
      ::entry *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::entry >(0);
      static ::ROOT::TGenericClassInfo 
         instance("entry", ::entry::Class_Version(), "EmcCalibDataModel.hh", 179,
                  typeid(::entry), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::entry::Dictionary, isa_proxy, 4,
                  sizeof(::entry) );
      instance.SetNew(&new_entry);
      instance.SetNewArray(&newArray_entry);
      instance.SetDelete(&delete_entry);
      instance.SetDeleteArray(&deleteArray_entry);
      instance.SetDestructor(&destruct_entry);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::entry*)
   {
      return GenerateInitInstanceLocal((::entry*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::entry*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr data::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *data::Class_Name()
{
   return "data";
}

//______________________________________________________________________________
const char *data::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::data*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int data::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::data*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *data::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::data*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *data::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::data*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr xtal_def::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *xtal_def::Class_Name()
{
   return "xtal_def";
}

//______________________________________________________________________________
const char *xtal_def::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::xtal_def*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int xtal_def::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::xtal_def*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *xtal_def::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::xtal_def*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *xtal_def::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::xtal_def*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr cal_store::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *cal_store::Class_Name()
{
   return "cal_store";
}

//______________________________________________________________________________
const char *cal_store::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cal_store*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int cal_store::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cal_store*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *cal_store::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cal_store*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *cal_store::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cal_store*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr parameter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *parameter::Class_Name()
{
   return "parameter";
}

//______________________________________________________________________________
const char *parameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::parameter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int parameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::parameter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *parameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::parameter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *parameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::parameter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr hit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *hit::Class_Name()
{
   return "hit";
}

//______________________________________________________________________________
const char *hit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::hit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int hit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::hit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *hit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::hit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *hit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::hit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr entry::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *entry::Class_Name()
{
   return "entry";
}

//______________________________________________________________________________
const char *entry::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::entry*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int entry::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::entry*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *entry::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::entry*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *entry::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::entry*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void data::Streamer(TBuffer &R__b)
{
   // Stream an object of class data.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(data::Class(),this);
   } else {
      R__b.WriteClassBuffer(data::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_data(void *p) {
      return  p ? new(p) ::data : new ::data;
   }
   static void *newArray_data(Long_t nElements, void *p) {
      return p ? new(p) ::data[nElements] : new ::data[nElements];
   }
   // Wrapper around operator delete
   static void delete_data(void *p) {
      delete ((::data*)p);
   }
   static void deleteArray_data(void *p) {
      delete [] ((::data*)p);
   }
   static void destruct_data(void *p) {
      typedef ::data current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::data

//______________________________________________________________________________
void xtal_def::Streamer(TBuffer &R__b)
{
   // Stream an object of class xtal_def.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(xtal_def::Class(),this);
   } else {
      R__b.WriteClassBuffer(xtal_def::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_xtal_def(void *p) {
      return  p ? new(p) ::xtal_def : new ::xtal_def;
   }
   static void *newArray_xtal_def(Long_t nElements, void *p) {
      return p ? new(p) ::xtal_def[nElements] : new ::xtal_def[nElements];
   }
   // Wrapper around operator delete
   static void delete_xtal_def(void *p) {
      delete ((::xtal_def*)p);
   }
   static void deleteArray_xtal_def(void *p) {
      delete [] ((::xtal_def*)p);
   }
   static void destruct_xtal_def(void *p) {
      typedef ::xtal_def current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::xtal_def

//______________________________________________________________________________
void cal_store::Streamer(TBuffer &R__b)
{
   // Stream an object of class cal_store.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(cal_store::Class(),this);
   } else {
      R__b.WriteClassBuffer(cal_store::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_cal_store(void *p) {
      return  p ? new(p) ::cal_store : new ::cal_store;
   }
   static void *newArray_cal_store(Long_t nElements, void *p) {
      return p ? new(p) ::cal_store[nElements] : new ::cal_store[nElements];
   }
   // Wrapper around operator delete
   static void delete_cal_store(void *p) {
      delete ((::cal_store*)p);
   }
   static void deleteArray_cal_store(void *p) {
      delete [] ((::cal_store*)p);
   }
   static void destruct_cal_store(void *p) {
      typedef ::cal_store current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cal_store

//______________________________________________________________________________
void parameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class parameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(parameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(parameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_parameter(void *p) {
      return  p ? new(p) ::parameter : new ::parameter;
   }
   static void *newArray_parameter(Long_t nElements, void *p) {
      return p ? new(p) ::parameter[nElements] : new ::parameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_parameter(void *p) {
      delete ((::parameter*)p);
   }
   static void deleteArray_parameter(void *p) {
      delete [] ((::parameter*)p);
   }
   static void destruct_parameter(void *p) {
      typedef ::parameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::parameter

//______________________________________________________________________________
void hit::Streamer(TBuffer &R__b)
{
   // Stream an object of class hit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(hit::Class(),this);
   } else {
      R__b.WriteClassBuffer(hit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_hit(void *p) {
      return  p ? new(p) ::hit : new ::hit;
   }
   static void *newArray_hit(Long_t nElements, void *p) {
      return p ? new(p) ::hit[nElements] : new ::hit[nElements];
   }
   // Wrapper around operator delete
   static void delete_hit(void *p) {
      delete ((::hit*)p);
   }
   static void deleteArray_hit(void *p) {
      delete [] ((::hit*)p);
   }
   static void destruct_hit(void *p) {
      typedef ::hit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::hit

//______________________________________________________________________________
void entry::Streamer(TBuffer &R__b)
{
   // Stream an object of class entry.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(entry::Class(),this);
   } else {
      R__b.WriteClassBuffer(entry::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_entry(void *p) {
      return  p ? new(p) ::entry : new ::entry;
   }
   static void *newArray_entry(Long_t nElements, void *p) {
      return p ? new(p) ::entry[nElements] : new ::entry[nElements];
   }
   // Wrapper around operator delete
   static void delete_entry(void *p) {
      delete ((::entry*)p);
   }
   static void deleteArray_entry(void *p) {
      delete [] ((::entry*)p);
   }
   static void destruct_entry(void *p) {
      typedef ::entry current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::entry

namespace ROOT {
   static TClass *vectorlEhitgR_Dictionary();
   static void vectorlEhitgR_TClassManip(TClass*);
   static void *new_vectorlEhitgR(void *p = 0);
   static void *newArray_vectorlEhitgR(Long_t size, void *p);
   static void delete_vectorlEhitgR(void *p);
   static void deleteArray_vectorlEhitgR(void *p);
   static void destruct_vectorlEhitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<hit>*)
   {
      vector<hit> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<hit>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<hit>", -2, "vector", 214,
                  typeid(vector<hit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEhitgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<hit>) );
      instance.SetNew(&new_vectorlEhitgR);
      instance.SetNewArray(&newArray_vectorlEhitgR);
      instance.SetDelete(&delete_vectorlEhitgR);
      instance.SetDeleteArray(&deleteArray_vectorlEhitgR);
      instance.SetDestructor(&destruct_vectorlEhitgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<hit> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<hit>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEhitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<hit>*)0x0)->GetClass();
      vectorlEhitgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEhitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEhitgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<hit> : new vector<hit>;
   }
   static void *newArray_vectorlEhitgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<hit>[nElements] : new vector<hit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEhitgR(void *p) {
      delete ((vector<hit>*)p);
   }
   static void deleteArray_vectorlEhitgR(void *p) {
      delete [] ((vector<hit>*)p);
   }
   static void destruct_vectorlEhitgR(void *p) {
      typedef vector<hit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<hit>

namespace ROOT {
   static TClass *vectorlEdatagR_Dictionary();
   static void vectorlEdatagR_TClassManip(TClass*);
   static void *new_vectorlEdatagR(void *p = 0);
   static void *newArray_vectorlEdatagR(Long_t size, void *p);
   static void delete_vectorlEdatagR(void *p);
   static void deleteArray_vectorlEdatagR(void *p);
   static void destruct_vectorlEdatagR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<data>*)
   {
      vector<data> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<data>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<data>", -2, "vector", 214,
                  typeid(vector<data>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdatagR_Dictionary, isa_proxy, 0,
                  sizeof(vector<data>) );
      instance.SetNew(&new_vectorlEdatagR);
      instance.SetNewArray(&newArray_vectorlEdatagR);
      instance.SetDelete(&delete_vectorlEdatagR);
      instance.SetDeleteArray(&deleteArray_vectorlEdatagR);
      instance.SetDestructor(&destruct_vectorlEdatagR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<data> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<data>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdatagR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<data>*)0x0)->GetClass();
      vectorlEdatagR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdatagR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdatagR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<data> : new vector<data>;
   }
   static void *newArray_vectorlEdatagR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<data>[nElements] : new vector<data>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdatagR(void *p) {
      delete ((vector<data>*)p);
   }
   static void deleteArray_vectorlEdatagR(void *p) {
      delete [] ((vector<data>*)p);
   }
   static void destruct_vectorlEdatagR(void *p) {
      typedef vector<data> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<data>

namespace {
  void TriggerDictionaryInitialization_EmcCalibDataModelDict_Impl() {
    static const char* headers[] = {
"EmcCalibDataModel.hh",
0
    };
    static const char* includePaths[] = {
"/ustcfs/bes3user/2013/liud/software/panda2/external/include/root",
"/ustcfs/bes3user/2013/liud/pandawork/calibration/BernhardRoth/src/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "EmcCalibDataModelDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$EmcCalibDataModel.hh")))  data;
class __attribute__((annotate("$clingAutoload$EmcCalibDataModel.hh")))  xtal_def;
class __attribute__((annotate("$clingAutoload$EmcCalibDataModel.hh")))  cal_store;
class __attribute__((annotate("$clingAutoload$EmcCalibDataModel.hh")))  parameter;
class __attribute__((annotate("$clingAutoload$EmcCalibDataModel.hh")))  hit;
class __attribute__((annotate("$clingAutoload$EmcCalibDataModel.hh")))  entry;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "EmcCalibDataModelDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "EmcCalibDataModel.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"cal_store", payloadCode, "@",
"data", payloadCode, "@",
"entry", payloadCode, "@",
"hit", payloadCode, "@",
"parameter", payloadCode, "@",
"xtal_def", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("EmcCalibDataModelDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_EmcCalibDataModelDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_EmcCalibDataModelDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_EmcCalibDataModelDict() {
  TriggerDictionaryInitialization_EmcCalibDataModelDict_Impl();
}
