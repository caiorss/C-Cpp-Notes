#include <iostream>
#include <vector>
#include <string>
#include <cstdio>

#include <windows.h>
#include <urlmon.h>    //Provides URLDownloadToFileW 
#include <wininet.h>   
#include <tchar.h> 

// Only for MSVC
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")

auto launchedFromConsole() -> bool;

// Simple C++ wrapper for the function: URLDownloadToFileA
HRESULT downloadFile(std::string url, std::string file);

void testHTTPRequest();

int main(){
	// ==================  File Download  ========================= //
	//
	HRESULT hr;

	hr = downloadFile("http://httpbin.org/image/jpeg", "image.jpeg");   
	if(SUCCEEDED(hr))
		std::cout << "Download successful OK." << '\n';
	else
		std::cout << "Download failed." << '\n';

	hr = downloadFile("httpxpabin.org/image/jpeg-error", "image2.jpeg");    
	if(SUCCEEDED(hr))
		std::cout << "Download sucessful OK." << '\n';
	else
		std::cout << "Download failed." << '\n';

	//=============== HTTP Protocol ===================================//
	//
	testHTTPRequest();

	if(!launchedFromConsole()){
		std::cout << "Type RETURN to exit" << std::endl;
		std::cin.get();     
	}   
	return 0;
}

auto launchedFromConsole() -> bool {
	DWORD procIDs[2];
	DWORD maxCount = 2;
	DWORD result = GetConsoleProcessList((LPDWORD)procIDs, maxCount);
	return result != 1;
}

HRESULT downloadFile(std::string url, std::string file){
	HRESULT hr = URLDownloadToFileA(
		// (pCaller)    Pointer to IUknown instance (not needed)
		NULL
		// (szURL)      URL to the file that will be downloaded
		,url.c_str()
		// (szFileName) File name that the downloaded file will be saved.
		,file.c_str()
		// (dwReserved) Reserverd - always 0
		,0
		// (lpfnCB)     Status callback
		,NULL       
		);
	return hr;
}

void testHTTPRequest(){
	// Reference: http://www.cplusplus.com/forum/beginner/75062/
	HINTERNET hConnect = InternetOpen("Fake browser",INTERNET_OPEN_TYPE_PRECONFIG,NULL, NULL, 0);
	if(!hConnect){
		std::cerr << "Error: Connection Failure.";
		return;
	}

	HINTERNET hAddr = InternetOpenUrl(
		hConnect
		,"http://www.httpbin.org/get"
		,NULL
		,0
		,INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION
		,0
		);

	if ( !hAddr )
	{
		DWORD errorCode = GetLastError();
		std::cerr << "Failed to open URL" << '\n' << "Error Code = " << errorCode;
		InternetCloseHandle(hConnect);
		return;
	}

	// Buffer size - 4kb or 4096 bytes 
	char  bytesReceived[4096];
	DWORD NumOfBytesReceived = 0;
	while(InternetReadFile(hAddr, bytesReceived, 4096, &NumOfBytesReceived) && NumOfBytesReceived )
	{
		std::cout << bytesReceived;
	}

	InternetCloseHandle(hAddr);
	InternetCloseHandle(hConnect);
     
} // --- EoF testHTTPRequest() --- // 
