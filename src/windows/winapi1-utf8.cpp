// File:   winapi1-utf8.cpp 
// Brief:  Demonstration about how to use Windows API wiht Unicode UTF-8
// Author: Caio Rodrigues
//
// Dependencies: user32.lib, advapi32.lib
//=======================================================================

#include <iostream>
#include <string>

#include <windows.h>
#include <Lmcons.h>
#include <winbase.h>

// MSVC directive to link dependencies 
// #pragma comment(lib, "user32.lib")
// #pragma comment(lib, "advapi32.lib")

// UTF8 to Unicode Wide wchar_t (UTF16) functions
// taken from the (CREDITS) https://gist.github.com/pezy/8571764 
auto utf8_encode(const std::wstring &wstr) -> std::string;
auto utf8_decode(const std::string &str)   -> std::wstring;

auto messageBox(const std::string& title, const std::string& text) -> void;
auto getUserName() -> std::string;


int main(){
	std::cout << "USER NAME = " << getUserName() << std::endl;
	
	std::string message = "Text with unicode UTF8 - 读写汉字1 - Если указан параметр ";
	std::string title   = "Title in UTF8 الْحُرُوف الْعَرَبِيَّة";	
	messageBox(title, message);
	
	return 0;
}

// ======== Implementations =================//

// Requires: <windows.h>, <winbase.h> and <Lmcons.h>
auto getUserName() -> std::string
{
	DWORD size = UNLEN + 1;
	std::wstring buffer(size, 0x00);
	//BOOL GetUserNameW(LPWSTR lpBuffer, LPDWORD pcbBuffer);
	GetUserNameW(&buffer[0], &size);
	buffer.resize(size - 1);
	return utf8_encode(buffer);
}

auto messageBox(const std::string& title, const std::string& text) -> void 
{
	DWORD const infoboxOptions  =
		MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND;
	::MessageBoxW(
		0
		,utf8_decode(text).c_str()
		,utf8_decode(title).c_str()
		,infoboxOptions
		);
}

// Convert a wide Unicode string to an UTF8 string
// Credits: https://gist.github.com/pezy/8571764
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

// Convert an UTF8 string to a wide Unicode String
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
