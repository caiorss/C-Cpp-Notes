// File:   testlib.hpp
// Brief:  Sample Windows shared library / DLL header
// Author: Caio Rodrigues
//------------------------------------------------------
#ifndef _TESTLIB_H_
#define _TESTLIB_H_

#include <ostream>
#include <vector>
#include <string>

#ifndef _WIN32
  // If not compiled for Windows, remove declspec directive.
  #define __declspec(param)  
#endif 

#define EXPORT_CPP 	__declspec(dllexport)
 
#ifdef __cplusplus
  // Indicat that a given symbol/function has C-linkage and
  // does not have name mangling. 
  #define EXPORT_C extern "C" __declspec(dllexport)
#else
  // If a C-compiler uses this header, remove 'extern "C"'
  #define EXPORT_C  __declspec(dllexport)
#endif 


/** The macro __cplusplus is used for allowing this 
 * header to be used from 'C'. If a C compiler is used
 * all definitions inside this #ifdef are discarded.
 */
#ifdef __cplusplus

namespace Linalg {
	EXPORT_CPP double norm(const std::vector<double>& xs);

	EXPORT_CPP std::vector<double> linTransform(
		double a,
		double b,
		std::vector<double>& xs
		);

	EXPORT_CPP
	std::ostream&
	printVector(std::ostream& os, std::vector<double>& xs);	
}

#endif 

// ======= C-interface for Linalg namespace =========//

/** Handle or opaque pointer for std::vector<double> */
typedef void* hVectorD;

/* ----- C-Wrappers for Linalg namespace ---- */

extern "C"  __declspec(dllexport)
double testlib_vectorD_Linalg_norm(hVectorD hv);

extern "C"  __declspec(dllexport)
void testlib_vectorD_Linalg_printVector(const char* name, hVectorD hv);


// ======= Non-polymorphic class exported by DLL =========//

#ifdef __cplusplus
// Non-polymorphic class 
class __declspec(dllexport) SampleClass{
public:
	SampleClass();
	SampleClass(const std::string& name);	
	~SampleClass();
	std::string getName() const;
	int get();
	void set(int n);	
private:
	std::string m_name;
	int m_counter;	
};
#endif 


/* ----- C-Wrappers for SampleClass namespace ---- */

using hSampleClass = void*;

/** Nullable constructor zero-arg constructor */
extern "C" __declspec(dllexport)
hSampleClass
testlib_SampleClass_make0();

/** Other constructor */
EXPORT_C hSampleClass testlib_SampleClass_make1(const char* name);

/** Destructor */
EXPORT_C
void
testlib_SampleClass_delete(hSampleClass hnd);

/** Wrapper for get method */
EXPORT_C
int
testlib_SampleClass_get(hSampleClass hnd);

/** Wrapper for set method */
EXPORT_C
void
testlib_SampleClass_set(hSampleClass hnd, int n);

EXPORT_C
const char*
testlib_SampleClass_getName(hSampleClass hnd);

//==========  Interface class ============//


// Polymorphic Interface class
// binary compatible across different compilers
// as it does not use any STL container on the
// interface.
#ifdef __cplusplus
struct InterfaceClass{
	/* Returns class unique ID */
	virtual const char* getID()	const = 0;
	/** Set class internal state */
	virtual void setName(const char* name) = 0;
	virtual const char* getName() = 0;

	/** Virtual constructor */
	virtual ~InterfaceClass() = default;
	// virtual ~InterfaceClass();
};
#else
  #define InterfaceClass void 
#endif 


/** Factory function */
EXPORT_C InterfaceClass* teslib_InterfaceClass_factory(const char* class_id);
/** C-wrapper for destructor */
EXPORT_C void testlib_InterfaceClass_delete(InterfaceClass* hinst);
/** C-wrapper for getID method */
EXPORT_C const char* testlib_InterfaceClass_getID(InterfaceClass* hinst);
EXPORT_C void testlib_InterfaceClass_setName(InterfaceClass* hinst, const char* name);
EXPORT_C const char* testlib_InterfaceClass_getName(InterfaceClass* hinst);


#endif /** --- End of file */
