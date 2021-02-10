// File:   dynamic-loading1.cpp
// Brief:  Dynamically loads a DLL - shared library at runtime.
// Note:   Demonstration of WINAPIs LoadLibrary, FreeLibrary, GetProcAddress
// Author: Caio Rodrigues
//---------------------------------------------------------------------------
#include <iostream>
#include <string>

#include <windows.h>

int main(){

	/** Reference: https://msdn.microsoft.com/en-us/ie/ms775123(v=vs.94)
        Function Signature:          

		HRESULT URLDownloadToFile(
					 LPUNKNOWN            pCaller,
					 LPCTSTR              szURL,
					 LPCTSTR              szFileName,
		  _Reserved_ DWORD                dwReserved,
					 LPBINDSTATUSCALLBACK lpfnCB
		);

      ---------------------------------------------- */

	// Wide-unicode strings 
	std::wstring fileURL = L"http://httpbin.org/image/jpeg";
	std::wstring file1   = L"download-file1.jpeg";
	std::wstring file2   = L"download-file2.jpeg";	
	
	// Wide unicode version for LoadLibrary API
	HMODULE hLib = ::LoadLibraryW(L"urlmon.dll");
	if(hLib == nullptr){
		std::cerr << " [ERROR] Error: failed to load shared library" << std::endl;
		// Early return on Error. 
		return EXIT_FAILURE;		
	}
	std::cerr << " [INFO] DLL Loaded OK" << std::endl;

	std::cout << "============ EXPERIMENT 1 ===================" << std::endl;

	// The function pointer must have the same
	// signature of the function to be dynamically loaded
	//
	// Note: Windows function pointer should include calling convention 
	// If omitted, the default calling convention is __cdcel
	// Possible Calling Conventions: __cdcel, __stdcall, __fastcall and so on.
	//---------------------------

	// C+11 type alias for function pointer 
	using FunptrType =
		HRESULT (__cdecl *)(LPUNKNOWN, LPCWSTR, LPCWSTR, DWORD, LPBINDSTATUSCALLBACK);


	// Alternative 2 for type alias (C++98/03)
	typedef HRESULT (__cdecl * FunptrTypeTypedef)(LPUNKNOWN, LPCWSTR, LPCWSTR, DWORD, LPBINDSTATUSCALLBACK);

	
	FARPROC hFunc = ::GetProcAddress(hLib, "URLDownloadToFileW");
	if(hFunc == nullptr){
		std::cerr << " [Error] Failed to load function from DLL" << std::endl;
		return EXIT_FAILURE;
	}
	std::cerr << " [INFO] Function loaded OK" << std::endl;

	// Functin Pointer 
	FunptrType URLDownloadToFileFunPTR = reinterpret_cast<FunptrType>(hFunc);	
	
	HRESULT result1 = URLDownloadToFileFunPTR(nullptr, fileURL.c_str(), file1.c_str(), 0, nullptr);
	if(SUCCEEDED(result1))
		std::cerr << " [INFO] Download successful OK. " << std::endl;
	else
		std::cerr << " [ERROR] Download failure. " << std::endl;

	
	std::cout << "============ EXPERIMENT 2 ===================" << std::endl;
	// Same cast as reinterpret_cast 
	void* hFunc2 = (void*) ::GetProcAddress(hLib, "URLDownloadToFileW");
	// Omit error checking if(hFunct2 == nullptr){ ... }

	// Note: Calling convention __cdecl can be omitted
	using URLDownloadToFileW_t =
		auto __cdecl (LPUNKNOWN, LPCWSTR, LPCWSTR, DWORD, LPBINDSTATUSCALLBACK) -> HRESULT;
  	//  auto (LPUNKNOWN, LPCWSTR, LPCWSTR, DWORD, LPBINDSTATUSCALLBACK) -> HRESULT;
	
	auto DownloadFile = reinterpret_cast<URLDownloadToFileW_t*>(hFunc2);

	HRESULT hresult2 = DownloadFile(nullptr, fileURL.c_str(), file2.c_str(), 0, nullptr);
	if(SUCCEEDED(result1))
		std::cerr << " [INFO] Download successful OK. " << std::endl;
	else
		std::cerr << " [ERROR] Download failure. " << std::endl;
		
	// WARNING: Code NOT Safe - prone to resource leaking
	// in case of exception or forget to call this function. 
	::FreeLibrary(hLib);
	std::cerr << " [INFO] Application ended gracefully. OK." << std::endl;
	
	return 0;
}
