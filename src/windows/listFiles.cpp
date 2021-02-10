// File:   listFiles.cpp 
// Brief:  List all files of a given directory
// Note:   Windows-only - Uses API 
// Author: Caio Rodrigues 
//----------------------------------------------------
#include <iostream>
#include <string> 
#include <sstream>
#include <iomanip>
#include <functional>

//- Windows Headers ---
#include <windows.h>

//----------------------------------------------//

// Uses RAII for setting console to UTF8
// and restoring its previous settings.
class ConsoleUTF8{
public:
   // Constructor saves context 
   ConsoleUTF8(){
       m_config = ::GetConsoleOutputCP();
       ::SetConsoleOutputCP(CP_UTF8);
       std::cerr << " [TRACE] Console set to UTF8" << std::endl;
   }
   // Destructor restores context 
   ~ConsoleUTF8(){
	   std::cerr << " [TRACE] Console restored." << std::endl;
      ::SetConsoleOutputCP(m_config);
   }
private:
   unsigned int m_config;
};

struct CloseHandleRAAI{
	using Disposer = std::function<void ()>;
	Disposer m_dispose;
	CloseHandleRAAI(Disposer dispose): m_dispose(dispose){ }
	~CloseHandleRAAI(){ m_dispose(); }
};

void CloseHandleLog(HANDLE h, const std::string& name){
	::CloseHandle(h);
	std::cerr << " [LOG] Handler <" << name << "> closed OK." << std::endl;	
}

auto utf8_encode(const std::wstring &wstr) -> std::string;
auto utf8_decode(const std::string &str) -> std::wstring;

using FileEnumerator = std::function<bool (const std::string&)>;

auto getLastErrorAsString() -> std::string;
auto EnumerateFiles(const std::string& path, FileEnumerator Enumerator) -> int;


int main(int argc, char** argv){
	if(argc < 2){
		std::cerr << "Usage: " << argv[0] << " " << "[PATH]" << std::endl;
		return EXIT_FAILURE;
	}
	auto utf8Console = ConsoleUTF8();
	auto directoryPath = std::string{argv[1]};
	std::cout << "directoryPath = " << directoryPath << "\n";
	int count = 0;	
	// Show 50 first files. 
	int status = EnumerateFiles(
		directoryPath,
		[&count](const auto& file){
			std::cout << " => " << file << "\n";
			if(count++ < 50)
				return true;
			else
				return false;
		});
	if(status != ERROR_SUCCESS){
		std::cout << " => Error code    = " << ::GetLastError() << std::endl;
		std::cout << " => Error message = " << getLastErrorAsString() << std::endl;
		return EXIT_FAILURE;
	}	
	std::puts(" [LOG] End sucessfully");
	return EXIT_SUCCESS;
}
//---------------------------------------------------------//

/** Enumerate files of some directory util enumerator function (callback) returns false. 
  * - path       - Directory path to be listed 
  * - Enumerator - Functions which consumes a string (file listed) and returns bool.
  *                this function returns false when it is no longer interested in 
  *                being called. When it returns false the iteration stops. 
  *
  * - Return    - Returns error code from GetLastError(). If it is successful, 
  *               the function returns ERROR_SUCCESS.
  */
auto EnumerateFiles(const std::string& path, FileEnumerator Enumerator) -> int {
	WIN32_FIND_DATAW fdata;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	// Ensure that resource hFind is always disposed. 
	auto close_hFind = CloseHandleRAAI(std::bind(CloseHandleLog, hFind, "hFile"));
	hFind = FindFirstFileW(utf8_decode(path).c_str(), &fdata);
	if(hFind == INVALID_HANDLE_VALUE)
		return GetLastError();
	do { //Consumer function 
		if(!Enumerator(utf8_encode(fdata.cFileName))) break;
	} while(FindNextFileW(hFind, &fdata) != 0);
	return ERROR_SUCCESS;
}

// Print human-readable description of GetLastError() - Error Code 
// Source: https://stackoverflow.com/questions/1387064
// Requires: <string>, <sstream>, <windows.h>, 
auto getLastErrorAsString() -> std::string {
    //Get the error message, if any.
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0)
        return std::string();
    LPSTR messageBuffer = nullptr;
    static const DWORD flags  =
      FORMAT_MESSAGE_ALLOCATE_BUFFER
      | FORMAT_MESSAGE_FROM_SYSTEM
      | FORMAT_MESSAGE_IGNORE_INSERTS;
    size_t size = FormatMessageA(
		flags,
		NULL,
		errorMessageID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&messageBuffer, 0, NULL);
    auto message = std::string(messageBuffer, size);
	auto ss = std::stringstream(message);
	auto line = std::string{};
	std::getline(ss, line, '\r');
    //Free the buffer.
    LocalFree(messageBuffer);
    return line;  
}


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

// Credits: https://gist.github.com/pezy/8571764
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

