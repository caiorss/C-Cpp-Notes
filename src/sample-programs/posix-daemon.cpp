// File:   posix-daemon.cpp 
// Brief:  Sample Posix Daemon encapsulated in a class with syslog. 
// Author: Caio Rodrigues
//-------------------------------------------------------------------

#include <iostream>
#include <random>
#include <string>
#include <sstream>
#include <functional>
#include <thread>
#include <fstream>
#include <iomanip>
#include <memory> // Smart pointer 

#include <cstdio>
#include <cstdlib>
#include <cstring> // string.h
#include <csignal>

//------ U*nix only ----//
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
// Syslog
#include <syslog.h>


class PosixDaemon{
public:
	using Action = std::function<bool ()>;

	PosixDaemon(std::string path, std::string pidfile,  Action action)
		: m_path(std::move(path)),
		  m_pidfile(std::move(pidfile)),
		  m_action(action) { }
	~PosixDaemon(){}	
	
	// Disable copy constructor in order to forbid copy 
	PosixDaemon(const PosixDaemon&) = delete;
	// Disable copy-assignment operator to make the class non-copiable.
	PosixDaemon& operator= (const PosixDaemon&) = delete;
	
	auto run() -> void {
		// Make child process
		pid_t child_pid = fork();
		if(child_pid < 0){
			std::cerr << "Error: failed to fork this process." << "\n";
			return;
		}
		if(child_pid > 0){
			std::cout << "Process ID of child process = " << child_pid << "\n";
			return;
		}
		// Umask file mode
		::umask(0);
		// Set new session
		pid_t sid = ::setsid();
		if(sid < 0)
			return;
		//------ Code of Forked Process ---------//
		// Set path of forked process (daemon)
		::chdir(m_path.c_str());

		// Check whether there is a running process of this program 
		auto fs = std::ifstream(m_pidfile);
		if(fs.good()){
			int pid;
			fs >> pid;
			std::cerr << " [LOG] Kill process of PID = " << pid << "\n";
			::kill(-pid, SIGTERM);
			fs.close();							
		}
		auto fo = std::ofstream(m_pidfile);
		if(fo.good()){
			int pid = ::getpid();
			std::cerr << "Child PID = " << pid << "\n";
			fo << pid  << std::flush;
		} else {
			std::cerr << " [LOG] Error: could not open PID file " << m_pidfile << "\n";
			return;
		}	
		// Close stdin, stdout and stderr file descriptors.
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		while(m_action());
	}	
private:
	std::string m_path;
	std::string m_pidfile;
	Action m_action;
	// Action m_onExit;
};

int main(int argc, char** argv){

	std::random_device rdng;	
	auto randomGen = std::bind(
		std::uniform_real_distribution<double>(10.0, 60.0),
		std::default_random_engine(rdng())
		);

	setlogmask (LOG_UPTO (LOG_INFO));
	openlog ("price-service", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);	
	
	PosixDaemon daemon{		
		"/",
		"/tmp/price-server.pid",
		[&randomGen](){
			// Action executed in the child process (daemon)
			std::stringstream ss;
			ss << std::setprecision(3) << std::fixed;
			ss << "Price = " << randomGen() << " path = " << getcwd(nullptr, 0) ;
			syslog (LOG_INFO, "%s", ss.str().c_str());
			// 1 seconds delay
			std::this_thread::sleep_for(std::chrono::seconds(1));
			return true;
		}					  
	};

	daemon.run();
	return 0;
}
