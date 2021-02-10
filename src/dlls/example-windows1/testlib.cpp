// File:  testlib.cpp 
// Brief: Sample Windows shared library / DLL implementation 
// Author: Caio Rodrigues
//---------------------------------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <functional>

#include <windows.h>

#include "testlib.hpp"

#define WindbgTrace(text)  \
	{  std::stringstream ss; \
	   ss << __FILE__ << ":" << __LINE__ << ": <" << __FUNCTION__ << "> " \
		   << text << std::endl; \
	   OutputDebugString(ss.str().c_str()); \
	}

#define DbgTrace(text) \
	{  std::cerr << __FILE__ << ":" << __LINE__ << ": <" << __FUNCTION__ << "> " \
				 << text << std::endl;	 }

#define DbgDisp(expr) \
	{  std::cerr << __FILE__ << ":" << __LINE__ << ": <" << __FUNCTION__ << ">  " \
				 << #expr << " = " << (expr) << std::endl; }



/** - DLL Entry point - main function of DLL which is executed when 
    the DLL is loaded by some process. 
 */
extern "C" __declspec(dllexport)
BOOL APIENTRY DllMain (HINSTANCE hInst, DWORD reason, LPVOID lpReserved)
{
	std::string text =
		std::string("DLL Loaded into the process => PID = ")
		+ std::to_string(::GetCurrentProcessId());
	WindbgTrace(text);
	DbgTrace(text);
	   	
	switch (reason)
    {
    case DLL_PROCESS_ATTACH:
		WindbgTrace("DLL attached to process");
		DbgTrace("DLL attached to process.");			
      break;
    case DLL_PROCESS_DETACH:
		WindbgTrace("=> DLL detached.");
		DbgTrace("=> DLL attached");
      break;
    case DLL_THREAD_ATTACH:
		WindbgTrace("DLL attached to thread");
		DbgTrace("DLL detached to thread.");
		break;
    case DLL_THREAD_DETACH:
		WindbgTrace("DLL detached from thread");
		DbgTrace("DLL detached from thread.");
      break;
    }

  return TRUE;
}
		
//================================================================//

// Linear algebra tools 
 namespace Linalg{
 	   __declspec(dllexport)
	  double norm(const std::vector<double>& xs){
		double sum = 0.0;
		for(const auto& x : xs) sum += x * x;
		return std::sqrt(sum);
	}

	 __declspec(dllexport)
	std::vector<double>
	linTransform(double a, double b, std::vector<double>& xs){
		std::vector<double> out(xs.size());
		for(size_t i = 0; i < xs.size(); i++){		
			out[i] = a * xs[i] + b;
		}
		return out;
	}

	 __declspec(dllexport)
	std::ostream&
	printVector(std::ostream& os, std::vector<double>& xs){
		os << "[" << xs.size() << "]( ";
		for(const auto&  x: xs)
			os << x << ", ";
		return os << " )";
	}	
}

//=========== C-wrappers ---------------///

// Handler for double vector 
using hVectorD = void*;
using pVectorD = std::vector<double>*;

/** C-wrapper for vector<double> constructor */
extern "C" __declspec(dllexport)
hVectorD
testlib_vectorD_make0(size_t n, double x){
	return new std::vector<double>(n, x);
}

/** C-wrapper for range constructor */
extern "C" __declspec(dllexport)
hVectorD
testlib_vectorD_make1(size_t n, double array []){
	return new std::vector<double>(array, array + n);
}

/** C-wrapper for setting elements of vector<double> */
extern "C" __declspec(dllexport)
void
testlib_vectorD_set(hVectorD hv, size_t n, double x){
	reinterpret_cast<pVectorD>(hv)->operator[](n) = x;
}

/** C-wrapper for vector<double> destructor */
extern "C" __declspec(dllexport)
void
testlib_vectorD_delete(hVectorD hv){
	delete reinterpret_cast<pVectorD>(hv);
}

/** C-wrapepr for Linalg::norm function */
extern "C" __declspec(dllexport)
double
testlib_vectorD_Linalg_norm(hVectorD hv){
	return Linalg::norm(*reinterpret_cast<pVectorD>(hv));
}

extern "C" __declspec(dllexport)
void
testlib_vectorD_Linalg_printVector(const char* name, hVectorD hv){
	std::cout << name << " = ";
	Linalg::printVector(std::cout, *reinterpret_cast<pVectorD>(hv));
	std::cout << std::endl;;
}



//==========>>> class SampleClass <<===========


SampleClass::SampleClass(const std::string& name)
	: m_name(name), m_counter(0)
{
	std::cout << " Instance created with name = " << m_name << std::endl;
}
/** Delegated constructor on right-hand-side */
SampleClass::SampleClass(): SampleClass("unnamed"){}   
	
SampleClass::~SampleClass(){
	std::string text = std::string("SampleClass => name = ") + m_name +  " deleted";
	DbgTrace(text);
}

std::string SampleClass::getName() const {
	return m_name;
}
int SampleClass::SampleClass::get(){		
	return m_counter;
}
void SampleClass::set(int n){
	std::cout << " Counter set to value = " << n << std::endl;
	m_counter = n;
}	

//======= C-interface of SampleClass ===========//

using hSampleClass = void*;

/** Nullable constructor zero-arg constructor */
extern "C" __declspec(dllexport)
hSampleClass
testlib_SampleClass_make0(){
	return new SampleClass();
}

/** Other constructor */
extern "C" __declspec(dllexport)
hSampleClass
testlib_SampleClass_make1(const char* name){
	return new SampleClass(name);
}

/** Destructor */
extern "C" __declspec(dllexport)
void
testlib_SampleClass_delete(hSampleClass hnd){
	delete reinterpret_cast<SampleClass*>(hnd);
}

/** Wrapper for get method */
extern "C" __declspec(dllexport)
int
testlib_SampleClass_get(hSampleClass hnd){
	return reinterpret_cast<SampleClass*>(hnd)->get();
}

/** Wrapper for set method */
extern "C" __declspec(dllexport)
void
testlib_SampleClass_set(hSampleClass hnd, int n){
	return reinterpret_cast<SampleClass*>(hnd)->set(n);
}

extern "C" __declspec(dllexport)
const char*
testlib_SampleClass_getName(hSampleClass hnd){
	return reinterpret_cast<SampleClass*>(hnd)->getName().c_str();
}

//========= Implementations of the interface class ===//

// Destructor 
// InterfaceClass::~InterfaceClass(){}

class ImplementationA: public InterfaceClass
{
private:
	std::string m_name;
public:
	static constexpr const char* class_id = "ImplementationA";
	
	ImplementationA(): m_name("Unammed-A"){ }
	ImplementationA(const std::string& name)
		: m_name(name){}	
	~ImplementationA(){
		std::cout << " [INFO] ImplementationA deleted => name = "
				  << m_name
				  << " ; type = " << class_id
				  << std::endl;
	}
	const char* getID() const {		
		return class_id;
	}
	void setName(const char* name) {
		m_name = name;
	}
	const char* getName() {
		return m_name.c_str();
	}
};


class ImplementationB: public InterfaceClass
{
private:
	std::string m_name;
public:
	static constexpr const char* class_id = "ImplementationB";
	
	ImplementationB(): m_name("Unammed-B"){ }
	ImplementationB(const std::string& name)
		: m_name(name){}	
	~ImplementationB(){
		std::cout << " [INFO] ImplementationB deleted => name = "
				  << m_name
				  << " ; type = " << class_id
				  << std::endl;
	}
	const char* getID() const {		
		return class_id;
	}
	void setName(const char* name) {
		m_name = name;
	}
	const char* getName() {
		return m_name.c_str();
	}
};

EXPORT_C InterfaceClass*
teslib_InterfaceClass_factory(const char* class_id)
{
	auto s = std::string(class_id);
	if(s == "ImplementationA")
		return new ImplementationA();
	if(s == "ImplementationB")
		return new ImplementationB();
	return nullptr;
}

EXPORT_C void testlib_InterfaceClass_delete(InterfaceClass* hinst)
{
	delete hinst;
}
EXPORT_C
const char* testlib_InterfaceClass_getID(InterfaceClass* hinst)
{
	return hinst->getID();
}
EXPORT_C
void testlib_InterfaceClass_setName(InterfaceClass* hinst, const char* name)
{
	hinst->setName(name);
}
EXPORT_C
const char* testlib_InterfaceClass_getName(InterfaceClass* hinst){
	return hinst->getName();
}

///================================================//

// Class private to this compilation unit -
// cannot be accessed from any other file 
namespace {
	class _StaticObject{		
	public:
		using Action = std::function<void ()>;
		Action m_end;
		
		_StaticObject(Action init, Action end)
			:  m_end(end)
		{
			init();
		}
		~_StaticObject(){ m_end(); }
	};

	auto initDLL = _StaticObject(
		[]{
			std::cout << " [StaticObject] => Initialize DLL"
					  << std::endl;						
		},
		[]{	
			std::cout << " [StaticObject] => Shutdown DLL"
					  << std::endl;								
		});
}


/*** ===========>>>> run32dll Entry Points =================  */

extern "C" __declspec(dllexport)
void entryPoint1(HWND hwn, HINSTANCE hinst, LPSTR cmdLine, int nCmdShow){  
	DbgDisp(cmdLine);
	OutputDebugString("Rudll32 called entryPoint1()");
	MessageBoxA(NULL, "DLL ENTRY POINT", "Entry point 1", 0);
}


extern "C" __declspec(dllexport)
int main(){
	std::cout << "Running program OK." << std::endl;
	return 0;
}
