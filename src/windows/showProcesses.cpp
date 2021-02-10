//---------------------------------------------
// Brief:  Show all processes in current machine.
// Author: Caio Rodrigues.
//---------------------------------------------

// C++ Headers 
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

// Windows Headers 
#include <windows.h>
#include <tlhelp32.h>

auto showProcessInfo(std::ostream& os) -> int;

int main(){
	// Log file 
	auto plog = std::ofstream("process-log.txt");
	// Print all processes to stdout. 
	showProcessInfo(std::cout);
	// Write all processes to file 
	showProcessInfo(plog);
	// Flush buffer - force data to be written to file.
	plog.flush();

	std::cout << "\n ==> Type return to exit." << std::endl;
	std::cin.get();
  return 0;
}

//=======================///

// Show all processes in current machine 
auto showProcessInfo(std::ostream& os) -> int {
  // Get snapshot with process listing.
  HANDLE hProcessSnapShot =
    CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  
  // Instnatiate process' entry structure.
  PROCESSENTRY32 ProcessEntry = { 0 };
  ProcessEntry.dwSize = sizeof( ProcessEntry );
  BOOL Return = FALSE;
  Return = Process32First( hProcessSnapShot, &ProcessEntry );
  // Returns -1 if process failed 
  if(!Return ) {  return -1;}
  do { // print process' data
      os << "EXE File     = " << ProcessEntry.szExeFile      << "\n"
         << "PID          = " << ProcessEntry.th32ProcessID  << "\n"
         << "References   = " << ProcessEntry.cntUsage       << "\n"
         << "Thread Count = " << ProcessEntry.cntThreads     << "\n"  
         << "-----------------------------------------------\n";
    }
  while( Process32Next( hProcessSnapShot, &ProcessEntry ));
  // Close handle releasing resource.
  CloseHandle( hProcessSnapShot );
  return 1;
}
