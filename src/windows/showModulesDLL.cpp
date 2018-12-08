// File;   showModulesDLL.cpp 
// Brief:  List all modules DLLs (Dynamic Linked Libraries) loaded by some process.
// Author: Caio Rodrigues 
//-----------------------------------------------------------------------------
#include <iostream>
#include <string> 
#include <iomanip>
#include <functional>

//- Windows Headers ---
#include <windows.h>
#include <psapi.h>

// RAAI for managing Handler 
template<class HANDLER>
class ResourceHandler
{
public:
	using Disposer = std::function<void (HANDLER)>;
	ResourceHandler(HANDLER hnd, Disposer disposer)
		: m_hnd(hnd), m_fndisp(disposer)
	{  }
	auto get() -> HANDLER {
		return m_hnd;
	}
	~ResourceHandler(){
		m_fndisp(m_hnd);
	}
	// Disable copy-constructor and copy-assignment operator 
	ResourceHandler(const ResourceHandler&) = delete;
	auto operator= (const ResourceHandler&) -> ResourceHandler& = delete;
	// Move member functios
	ResourceHandler(ResourceHandler&& rhs)
		: m_hnd(rhs.m_hnd),
		  m_fndisp(rhs.m_fndisp){ }
	auto operator= (ResourceHandler&& rhs){
		std::swap(this->m_hnd, rhs.m_hnd);
		this->m_fndisp = rhs->m_fndisp;		
	}
private:
	HANDLER m_hnd;
	Disposer m_fndisp;
};

auto utf8_encode(const std::wstring& wstr) -> std::string;
auto utf8_decode(const std::string&  str) -> std::wstring;

namespace WProcess{
	using ModuleConsumer = std::function<auto (HMODULE, const std::string&) -> void>;	
	auto GetName(HANDLE hProc) -> std::string;
	auto GetExecutablePath(HANDLE hProc) -> std::string;
	auto ForEachModule(HANDLE hProc, ModuleConsumer FunIterator) -> void;
}


int main(int argc, char **argv){  
  if(argc < 2){
    std::cerr << "Error: missing Process ID <PID>.";
    return EXIT_FAILURE;
  }
  
  DWORD pid;
  DWORD  flags = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;
  
  try{
	  pid  = std::stoi(argv[1]);
  } catch(const std::invalid_argument& ex){
	  std::cerr << "Error invalid PID" << std::endl;
	  return EXIT_FAILURE;
  }  
    
  auto hProc = ResourceHandler<HANDLE>{
	  ::OpenProcess(flags, FALSE, pid),
	  ::CloseHandle
  };

  if(hProc.get() == nullptr){
	  std::cerr << "Error: process of pid = <" << pid << "> not found." << std::endl;
	  return EXIT_FAILURE;
  }

  std::cout << "Process base name = "
			<< WProcess::GetName(hProc.get())
			<< std::endl; 
  std::cout << "Process path      = "
			<< WProcess::GetExecutablePath(hProc.get())
			<< std::endl;

  std::cout << std::endl;

  // Print all DLLs used by some process 
  WProcess::ForEachModule(
	  hProc.get(),
	  [](HMODULE hmod, const std::string& path) -> void
	  {
		  std::cout << std::setw(10) << hmod
					<< std::setw(5)  << " "
					<< std::left 	 << std::setw(45) << path
					<< "\n";					
	  }
  	  );
	  
  return EXIT_SUCCESS;
}
// ================= End of Main =================//

auto utf8_encode(const std::wstring &wstr) -> std::string
{
    int size_needed = WideCharToMultiByte(
            CP_UTF8, 0, &wstr[0],
            (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(
            CP_UTF8, 0, &wstr[0],
            (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

auto utf8_decode(const std::string &str) -> std::wstring
{
     int size_needed = MultiByteToWideChar(
             CP_UTF8, 0,
             &str[0], (int)str.size(), NULL, 0);
     std::wstring wstrTo(size_needed, 0);
     MultiByteToWideChar(
             CP_UTF8, 0, &str[0],
             (int) str.size(), &wstrTo[0], size_needed);
     return wstrTo;
}
	

namespace WProcess{
	using ModuleConsumer = std::function<auto (HMODULE, const std::string&) -> void>;
	
	auto GetName(HANDLE hProc) -> std::string {
		std::wstring basename(MAX_PATH, 0);
		int n1 = ::GetModuleBaseNameW(hProc, NULL, &basename[0], MAX_PATH);
		basename.resize(n1);
		return utf8_encode(basename);
	}
	auto GetExecutablePath(HANDLE hProc) -> std::string {
		std::wstring path(MAX_PATH, 0);
		int n2 = ::GetModuleFileNameExW(hProc, NULL, &path[0], MAX_PATH);
		path.resize(n2);
		return utf8_encode(path);
	}
	auto ForEachModule(HANDLE hProc, ModuleConsumer FunIterator) -> void {
		HMODULE hMods[1024];
		DWORD   cbNeeded;
		if (::EnumProcessModules(hProc, hMods, sizeof(hMods), &cbNeeded)){
			int n = cbNeeded / sizeof(HMODULE);
			std::wstring path(MAX_PATH, 0);
			for(int i = 0; i < n; i++){
				DWORD nread = ::GetModuleFileNameExW(hProc, hMods[i], &path[0], MAX_PATH);
				path.resize(nread);		
				if(nread) FunIterator(hMods[i], utf8_encode(path));
			}
		}	
	}
	
} // --- End of namespace WProcess ---- //
