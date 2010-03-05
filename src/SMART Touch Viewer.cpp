#include "SMART Touch Viewer.h"
#include "SMART Touch Handler.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE			g_hInst;
TCHAR				g_szTitle[MAX_LOADSTRING];
TCHAR				g_szWindowClass[MAX_LOADSTRING];
SMARTTouchHandler*	g_pHandler;
GdiplusStartupInput g_gdiStartup;
ULONG_PTR           g_gdiToken;


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


// Console entry point
int main(int argc, char** argv)
{
	return WinMain(GetModuleHandle(NULL), NULL, argv[0], SW_MAXIMIZE);
}
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// Initialize GDI+.
	GdiplusStartup(&g_gdiToken, &g_gdiStartup, NULL);


	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, g_szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SMARTTOUCHVIEWER, g_szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	SetConsoleTitle(g_szTitle);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_SMARTTOUCHVIEWER);

	// Main message loop:
	while(true)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				// WM_QUIT was posted, so exit
				return true;
			}

			// Translate and dispatch the message
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(g_gdiToken);

	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= g_szWindowClass;
	wcex.hIconSm		= 0;

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   g_hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(g_szWindowClass, g_szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   //ShowWindow(hWnd, nCmdShow);
   ShowWindow(hWnd, SW_MAXIMIZE);
   UpdateWindow(hWnd);

   // Create handler
   g_pHandler = new SMARTTouchHandler(hWnd);
   if(!g_pHandler)
   {
		return FALSE;
   }
   

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
		case WM_PAINT:
			break;

		case WM_SIZE:
			break;

		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONDOWN:
			g_pHandler->OnMouseDown(lParam, wParam);
			break;

		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_LBUTTONUP:
			g_pHandler->OnMouseUp(lParam, wParam);
			break;

		case WM_MOUSEMOVE:
			g_pHandler->OnMouseMove(lParam, wParam);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			if(message == CSBSDK2::SBSDK_NEW_MESSAGE)
			{
				g_pHandler->MessageReceived();
				return 0;
			}
			
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
