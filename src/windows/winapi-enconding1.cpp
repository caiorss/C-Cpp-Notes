// File:   winapi-enconding1.cpp 
// Brief:  Test Windows API character enconding features.
// Author: Caio Rodrigues
//-----------------------------------------------------------------
#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>

// Enable Unicode version of Windows API compile with -DWITH_UNICODE 
#ifdef WITH_UNICODE
  #define UNICODE
  #define _UNICODE
#endif 

#include <windows.h>
#include <tchar.h>

int main(){
	// ===========> EXPERIMENT 1 - Print to Console ============//
	
	std::cout << "\n ===>>> EXPERIMENT 1: Print to terminal [ANSI/UTF8] <<<=== " << std::endl;
	{
		std::cout  <<  " [COUT] Some text example - указан - 读写汉字1 " << "\n";
		std::wcout << L" [WCOUT] Some text example - указан - 读写汉字1 " << L"\n";	
	}

	// ===========> EXPERIMENT 2 - Print to File ============//
    std::cout << "\n ===>>> EXPERIMENT 2: Write non ANSI Chars to File <<<=== " << "\n";
	std::stringstream ss;
	ss << " Text in Cyrllic Script: Если указан параметр  " << "\n"
	   << " Text in Chinese Script: 读写汉字1 " << "\n"
	   << "\n";

	auto dfile = std::ofstream("logging.txt");
	dfile << ss.str() << std::flush;
	

	// ===========> EXPERIMENT 4 - WinAPI - CreateDirectory ============//
	std::cout << "\n ===>>> EXPERIMENT 3: WinAPI CreateDirectory <<<=== " << std::endl;

	{ // -- ANSI Version of CreateDirectory API 
		bool res;
		res = CreateDirectoryA("directoryANSI-读写汉字1", NULL);
		std::cout << "Successful 1 ?=  " << std::boolalpha << res << std::endl;
	}

	{  // -- Unicode (UTF16) - Wide character version of CreateDirectory API 
		bool res;
		res = CreateDirectoryW(L"directoryWCHAR-读写汉字", NULL);
		std::cout << "Successful 2 ?= " << std::boolalpha << res << std::endl;  		
	}
	{
		// -- TCHAR Version Wide character version of CreateDirectory API 
		bool res;

        #ifdef UNICODE
		std::cout << " [INFO] UNICODE (UTF16) CreateDirectory expanded to CreateDirectoryW" << std::endl;
		#else
   		std::cout << " [INFO] ANSI CreateDirectory expanded to CreateDirectoryA" << std::endl;
		#endif
		res = CreateDirectory(_T("directoryTCHAR-读写汉字"), NULL);
		std::cout << "Successful 3 ?= " << std::boolalpha << res << std::endl;  		
	}


	// ===========> EXPERIMENT 4 - WinAPI - MessageBox ============//
	std::cout << "\n ===>>> EXPERIMENT 4: MessageBox <<<=== " << std::endl;
	
	DWORD const infoboxOptions  = MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND;
	// Text in UTF8 => Note => Windows API doesn't work with UTF8
	// or multi-byte characters as the API treats the chars as they were ANSI.
	char narrowText []  = "한국어 (Korean) / 读写汉字 - 学中文 (Chinese)";
	// Unicode text in UTF16 
	wchar_t wideText []  = L"한국어 (Korean) /  读写汉字 - 学中文 (Chinese)";
	MessageBoxA( 0, narrowText, "ANSI (narrow) text:", infoboxOptions );    
    MessageBoxW( 0, wideText, L"Unicode (wide) text:", infoboxOptions );
	
	return 0;
}
