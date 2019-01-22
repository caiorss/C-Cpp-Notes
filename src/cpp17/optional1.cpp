#include <iostream>
#include <string>  // std::stof 
#include <ostream> // operator (<<) and class ostream 
#include <vector>
#include <iomanip> // setprecision

#include <optional>

// Class with optional field. 
struct Location{
	double lat;
	double lon;
	std::optional<std::string> name; 
};

// Make std::optional<Location> printable 
std::ostream& operator<<(std::ostream& os, const Location& x){
	os << std::setprecision(2) << std::fixed;
	if(x.name)
		os << "Location("
		   << "x = " << x.lat
		   << " ; y = " << x.lon
		   << " ; name = "
		   << x.name.value() << ")";
	else
		os << "Location("
		   << "x = " << x.lat
		   << " ; y = " << x.lon
		   << " ; name = "
		   << "<unknown>" << ")";       
	return os;  
}

// Make std::optional<double> printable 
std::ostream& operator<<(std::ostream& os, const std::optional<double>& x){
	if(x)
		os << "Some[Double](" << x.value() << ")";
	else
		os << "None[Double]";
	return os;  
}

std::optional<double>
parseDouble(const std::string& str)
{
	try{
		// Return some value 
		return std::make_optional(stod(str));
	} catch (const std::invalid_argument& ex) {
		// Return nothing 
		return std::nullopt;
	}
}

auto getEnvVar(std::string const& varname) -> std::optional<std::string>
{
	if(const char* v = std::getenv(varname.c_str()))
		// Returns some value 
		return v;
	else
		// Returns nothing (same as std::nullopt)		
		return {};
}
      
int main(){
	char endl = '\n';
      
	std::optional<double> oa;
	std::optional<double> ob = std::nullopt;
	std::optional<double> oc = 10.233;
	std::optional<double> od(1e3);
	std::optional<double> oe {};

	std::puts("==== EXPERIMENT 1 === Print optional value");
	std::cout << "oa = " << oa << endl;
	std::cout << "ob = " << ob << endl;
	std::cout << "oc = " << oc << endl;
	std::cout << "od = " << od << endl;
	std::cout << "oe = " << oe << endl; 

	std::puts("==== EXPERIMENT 2 === Reset optional value");
	std::cout << "Reset variable oc " << endl;
	oc.reset();
	std::cout << "oc = " << oc << endl;
      
	std::puts("==== EXPERIMENT 3 ==== check whether optional has value. (is not empty) ====");
	if(oc)
		std::cout << ">> oc has a vaule" << endl;
	else
		std::cout << ">> oc is empty" << endl;

	std::puts("==== EXPERIMENT 4 ==== try parse double ====");      
	std::cout << "x0 = " << parseDouble("-1.0e3") << endl;
	std::cout << "x1 = " << parseDouble("1.351 ") << endl;
	std::cout << "x2 = " << parseDouble("1.asdasd351 xxgh") << endl;
	std::cout << "x4 = " << parseDouble("sad4543fgx") << endl;

	std::puts("=== EXPERIMENT 5 ======== Structs/Classes with optional fields ====");
	std::vector<Location> xs {
		{10.31, 23.4, "Waypoint Delta"},
		{-46.23, 145.13, "Waypoint gamma"},
		{90.43, 100.345, std::nullopt},
		{0, 0, {}},
	};
	int i = 0;
	for(auto x: xs){
		std::cout << "x[" << i << "]" << " = " << x << endl;
		i++;
	}

	std::puts("=== EXPERIMENT 6 - Print environment variables. ===== ");
	auto var_home =  getEnvVar("HOME");
	if(var_home.has_value())
		std::cout << "$HOME = " << var_home.value() << endl;
	else
		std::cout << "$HOME = " << "UNKNOWN" << endl;
      
	auto var_dummy =  getEnvVar("DUMMY-VAR");
	if(var_dummy.has_value())
		std::cout << "$DUMMYVAR = " << var_dummy.value() << endl;
	else
		std::cout << "$DUMMYVAR = " << "UNKNOWN" << endl;
      

	std::puts("==== EXPERIMENT 7 - Test member function .value_or() ====");
	std::cout << "$XDG_SESSION_PATH = "
			  << getEnvVar("XDG_SESSION_PATH").value_or("<<error: not set>>") << '\n';
	std::cout << "$HOMEX = "
			  << getEnvVar("HOMEX").value_or("<<error: not set>>") << '\n';
      
	return 0;
}
