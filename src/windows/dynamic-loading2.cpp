// File:   dynamic-loading2.cpp
// Brief:  Dynamically loads a DLL - shared library at runtime using a class.
// Note:   Demonstration of WINAPIs LoadLibrary, FreeLibrary, GetProcAddress
// Author: Caio Rodrigues
//---------------------------------------------------------------------------
#include <iostream>
#include <string>

#include <windows.h>

//========== File: DLLLoader.hpp =======//
class DLLLoader
{
private:
	HMODULE      m_hLib;
	std::string  m_file;
public:
	DLLLoader(const std::string& file);
	DLLLoader(const DLLLoader&) = delete;	
	DLLLoader(DLLLoader&& rhs);
	~DLLLoader();
	auto operator=(const DLLLoader&) = delete;
	auto operator=(DLLLoader&& rhs);
	
	auto GetFile() const ->  std::string;
	auto IsLoaded() const -> bool;
	operator bool() const;

   template<class FunctionSignature>
	auto GetFunction(const std::string& functionName ) const -> FunctionSignature*
	{
		if(m_hLib == nullptr)
			return nullptr;		
		FARPROC hFunc = ::GetProcAddress(m_hLib, functionName.c_str());
		if(hFunc == nullptr)
			return nullptr;
		return reinterpret_cast<FunctionSignature*>(hFunc);
	}	
};

//========== File: Main.cpp =======//

int main(){
	// Wide-unicode strings 
	std::wstring fileURL = L"http://httpbin.org/image/jpeg";
	std::wstring file1   = L"download-file1.jpeg";
	std::wstring file2   = L"download-file2.jpeg";	

	auto dll = DLLLoader("urlmon.dll");
	if(!dll){
		std::cerr << "[Error] failed to load DLL." << std::endl;
		return EXIT_FAILURE;
	}
	std::cerr << "[INFO] DLL loaded OK." << std::endl;

	// -----------------------------------------------------------------//
	std::cout << "===== Experiment 1 =========== " << std::endl;
	
	auto URLDownloadToFileW =
		dll.GetFunction<HRESULT (LPUNKNOWN,
								 LPCWSTR,
								 LPCWSTR,
								 DWORD,
								 LPBINDSTATUSCALLBACK)>("URLDownloadToFileW");

	if(URLDownloadToFileW == nullptr){
		std::cerr << "[Error] failed to load function. " << std::endl;
		return EXIT_FAILURE;
	}
	std::cerr << "[INFO] Function URLDownloadToFileW loaded OK. " << std::endl;


	HRESULT result1 = URLDownloadToFileW(nullptr, fileURL.c_str(), file1.c_str(), 0, nullptr);
	if(SUCCEEDED(result1))
		std::cerr << " [INFO] Download successful OK. " << std::endl;
	else
		std::cerr << " [ERROR] Download failure. " << std::endl;	   

	// -----------------------------------------------------------------//
	std::cout << "===== Experiment 2 =========== " << std::endl;
	// Note: Calling convention __cdecl can be omitted
	using URLDownloadToFileW_t =
		HRESULT (*) (LPUNKNOWN, LPCWSTR, LPCWSTR, DWORD, LPBINDSTATUSCALLBACK);	
	
	auto URLDownloadToFileW2 =
		dll.GetFunction<URLDownloadToFileW_t>("URLDownloadToFileW");

	HRESULT result2 =
		URLDownloadToFileW(nullptr, fileURL.c_str(), file2.c_str(), 0, nullptr);
	
	if(SUCCEEDED(result2))
		std::cerr << " [INFO] Download successful OK. " << std::endl;
	else
		std::cerr << " [ERROR] Download failure. " << std::endl;	   	
	return 0;
}

///==== Implementations - file: DLLoader.hpp =======//
DLLLoader::DLLLoader(const std::string& file):
	m_file(file),
	m_hLib(::LoadLibraryA(file.c_str()))
{
}

// Move CTOR
DLLLoader::DLLLoader(DLLLoader&& rhs):
	m_hLib(std::move(rhs.m_hLib)),
	m_file(std::move(rhs.m_file))
{   	
}
// Move assignment operator
auto DLLLoader::operator=(DLLLoader&& rhs)		
{
	std::swap(this->m_hLib, rhs.m_hLib);
	std::swap(this->m_file, rhs.m_file);
}
	
DLLLoader::~DLLLoader()
{
	std::cerr << " [INFO] DLL handler released OK." << std::endl;
	if(m_hLib != nullptr)
		::FreeLibrary(m_hLib);
}

auto DLLLoader::GetFile() const ->  std::string
{
	return m_file;
}
	
auto DLLLoader::IsLoaded() const -> bool
{
	return m_hLib == nullptr;
}

DLLLoader::operator bool() const
{
	return m_hLib != nullptr;
}
