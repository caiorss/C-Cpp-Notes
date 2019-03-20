// Author: Caio Rodrigues
// Descr:  Sample Native Python 3 module (library) DLL
//
// Compile with:
// $ clang++ mymodule.cpp -o mymodule.so -g -std=c++1z -fPIC -shared -I/usr/include/python3.6m  
//-------------------------------------------------------

#include <iostream>
#include <string>
#include <functional>
#include <iomanip>

// Solve Mingw error: '::hyport' has not been declared 
#include <math.h>

//#define hypot _hypot

/// C-API header to Python Native Interface API, similar to Java JNI
/// (Java Native Interface)
#include <Python.h>

#ifdef _WIN32
  #include <process.h> // Exports _getpid()
#else
  #include <unistd.h>  // Exports getpid()
#endif 


struct DLLInitialization
{
	DLLInitialization(){
		int pid;
		#ifdef _WIN32
 		   pid = _getpid();
		#else
		   pid = getpid();       
		#endif 
		std::cerr << " [TRACE] Python module DLL loaded by process PID = <"
				  << pid << "> "
				  << std::endl;
		std::cerr << " [TRACE] Attach the debugger with: $ gdb -pid=" << pid << "\n";
	}
	~DLLInitialization(){
		std::cerr << " [TRACE] DLL native DLL unloaded OK." << std::endl;
	}	
};

DLLInitialization dllinit_hook;

/** ======== List of functions exposed to Python =========== */

PyObject*
exposedFunction(PyObject* self, PyObject* args);

PyObject* testArguments(PyObject* self, PyObject* args);

auto printRange(PyObject* self, PyObject* args) -> PyObject*;
auto taylorSeriesExp(PyObject* self, PyObject* args) -> PyObject*;
PyObject* returnTuple(PyObject* self, PyObject* args);
PyObject* returnDictionary(PyObject* self, PyObject* args);
PyObject* tabulateFunction(PyObject* self, PyObject* args);
PyObject* computeStatistics(PyObject* self, PyObject* args);

static PyMethodDef ModuleFunctions [] =
{
	{ "exposedFunction", exposedFunction, METH_VARARGS,
	  "Documentation or docstring of function exposedFunction1." }

	,{"testArguments", testArguments, METH_VARARGS,
	  "Test python arguments: Signature testArguments(int, float, double, const char*)"}
	
	// Function without docstring: printRange(int i, double x)
	,{ "printRange", printRange, METH_VARARGS, nullptr }

	,{ "taylorSeriesExp", &taylorSeriesExp, METH_VARARGS,
	   "taylorSeriesExp(double x, size_t maxiter, double tol) -> double"
	   "\n Computes exponential of a given value with taylor serie approximation."
	   "\n Formula reference:  https://www.mathsisfun.com/algebra/taylor-series.html"
	}
	,{"returnTuple", &returnTuple, METH_VARARGS, nullptr}
	,{"returnDictionary", &returnDictionary, METH_VARARGS, nullptr}
	,{"computeStatistics", &computeStatistics, METH_VARARGS, nullptr}
	,{"tabulateFunction", tabulateFunction, METH_VARARGS,
	  "Tabulate some mathematical function or callable object"}
	// Sentinel value used to indicate the end of function listing.
	// All function listing must end with this value.
	,{nullptr, nullptr, 0, nullptr}									
};

/* Module definition */
static struct PyModuleDef ModuleDefinitions {
	PyModuleDef_HEAD_INIT,
	// Module name as string 
	"mymodule",
	// Module documentation (docstring)
	"A sample C++ native-code module for python3.",
	-1,
	// Functions exposed to the module 
	ModuleFunctions
};

/** Module Initialization function: must have this name schema
 *  PyInit_<ModuleName> where ModuleName is the same base name of the 
 *  shared library ModuleName.so (on Linux) or ModuleName.pyd (on Windows)
 */
PyMODINIT_FUNC PyInit_mymodule(void)
{
	Py_Initialize();
	PyObject* pModule = PyModule_Create(&ModuleDefinitions);
	PyModule_AddObject(pModule, "version", Py_BuildValue("s", "version 0.1-Alpha"));
	return pModule;
}


