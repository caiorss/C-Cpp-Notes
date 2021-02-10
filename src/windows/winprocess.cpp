// File:   winprocess.cpp 
// Brief:  Class encapsulatiung MS-Windows process API functions.
// APIS:   CreateProcess, WaitForSingObject, GetProcessExitCode ...
// Author: Caio Rodrigues 
//--------------------------------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <sstream>
#include <functional>
#include <fstream>

#include <windows.h>

//========= File: ProcessBuilder.hpp - Header ===================//

/** Requires: <iostream> <string>, <functional> <vector> <windows.h> */
class ProcessBuilder
{
private:
	// Program to be run 
	std::string              m_program;
	// Arguments passed to the program 
	std::vector<std::string> m_args = {};
	// If this flag is true, the program is launched on console 
	bool                     m_console = true;
	std::string              m_cwd; 
	STARTUPINFO              m_si = { sizeof(STARTUPINFO)};
	PROCESS_INFORMATION      m_pi;
public:	
	using SELF = ProcessBuilder&;
	using LineConsumer = std::function<bool (std::string)>;
	
	ProcessBuilder() = default;
	ProcessBuilder(const std::string& program, const std::vector<std::string>& args = {});
	ProcessBuilder(const ProcessBuilder&) = delete;
	auto operator=(const ProcessBuilder&)  = delete;	
	~ProcessBuilder();
	ProcessBuilder(ProcessBuilder&& rhs);
	auto operator=(ProcessBuilder&& rhs) -> SELF;
	auto SetProgram(const std::string& program) -> SELF;	
	auto SetConsole(bool flag) -> SELF;
	/** Set process directory.
	 * @param path - Process directory path.
	 */
	auto SetCWD(const std::string& path) -> SELF;	
	/** Start process without waiting for its termination.  */
	auto Run() -> bool;
	auto Wait() -> void;
	// Start process and wait for its termination. 
	auto RunWait() -> bool;
	auto GetPID() -> DWORD;
	auto Terminate() -> bool;
	auto isRunning() -> bool;	
	auto StreamLines(LineConsumer consumer) -> bool; 
	auto GetOutput() -> std::string;	
private:
	auto ReadLineFromHandle(HANDLE hFile) -> std::pair<bool, std::string>;	
}; //========= End of Class ProcessBuilder ===== // 


//========= File:  main.cpp ===================//


