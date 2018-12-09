// Brief:  Get information about current process
// Author: Caio Rodrigues 
//------------------------------------------------------
#include <iostream>
#include <string>
#include <cassert>

#include <windows.h>

// Return true if program was launched by clicking on it, 
// return false if this program was launched from command line.
auto isInOwnConsole() -> bool {
	DWORD procIDs[2];
	DWORD maxCount = 2;
	DWORD result = GetConsoleProcessList((LPDWORD)procIDs, maxCount);
	return result != 1;
}

auto ExecutablePath = [](int pid){
	HANDLE hProc = OpenProcess( PROCESS_QUERY_INFORMATION
								| PROCESS_VM_READ, FALSE, pid );
	// Get process file path 
	std::string process_path(MAX_PATH, 0);
	DWORD size = MAX_PATH; 
	QueryFullProcessImageNameA(hProc, 0, &process_path[0], &size);
	process_path.resize(size);	
	CloseHandle(hProc);
	return process_path;
};
	
auto CurrentDirectory = []{
	DWORD size = ::GetCurrentDirectory(0, nullptr);
	assert(size != 0);
	std::string path(size + 1, 0x00);
	size = ::GetCurrentDirectory(path.size(), &path[0]);
	path.resize(size);
	return path;
};	

int main(){
	// Get current process ID 
	DWORD    pid   = GetCurrentProcessId();
	// Return module handle of current process 
	HMODULE  hProc = GetModuleHandleA(nullptr);
    // Get process module

	std::cout << "=========== Current Process Info ========"  << std::endl;
	std::cout << "PID              = " << pid                 << std::endl;
	std::cout << "hProc            = 0x" << hProc             << std::endl;
	std::cout << "Executable path  = " << ExecutablePath(pid) << std::endl;
	std::cout << "Current path     = " << CurrentDirectory()  << std::endl;

	// Copy itself to Desktop
	CopyFile(ExecutablePath(pid).c_str(), "C:\\Users\\archbox\\Desktop\\appinfo.exe", false) ;

	// Stop program from exiting if it was launched by clicking on it.
	if(!isInOwnConsole()){
		std::cout << "\n ==> Type RETURN to exit." << std::endl;
		std::cin.get();
	}	
	return EXIT_SUCCESS;
}
