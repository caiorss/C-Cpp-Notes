// File:   gui-basic1.cpp 
// Brief:  Minimal Windows GUI Program with Win32 API. 
// Author: Caio Rodrigues 
//
// Build with: 
//   $ cl.exe gui-basic1.cpp /EHsc /Zi /nologo /Fe:GuiApp.exe user32.lib gdi32.lib 
//--------------------------------------------------------------------------------------

#include <iostream>
#include <string> 
#include <map>
#include <set>
#include <sstream>

#include <windows.h>

LRESULT CALLBACK windowProcedure( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

auto WinMessageToString(UINT msg) -> std::string;

int WINAPI WinMain(
	// Handle to current application isntance 
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	// Command line 
	LPSTR     lpCmdLine,
	int       nCmdShow
	){

	OutputDebugString("Starting WinMain Application");
	std::puts("It will not print to Console - Starting WinMain Application");	

	//Window class name must be unique 
	const char wincClassName [] = "NameOfWindow";
	
	// Win32 Window class structure
	WNDCLASSEX wc;
	// Win32 message structure 
	MSG Msg;        	
	
    // Name to identify the class with. 
	wc.lpszClassName = wincClassName;
	//Pointer to the window procedure for this window class. 
	wc.lpfnWndProc = windowProcedure;	
	// 1 - Register Windows Size
	wc.cbSize = sizeof(WNDCLASSEX);
	
	wc.style  = 0;
	//Amount of extra data allocated for this class in memory. Usually 0
	wc.cbClsExtra = 0;
	//Amount of extra data allocated in memory per window of this type. Usually 0. 
	wc.cbWndExtra = 0;
	//Handle to application instance (that we got in the first parameter of WinMain()). 
	wc.hInstance = hInstance;
	// Large (usually 32x32) icon shown when the user presses Alt+Tab. 
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	// Cursor that will be displayed over our window. 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	// Background Brush to set the color of our window. 
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); // (HBRUSH) CreateSolidBrush(RGB(10, 20, 30)); // 
	// Background Brush to set the color of our window. 
	wc.lpszMenuName = NULL;	
	// Small (usually 16x16) icon to show in the taskbar and in the top left corner of the window. 
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);  

	OutputDebugString("Registered Window Class OK.");

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
	OutputDebugString(" [INFO] Window created OK");
	
	if(hwnd == NULL){
		MessageBox(NULL,
				   "Error: Failure to create Window",
				   "Error Report",
				   MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}
	ShowWindow(hwnd, nCmdShow);
	std::cout << "nCmdShow = " << nCmdShow << std::endl;
	UpdateWindow(hwnd);

	//---- Message Loop ----------//
	while(GetMessage(&Msg, NULL, 0, 0) > 0 ){
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	
	OutputDebugString(" [INFO] Exiting application. OK.");	
	// Success status code 
	return 0;
}
// ------------ End of Main ------------------------- //


// Window Procedure - Process window messages or events 
LRESULT CALLBACK windowProcedure (
	 HWND   hwnd    // Window Handle (Window object)
	,UINT   msg     // Window Message  
	,WPARAM wParam  // Additional message information
	,LPARAM lParam  // Additional message information
	){

	// Variable initialized one. 
	static auto ignored_messages = std::set<UINT>{
		WM_MOUSEMOVE, WM_NCHITTEST, WM_SETCURSOR, WM_IME_NOTIFY
	};
	
	// Ignore messages which can flood the logging 
	// if(msg != WM_MOUSEMOVE && msg != WM_NCMOUSEMOVE
	//    && msg != WM_QUIT && msg != WM_NCHITTEST )
	if(ignored_messages.find(msg) == ignored_messages.end())
		OutputDebugString(WinMessageToString(msg).c_str());	
	
	// Process messages 
	switch(msg)
	{
	case WM_CREATE:
		SetWindowTextA(hwnd, "Change Window Title");		
		OutputDebugString(" [INFO] Window created. OK.");
		break;
    case WM_CLOSE:
		OutputDebugString(" [INFO] Window closed. OK.");
		DestroyWindow(hwnd);
		break;
    case WM_DESTROY:
		OutputDebugString(" [INFO] Exiting application. OK.");
		PostQuitMessage(0);
		break;
	case WM_MOVE:
		std::cerr << " [INFO] Move window." << std::endl;
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

/** Get a human-readable description of a Windows message as a string */
auto WinMessageToString(UINT msg) -> std::string {
	/** Database of Windows messages - full list of messages here: 
	 *   https://wiki.winehq.org/List_Of_Windows_Messages
	 */
	static auto WindowMessages = std::map<UINT, std::string>{
		{1, "WM_CREATE"}, {2, "WM_DESTROY"}, {5, "WM_SIZE"},
		{6, "WM_ACTIVATE"}, {13, "WM_SIZE"}, {22, "WM_SETVISIBLE"},
		{23, "WM_ENABLE"},  {29, "WM_PAINT"}, {3, "WM_MOVE"}, {30, "WM_CLOSE"},
		{32, "WM_SETCURSOR"},  {72, "WM_FULLSCREEN"}, {85, "WM_COPYDATA"},
		{512, "WM_MOUSEMOVE"},{132, "WM_NCHITTEST"}, {641, "WM_IME_SETCONTEXT"},
		{8, "WM_KILLFOCUS"}, {134, "WM_NCACTIVATE"}, {28, "WM_ACTIVATEAPP"},
		{160, "WM_NCMOUSEMOVE"}, {161, "WM_NCLBUTTONDOWN"},
		{36, "WM_GETMINMAXINFO"}, {642, "WM_IME_NOTIFY"}, {433, "WM_CAPTURECHANGED"},
		{534, "WM_MOVING"}, {674, "WM_NCMOUSELEAVE"}, {675, "WM_MOUSELEAVE"},
		{532, "WM_SIZING"}, {533, "WM_CAPTURECHANGED"}, {127, "WM_GETICON"},
		{20, "WM_ERASEBKGND"}, {70, "WM_WINDOWPOSCHANGING"}, {71, "WM_WINDOWPOSCHANGED"},
		{273, "WM_COMMAND"}, {274, "WM_SYSCOMMAND"}, {275, "WM_TIMER"}, {513, "WM_LBUTTONDOWN"},
		{514, "WM_LBUTTONUP"}
	};
	// Code for debugging messages sent to Window
	static std::stringstream ss;
	ss.str("");
	ss << " [TRACE] WNPROC Message =>  "
	   << " Code = " << msg;
	if(WindowMessages.find(msg) != WindowMessages.end())
		ss << " ; Message = " << WindowMessages[msg];
	else
		ss << " ; Message = Unknown "; 
	return ss.str();
}

