// File:   template-variadic.cpp
// Brief:  Examples about variadic templates.
// Author: Caio Rodrigues
//====================================================================================
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <cmath>
#include <functional>
#include <string>

// Unix (Linux, BSD, MacOSX)
#ifndef _WIN32
  #include <dlfcn.h>
#endif 

#define disp(expr)  std::cerr << __FILE__ << ":" << __LINE__ << ":" \
	<< " ; " <<  #expr << " = " << (expr)  <<  "\n"

/** ======= Print many unrelated arguments ========*/

template<typename T>
void printTypes(const T& x){	
	std::cout << std::left << std::setw(15) << x
			  << std::setw(10) << std::right << " size = " << std::setw(2) << sizeof(x) << "\n";
	std::clog << " [TRACE] Base case => x = " << x << "\n";
}
// Variadic template arguments 
template<typename T, typename ... Types>
void printTypes(const T& x, const Types ... args){
	std::cout << std::left << std::setw(15) << x
			  << std::setw(10) << std::right << " size = " << std::setw(2) << sizeof(x) << "\n";
	printTypes(args ...);
}

/** ======= Invoke methods indirectly through member function pointers =================*/

template<class T, class R, class ... Args>
auto makeCommand(
	// Pointer to member function 
	R (T::* pMemfn) (Args ... args),
	// Member function arguments 
	Args ... arglist) -> std::function<R (T& obj)> {
	return [=](T& obj){ return (obj.*pMemfn)(arglist ...); };
}

class CNCMachine{
private:
	std::string _machineid;
public:
	CNCMachine(const std::string machineid):
		_machineid(machineid){}
	void setSpeed(int n){
		std::cout << "[MACHINE] id = " <<  _machineid
				  << " Set machine speed to level " << n << "\n";
	}
	void setPosition(double x, double y){
		std::cout << "[MACHINE] id = " <<  _machineid << "  Equipment to position set to "
				  << " x = " << x << " ; y = " << y << "\n";
	}
	void shutdown(){
		std::cout << "[MACHINE] id = " <<  _machineid << " Shutdown equipment" << "\n";
	}
};


/** ====== Dynamic loading of symbols of shared libraries  ============= */

/** Type synonym for shared library handler 
 *  Requires: #include <dlfcn.h> and -ldl linker flag */
using LibHandle = std::unique_ptr<void, std::function<void (void*)>>;

auto loadDLL(const std::string& libPath) -> LibHandle {
	// Return unique_ptr for RAAI -> Resource Acquisition is Initialization
	// releasing closing handle when the unique_ptr goes out of scope. 
	return LibHandle(
		dlopen(libPath.c_str(), RTLD_LAZY),
		[](void* h){
			std::cout << " [INFO] Shared library handle released OK." << "\n";
			dlclose(h);
		});		
}

/** Load symbol from shared library 
  *  Requires: #include <dlfcn.h> and -ldl linker flag */
template<typename Function>
auto loadSymbol(const LibHandle& handle, const std::string& symbol) -> Function* {
	void* voidptr = dlsym(handle.get(), symbol.c_str());
	if(voidptr == nullptr)
		return nullptr;
	return reinterpret_cast<Function*>(voidptr);
}

template<class Container>
auto printContainer(
	const std::string& name,
	const Container&   cont,
	const std::string& sep = ", "
	) -> void {
	std::cout << name << " = ";
	for(const auto& x: cont)
		std::cout <<  x << sep;
	std::cout << "\n";
}

int main(){
	using std::string;
	const char nl = '\n';
	
	std::cout << nl << "EXPERIMENT 1 = Function of many argument for printing all of them" << nl;
	std::cout << "---------------------------------------" << nl;
	printTypes("hello world", 10, 'x', 20.23f, true, NAN);

	std::cout << nl << "EXPERIMENT 2 = Indirect method invocation" << nl;
	std::cout << "--------------------------------------" << nl;
	// Create function which invokes an object method, akin to command design pattern.
	using MachineCommand = std::function<void (CNCMachine&)>;
	auto commands = std::list<MachineCommand> {
		 makeCommand(&CNCMachine::setSpeed, 10)
		,makeCommand(&CNCMachine::setPosition, 10.0, -20.0)
		,makeCommand(&CNCMachine::shutdown)
	};
	
	CNCMachine mach1("7Z9FA");
	CNCMachine mach2("MY9FT");
	for(const auto& cmd: commands){
		cmd(mach1);
		cmd(mach2);
	}

	std::cout << nl << "EXPERIMENT 3 = Dynamic Loading from shared library (libgslcblas.so) " << nl;
	std::cout << "--------------------------------------" << nl;	

	auto handle1 = loadDLL("/usr/lib64/libgslcblas.so");
	// Type alias for code simplification 
	using cblas_daxpy_type = void (int, double, const double*, int, double*, int);
	// Get function pointer from shared library 
	auto cblas_daxpy = loadSymbol<cblas_daxpy_type>(handle1, "cblas_daxpy");

	if(cblas_daxpy)
		std::cerr << " [INFO]  Loaded clblas_daxpy OK!" << nl;
	else { 
		std::cerr << " [ERROR] Error: cannot load function cblas_daxpy" << nl;
		return EXIT_FAILURE;
	}

	auto xs = std::vector<double>{ 3.0, 5.0, 6.0, 10.0, 8.0};
	auto ys = std::vector<double>{ 2.0, 2.0, 2.0,  2.0, 2.0};

	printContainer("xs", xs);
	printContainer("ys", ys);
	// Compute xs * 4.0 + ys
	cblas_daxpy(xs.size(), 4.0, &xs[0], 1, &ys[0], 1);
	printContainer("ys", ys);	
	
	return EXIT_SUCCESS;
}