// =========  Functions of the Python Module ======== //


PyObject*
exposedFunction(PyObject* self, PyObject* args)
{
	std::cout << " =>> Hello word Python from C++ side." << "\n";
	// All python functions that returns anything
	// should end with this macro 
	Py_RETURN_NONE;
}


PyObject* testArguments(PyObject* self, PyObject* args)
{
	int         abool;
	int         aint;
	float       afloat;
	double      adouble;
	char*       aword;
	// Parse function arguments 
	if(!PyArg_ParseTuple(args, "pifds", &abool, &aint, &afloat, &adouble, &aword))
		return nullptr;

	std::cerr << " Function: [" << __FUNCTION__ << "]" 
			  << " ==> Number of arguments = " << PyTuple_GET_SIZE(args) << "\n";
	
	std::string str(aword, aword + strlen(aword));
	str = "'Received string = " + str + "'";
	
	std::cout << " Received Python Arguments " << "\n";
	std::cout << std::boolalpha;
	std::cout << "abool[bool] = " << (bool) abool << "\n"
			  << " abool[int] = " << abool << "\n"
			  << "      aint  = " << aint << "\n"
			  << "     afloat = " << afloat << "\n"
			  << "    adouble = " << adouble << "\n"
			  << "    aword   = " << str << "\n"
			  << "\n";		
	return Py_BuildValue("s", str.data());
}

auto printRange(PyObject* self, PyObject* args) -> PyObject*
{
	std::cerr << " =>> Printing numeric range " << "\n";	
	int    n;
	float  x;
	// Extract function argument from tuple argument object
	// "if" => i - Extract integer
	//      => f - Extract float (IEEE754 32 bits float point)
	if(!PyArg_ParseTuple(args, "if", &n, &x)){
		PyErr_SetString(PyExc_RuntimeError, "Invalid argument");
		// Always return null on failure 
		return nullptr; 
	}

	if(n <= 0){
		PyErr_SetString( PyExc_RuntimeError
						 ,"Invalid argument: n supposed to be greater than zero.");
		return nullptr;
	}	

	for(int k = 0; k < n; k++)
		std::cout << " k[" << k << "] = " << 3 * k + x << "\n";	
	Py_RETURN_NONE;
}

auto taylorSeriesExp(PyObject* self, PyObject* args) -> PyObject*
{
	double x;
	size_t maxiter;  // Maximum number of iterations 
	double tol;      // Tolerance

	// Parse function arguments 
	if(!PyArg_ParseTuple(args, "did", &x, &maxiter, &tol))
		return nullptr;

	// Validate function arguments
	
	if(tol <= 0 || tol > 1.0){
		PyErr_SetString( PyExc_RuntimeError
						 ,"Invalid tolerance, expected in range (0, 1]");
		return nullptr;
	}
	
	// Compute exponential taylor series available at
	// https://www.mathsisfun.com/algebra/taylor-series.html
	unsigned long factorial = 1;
	double        xpower    = 1.0;
	double        sum       = 0.0;
	double        term      = 0.0;
	size_t        idx       = 1;
	do{
		term      = xpower / factorial;
		sum       = sum + term;
		xpower    = xpower * x ;
		factorial = factorial * idx;		
		idx++;
	} while(idx <= maxiter && std::abs(term) > std::abs(sum) * tol );
	// Return float point constnat NAN (Not a Number)
	if(idx >= maxiter){
		std::cerr << " [ERROR] Series does not converge." << "\n";
		return Py_BuildValue("d", NAN);
	}
	return Py_BuildValue("d", sum);
}

/** Function that returns multiple values as tuple object */
PyObject* returnTuple(PyObject* self, PyObject* args)
{
	double x;
	double y;
	int    n;
	if(!PyArg_ParseTuple(args, "ddi", &x, &y, &n))
		return nullptr;
	// Build tuple object containing
	// (d: double, d: double, i: integer, s: string)
	return Py_BuildValue("(ddis)", 2 * x + y * n, x + 2 * y - n, n, "Result");
}


