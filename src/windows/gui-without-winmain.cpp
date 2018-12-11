// File:   gui-without-winmain.cpp 
// Brief:  Minimal Windows GUI Program for Win32 API without WinMain. 
// Author: Caio Rodrigues 
//--------------------------------------------------------------------------------------

#include <iostream>
#include <string> 

#include <windows.h>

LRESULT windowProcedure( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main(int argc, char** argv){
	//---- Get WinMain Parameters ----//

	HINSTANCE hInstance = GetModuleHandle(NULL);
    STARTUPINFO si;
    GetStartupInfo(&si);  
    int nCmdShow = si.wShowWindow;

	// -------------------------//
	OutputDebugString("Starting WinMain Application");
	std::puts("Starting WinMain Application");
	
	//Window class name must be unique 
	const char wincClassName [] = "NameOfWindow";
	
	// Win32 Window class structure
	WNDCLASSEX wc;
	// Win32 message structure 
	MSG Msg;        	
	
	wc.lpszClassName = wincClassName;
	wc.lpfnWndProc = windowProcedure;	
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style  = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); // (HBRUSH) CreateSolidBrush(RGB(10, 20, 30)); // 
	wc.lpszMenuName = NULL;	
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);  

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL,
				   "Window Registration Failed!",
				   "Error!",
				   MB_ICONEXCLAMATION | MB_OK);
		//Error status code 
		return -1;
	}

	std::cout << "Class Registered" << std::endl;
	int width = 500, height = 400;
	int pos_left = 400, pos_top = 100;

	HWND hwnd = CreateWindowA(
		wc.lpszClassName,
		"Title of Window",
		WS_OVERLAPPEDWINDOW,
		pos_left,
		pos_top,
		width,
		height,
		nullptr,
		nullptr,
		hInstance,
		nullptr
		);
	
	constexpr size_t buttonID = 1001;
	
	OutputDebugString(" [INFO] Window created OK");
	if(hwnd == NULL){
		MessageBox(NULL,
				   "Error: Failure to create Window",
				   "Error Report",
				   MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//---- Message Loop ----------//
	while(GetMessage(&Msg, NULL, 0, 0) > 0 ){
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}	
	// Success status code 
	return 0;
}
// ------------ End of Main ------------------------- //


// Window Procedure - Process window messages or events 
LRESULT windowProcedure (
	 HWND   hwnd    // Window Handle (Window object)
	,UINT   msg     // Window Message  
	,WPARAM wParam  // Additional message information
	,LPARAM lParam  // Additional message information
	){

	// Process messages 
	switch(msg)
	{
	case WM_CREATE:
		break;
    case WM_CLOSE:		
		DestroyWindow(hwnd);
		break;
    case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOVE:
		break; 
	case WM_PAINT:
	{
		// GDI - Graphics Devices Interface Here
		//--------------------------------------------
		PAINTSTRUCT ps;
		HDC hdc;
		// std::cerr << " [INFO] Windown painting" << std::endl;
		hdc = BeginPaint(hwnd, &ps);
		std::string text = "Hello world Window!";		
		TextOutA(hdc, 125, 200, text.c_str(), text.size());
		Ellipse(hdc, 100, 100, 160, 160);
		Rectangle(hdc, 100, 100, 160, 160);
		EndPaint(hwnd, &ps);		
	}
	break;
    default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
    }

	return 0;
}