int main(int argc, char** argv){
	if(argc < 2){
		std::cerr << "Usage: " << argv[0] << " [test0 | test1 | test2 | test3 | test4 ]" << std::endl;
		return EXIT_FAILURE;
	}
	auto tryExit = [&argc, &argv](std::function<int ()> Action) -> void {
		try {
			// End current process with exit code returned
			// from function
			int status = Action();			
			std::cerr << "[SUCCESS] End gracefully";
			std::exit(status);
		}catch(const std::runtime_error& ex)
		 {
			std::cerr << "[ERROR  ] " << ex.what() << std::endl;
			std::exit(1);
		}		
	};
	

	/** Stream process output to stdout line by line. Print line read form subprocess 
	 * output as soon as it arrives. */
	if(std::string(argv[1]) == "test0")
		tryExit([]{
				auto p = ProcessBuilder();
				p.SetProgram("ping 8.8.8.8");				
				std::puts("Stream output of process ping to stdout.");
				p.StreamLines([](std::string line){
						std::cout << " line = " << line << std::endl;
						return true;
					});
				return EXIT_SUCCESS;
			});
	
	/** Read whole process output and then print it to console and to a file. 
      * run $ dir . at path C:\\windows\\system32, read the whole process output 
      * as string printing it and saving it to a log file. (output.log)
      */
	if(std::string(argv[1]) == "test1")
		tryExit([]{
				std::puts("Get output of tasklist.");
				auto p = ProcessBuilder("dir .");		
				p.SetCWD("C:\\windows\\system32");
				std::cout << "Process output = " << std::endl;
				auto out = p.GetOutput();
				std::cout << out << std::endl;
				
				std::ofstream fs("output.log");
				fs << " ==>>> Process output = " << "\n";
				fs << " +=================+ " << "\n";
				fs << out;	
				return EXIT_SUCCESS;
			});

	/** Launch a process (for window subsystem) and wait for its termination. */
	if(std::string(argv[1]) == "test2")
		tryExit([]{
				std::puts("Launch process and wait for its termination");
				auto p = ProcessBuilder("notepad");
				p.Run();
				std::cout << "Waiting for process termination" << std::endl;
				p.Wait();
				std::cout << "Process terminated. OK." << std::endl;
				return EXIT_SUCCESS;
			});				
			
	/** Launch a process and terminate it when user hit RETURN. */
	if(std::string(argv[1]) == "test3")
		tryExit([]{
				std::puts("Launch process and wait for its termination");
				auto p = ProcessBuilder("notepad");
				p.Run();
				std::cout << "Enter RETURN to terminate process => PID = " << p.GetPID() << std::endl;
				std::cin.get();
				p.Terminate();
				std::cout << " Process terminated OK.";
				return EXIT_SUCCESS;
			});

	/** Simulate failure. */
	if(std::string(argv[1]) == "test4")
		tryExit([]{
				std::puts("Launch process and wait for its termination");
				auto p = ProcessBuilder("notepad-error-failure");
				p.Run();
				std::cout << "Enter RETURN to terminate process => PID = " << p.GetPID() << std::endl;
				std::cin.get();
				p.Terminate();
				std::cout << " Process terminated OK.";
				return EXIT_SUCCESS;
			});

	std::cerr << "Error: invalid option " << std::endl;	
	return EXIT_FAILURE;;
}


//========= File: ProcessBuilder.cpp - Implementation ===================//

ProcessBuilder::ProcessBuilder(const std::string& program,
							   const std::vector<std::string>& args)
	: m_program(std::move(program))
	, m_args(std::move(args))
	, m_si()
	  
{ }

ProcessBuilder::~ProcessBuilder(){
	::CloseHandle( m_pi.hProcess );
	::CloseHandle( m_pi.hThread );
}

ProcessBuilder::ProcessBuilder(ProcessBuilder&& rhs)
{
	m_program = std::move(rhs.m_program);
	m_args    = std::move(rhs.m_args);
	m_console = std::move(rhs.m_console);
	m_cwd     = std::move(m_cwd);
	m_si      = std::move(m_si);
	m_pi      = std::move(m_pi);
}
auto ProcessBuilder::operator=(ProcessBuilder&& rhs) -> SELF
{
	std::swap(m_program, rhs.m_program);
	std::swap(m_args,    rhs.m_args);
	std::swap(m_console, rhs.m_console);
	std::swap(m_cwd,     rhs.m_cwd);
	std::swap(m_si,      rhs.m_si);
	std::swap(m_pi,      rhs.m_pi);
	return *this;
}

auto ProcessBuilder::SetProgram(const std::string& program) -> ProcessBuilder& {
	m_program = program;
	return *this;
}
	
auto ProcessBuilder::SetConsole(bool flag) -> ProcessBuilder&
{
	m_console = flag;
	return *this;
}
/** Set process directory.
 * @param path - Process directory path.
 */
auto ProcessBuilder::SetCWD(const std::string& path) -> ProcessBuilder&
{
	m_cwd = path;
	return *this;
}