/** Return a dictionary object */
PyObject* returnDictionary(PyObject* self, PyObject* args)
{
	double x;
	double y;
	int    n;
	if(!PyArg_ParseTuple(args, "ddi", &x, &y, &n))
		return nullptr;
	
	// Create new dictionary object 
	PyObject* pDict = PyDict_New();

	PyObject* p = nullptr;
	// Fill dictionary
	p = Py_BuildValue("d", 2 * x + y * n);
	PyDict_SetItemString(pDict, "alpha", p);
	Py_DECREF(p);
	
	p = Py_BuildValue("i", 3 * n) ;
	PyDict_SetItemString(pDict, "size", p);
	Py_DECREF(p);

	p = Py_BuildValue("s", "Some string") ;
	PyDict_SetItemString(pDict, "beta", p);
	Py_DECREF(p);	
	
	return pDict;
}

PyObject* computeStatistics(PyObject* self, PyObject* args)
{

	std::cerr << " [TRACE] " << __FILE__ << "(" << __LINE__ << ") - "
			  << "Calling function: " << __FUNCTION__ << "\n";

	std::cerr << " [TRACE] Arguments = "; PyObject_Print(args, stdout, 0); std::cerr << "\n";
	
	PyObject* pSeq;
	// Parse function argument as sequence
	if(!PyArg_ParseTuple(args, "O", &pSeq))
		return nullptr;

	pSeq = PySequence_Fast(pSeq, "Expected iterable arguments");
	if(pSeq == nullptr) return nullptr;

	int numberOfElements = PySequence_Fast_GET_SIZE(pSeq);
	std::cerr << " [TRACE] numberOfElements = " << numberOfElements << "\n";

	PyObject* pItem = nullptr;
	double x;
	double sum;
	
	for(int n = 0; n < numberOfElements; n++)
	{
		pItem = PySequence_Fast_GET_ITEM(pSeq, n);
		if(!pItem) {
			Py_DECREF(pSeq);
			return nullptr;
		}
		// Print item 
		std::cout << "item[" << n << "] = ";
		PyObject_Print(pItem, stdout, 0);
		std::cout << "\n";
		
		x = PyFloat_AsDouble(pItem);
		if(PyErr_Occurred() != nullptr){
			PyErr_SetString(PyExc_TypeError, "Error: expected float point.");
			return nullptr;
		}
		sum += x;
	}

	double mean = sum / numberOfElements;
	std::cout << "\n (C++) Statics Result " << "\n";
	std::cout << "---------------" << "\n";
	std::cout << " Sum  = " << sum << "\n";
	std::cout << " Mean = " << mean << "\n";
	
	Py_DECREF(pSeq);
	return Py_BuildValue(""); // Return None 
}


/** Function with callback - A callback can be any function or object
 * with the __call__ method. 
 */
PyObject* tabulateFunction(PyObject* self, PyObject* args)
{
	PyObject* pObj = nullptr;
	double xmin, xmax, xstep;
	
	if(!PyArg_ParseTuple(args, "Oddd", &pObj, &xmin, &xmax, &xstep))
		return nullptr;
	if(pObj == nullptr) {
		PyErr_SetString(PyExc_RuntimeError, "Error: invalid None object.");
		return nullptr;
	}
	PyObject* pArgs  = nullptr;
	PyObject* pResult = nullptr;
	double y = 0.0;

	std::cout << std::fixed << std::setprecision(4);

	std::cout << "Tabulating range: "
			  << " ; xmin = " << xmin
			  << " ; xmax = " << xmax
			  << " ; step = " << xstep
			  << "\n";
	
	for(double x = xmin; x <= xmax; x += xstep )
	{
		pArgs  = Py_BuildValue("(d)", x);
		pResult = PyEval_CallObject(pObj, pArgs);
		y = PyFloat_AsDouble(pResult);
		if(PyErr_Occurred() != nullptr){
			PyErr_SetString(PyExc_RuntimeError, "Error: Invalid float point.");
			return nullptr;
		}
		std::cout << std::setw(8) << x << std::setw(10) << y  << "\n";
	}
	Py_RETURN_NONE;
}
