// File:   client.cpp 
// Brief:  Sample C++ client code for DLL testlib.dll 
// Author: Caio Rodrigues
//---------------------------------------------------------------

#include <iostream>
#include <ostream>
#include <vector>
#include <string>

#include "testlib.hpp"


#define WindbgTrace(text)  \
	{  std::stringstream ss; \
	   ss << __FILE__ << ":" << __LINE__ << ": <" << __FUNCTION__ "> "  \
		   << text << std::endl;\
	   OutputDebugString(ss.str().c_str()); \
	}

#define DbgTrace(text) \
	{  std::cerr << __FILE__ << ":" << __LINE__ << ": <" << __FUNCTION__ << "> " \
				 << text << std::endl;	 }

#define DbgDisp(expr) \
	{  std::cerr << __FILE__ << ":" << __LINE__ << ": <" << __FUNCTION__ << ">  " \
				 << #expr << " = " << (expr) << std::endl; }

extern "C" hVectorD testlib_vectorD_make0(size_t n, double);
extern "C" hVectorD testlib_vectorD_make1(size_t n, double array []);
extern "C" void testlib_vectorD_delete(hVectorD hv);

int main(){

	#ifndef DISABLE
    std::cout << "\n=== EXPERIMENT 1 ===> Import C++ functions from DLL" << std::endl;
	
	DbgTrace("Main process starts here.");
	std::vector<double> xs{1.0, 2.0, 3.0, 4.0, 5.0};
	std::cout << " => Linalg::norm(xs) " << Linalg::norm(xs) << std::endl;
	std::cout << "=> xs = "; Linalg::printVector(std::cout, xs); std::cout << std::endl;

    std::cout << "=== EXPERIMENT 2 ===> Import class from DLL" << std::endl;
	auto cls = SampleClass("Dummy");
	cls.set(100);
	std::cout << "cls.getName() = " << cls.getName() << std::endl;
	std::cout << "    cls.get() = " << cls.get() << std::endl;
    #endif // -- eof DISABLE flag

	//=========>> Load functions and classes using C-interface ==============//
	
	std::cout << "\n== EPERIMENT 3 ===> Import C-functions from DLL - C-interface" << std::endl;

	double arr [] = {1, 2, 3, 4, 5};
	hVectorD v1 = testlib_vectorD_make1(5, arr);
	testlib_vectorD_Linalg_printVector("v1", v1);
	std::cout << "norm(v1) = " << testlib_vectorD_Linalg_norm(v1) << std::endl;

	testlib_vectorD_delete(v1);

    std::cout << "\n== EPERIMENT 4 ===> Non-polymorphic class with C-interface " << std::endl;

	hSampleClass hcls = testlib_SampleClass_make1("[EXPERIMENT4]ClassHandle-OOP-C-API");
	std::cout << "[EXPERIMENT 4] hcls.getName() = " << testlib_SampleClass_getName(hcls) << std::endl;
	testlib_SampleClass_set(hcls, 100);
	std::cout << "[EXPERIMENT 4] hcls.get()     = " << testlib_SampleClass_get(hcls) << std::endl;
	testlib_SampleClass_set(hcls, 200);
	std::cout << "[EXPERIMENT 4] hcls.get()     = " << testlib_SampleClass_get(hcls) << std::endl;

	testlib_SampleClass_delete(hcls);	
	
	
	std::cout << "\n== EPERIMENT 5 ===> Load polymorphic classes from DLL " << std::endl;

	InterfaceClass* hinstA = teslib_InterfaceClass_factory("ImplementationA");
	InterfaceClass* hinstB = teslib_InterfaceClass_factory("ImplementationB");

	std::cout << " => hinstA->getID() = " << hinstA->getID() << std::endl;
	std::cout << " => hinstA->getID() = " << hinstB->getID() << std::endl;
	hinstA->setName("ClassA-implA");
	hinstB->setName("ClassB-implB");
	std::cout << " => hinstA->getName() = " << hinstA->getID() << std::endl;
	std::cout << " => hinstB->getName() = " << hinstB->getID() << std::endl;

	// Note: If delete is used directly to delete hinstA and hinstB,
	// a segmentatin fault will happen whenc compiling with Mingw/GCC
	testlib_InterfaceClass_delete(hinstA);
    testlib_InterfaceClass_delete(hinstB);

	std::cout << " [INFO] After deleting instances" << std::endl;
	
	DbgTrace("Program ended OK.");
	return 0;
}