/** Start process without waiting for its termination.  */
auto ProcessBuilder::Run() -> bool {
	std::string cmdline = m_program;
	for(const auto& s: m_args)
		cmdline = cmdline + " " + s;
	
	bool status = ::CreateProcessA(
		// lpApplicationName
		nullptr
		// lpCommandLine
		,&cmdline[0]
		// lpProcessAttributes
		,nullptr
		// lpThreadAttributes
		,nullptr
		// bInheritHandles
		,m_console ? true : false
		// dwCreationFlags
		,m_console ? 0  : CREATE_NO_WINDOW
		// lpEnvironment - Pointer to environment variables
		,nullptr
		// lpCurrentDirectory - Pointer to current directory
		,m_cwd.empty() ? nullptr : &m_cwd[0]
		// lpStartupInfo
		,&m_si
		// lpProcessInformation
		,&m_pi
		);
	DWORD code;
	// Wait 10 milliseconds 
	::WaitForSingleObject(m_pi.hProcess, 10);
		  ::GetExitCodeProcess(m_pi.hProcess, &code);  
	if(code != STILL_ACTIVE)
		throw std::runtime_error(std::string("Failed to create process = ") + m_program);
	return status;
}

auto ProcessBuilder::Wait() -> void {
	::WaitForSingleObject( m_pi.hProcess, INFINITE );
}

// Start process and wait for its termination. 
auto ProcessBuilder::RunWait() -> bool {
	bool status = this->Run();
	this->Wait();
	return status;
}

auto ProcessBuilder::GetPID() -> DWORD {
	return m_pi.dwProcessId;
}

auto ProcessBuilder::Terminate() -> bool {
	return ::TerminateProcess(m_pi.hProcess, 0);
}

auto ProcessBuilder::isRunning() -> bool {
	DWORD code;
	::GetExitCodeProcess(m_pi.hProcess, &code);
	return code == STILL_ACTIVE;
}

auto ProcessBuilder::StreamLines(ProcessBuilder::LineConsumer consumer) -> bool {
	HANDLE hProcStdoutRead = INVALID_HANDLE_VALUE;
	HANDLE hProcStdoutWrite = INVALID_HANDLE_VALUE;
	SECURITY_ATTRIBUTES sattr;
	sattr.nLength              = sizeof(SECURITY_ATTRIBUTES);
	sattr.bInheritHandle       = true;
	sattr.lpSecurityDescriptor = nullptr;
	if(!::CreatePipe(&hProcStdoutRead, &hProcStdoutWrite, &sattr, 0))
		throw std::runtime_error("Error: failed to create pipe");
		
	if(!::SetHandleInformation(hProcStdoutRead, HANDLE_FLAG_INHERIT, 0))
		throw std::runtime_error("Error: failed to set handle information");		

	m_si.hStdOutput = hProcStdoutWrite;
	m_si.dwFlags   |= STARTF_USESTDHANDLES;
	bool status = this->Run();	   
	// The writing handler must be closed befored reading the 
	::CloseHandle(hProcStdoutWrite);
	auto lin = std::pair<bool, std::string>{};
	while(lin = ReadLineFromHandle(hProcStdoutRead), lin.first)
		if(!consumer(std::move(lin.second))) break;
	return status;
} //--- EOf ReadOutput --- //

auto ProcessBuilder::GetOutput() -> std::string {
	std::stringstream ss;
	StreamLines([&ss](std::string line){
			ss << line << '\n';
			return true;
		});
	return ss.str();
}

auto ProcessBuilder::ReadLineFromHandle(HANDLE hFile) -> std::pair<bool, std::string> 
{
	std::stringstream ss; 
	DWORD bytesRead;
	char c;
	bool result;
	while(true){
		result = ::ReadFile(hFile, &c, 1, &bytesRead, nullptr); //, result
		if(GetLastError() == ERROR_HANDLE_EOF || GetLastError() == ERROR_BROKEN_PIPE){
			// std::cerr << " [TRACE] Reach END OF FILE." << std::endl;
			return std::make_pair(false, "");
		}				
		if(c == '\n') break;	
		ss << c;
	}
	// Return (NOTEOF?, string) => Flag returns false while end of file is not reached.
	std::string line = ss.str();
	char last = line[line.size()-1];
	if(last == '\r' || last == '\n')
		line = line.substr(0, line.size() - 2);
	return std::make_pair(!(bytesRead == 0 && result), ss.str());
};	
