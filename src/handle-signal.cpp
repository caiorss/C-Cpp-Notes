// Author: Caio Rodrigues
// Brief:  Test signal handling in C++.
//-------------------------------------------------

#include <iostream>
#include <map>
#include <string>
#include <functional> 
#include <csignal>
#include <fstream> 

// Unix specific (not valid for MS-Windows)
#include <unistd.h> // Import getpid()


class Dummy{
public:
	Dummy(){
		std::cerr << " [TRACE] Program initialized OK "  << "\n";
	}
	~Dummy(){
		std::cerr << " [TRACE] All objects destroyed. Program End OK. "  << "\n";
	}
};

Dummy dummyGlobal;

void signal_handler(int signal)
{
	// static auto logger = []{
	// 	auto log = std::ofstream("signals.log");
	// 	log << " [INFO] Initialized Logger OK." << std::endl;
	// 	return log;	
	// }();

	auto& logger = std::cerr;
	
	const static std::map<int, std::string> signalDatabse = {
		 {SIGINT,   "SIGINT"}
		,{SIGABRT,  "SIGABRT"}
		,{SIGSEGV,  "SIGSEV"}
		,{SIGFPE,   "SIGFPE"}
		,{SIGTSTP,  "SIGTSTP"}
		,{SIGTERM,  "SIGTERM"}
		,{SIGQUIT,  "SIGQUIT"}
	  };

	// std::cin.ignore(1000);
	std::cin.clear();
	
	std::string sigName;
	
	auto it = signalDatabse.find(signal);
	if(it != signalDatabse.end())
		sigName = it->second;
	else
		sigName = "UNKNOWN";

	logger << "\n [INFO] " << __FUNCTION__ << "() "
		   << " Received singal = {" << sigName << "} => code = " << signal << std::endl;
}

int main()
{
	pid_t pid = ::getpid();
	std::cout << " Process ID =  " << pid << "\n";
	std::cout << " Attach to it with $ gdb --tui --pid=" << pid << "\n";
	
	std::signal(SIGINT,  &signal_handler);
	std::signal(SIGFPE,  &signal_handler);
	std::signal(SIGTERM,  signal_handler);
	std::signal(SIGTSTP,  signal_handler);
	std::signal(SIGQUIT,  signal_handler);
	
	int n = 0;
	char ch = 0;

	std::string line;
	
	while(std::cin.good()){
		std::cout << " Variable n = " << n++ << "\n";
		std::cout << " => Input = ";
		std::getline(std::cin, line);
		
		// Simulate float-point exception 
		if(line == "sigfpe"){
			std::cout << " 10.0 / 0.0 = " << 10.0 / 0.0 << "\n";
		}
	}
	
	return EXIT_SUCCESS;
}
