#include <windows.h>
#include <cstdlib>
#include "Shape.h"
using namespace std;
bool running = true;

struct RenderState {
	int height, width;
	void* memory;

	BITMAPINFO bitmapInfo;
} renderState; 
#include "renderer.cpp" 
LRESULT CALLBACK window_callback(HWND hwnd, UINT   uMsg, WPARAM wParam, LPARAM lParam) { //If the user does something to the window
	LRESULT result = 0; // Creates an emtpy result
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY: { // If the user closes the window
		running = false;
	} break;

	case WM_SIZE: { // If the user changes the size if the window

		RECT rect;
		GetClientRect(hwnd, &rect); //rect gets the dimensions of the window
		renderState.width = rect.right - rect.left; // the width is the upmost of the windown minus th downmost of the window
		renderState.height = rect.bottom - rect.top; // the width is the leftmost of the windown minus th rightmost of the window 

		if (renderState.memory) VirtualFree(renderState.memory, 0, MEM_RELEASE); // if there already is a memory for the window it should be deleted

		int bufferSize = renderState.width * renderState.height * sizeof(unsigned int); // Buffersize is the area, in terms of pixels, times the size of an interger.
		renderState.memory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); // Starts at a location the computer decides becuase it's NULL, size of the allocation, se förklaring https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc, only able to read and write data not excecute code

		renderState.bitmapInfo.bmiHeader.biSize = sizeof(renderState.bitmapInfo.bmiHeader); // How many bytes are requried by the structure
		renderState.bitmapInfo.bmiHeader.biWidth = renderState.width; // The width of the picture
		renderState.bitmapInfo.bmiHeader.biHeight = -renderState.height; // The hieght od the picture, negative because it should render from up to down
		renderState.bitmapInfo.bmiHeader.biPlanes = 1; // "The number of planes for the target device. This value must be set to 1."
		renderState.bitmapInfo.bmiHeader.biBitCount = 32;  // How much memory one pixel requires
		renderState.bitmapInfo.bmiHeader.biCompression = BI_RGB; // Which kind of compression it's using, in this case no compression
	}

	default: {
		result = DefWindowProc(hwnd, uMsg, wParam, lParam); // Does nothing
	}
	}
	return result;
}


int  WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	// Create Window Class
	WNDCLASS windowClass = {}; 
	windowClass.style = CS_HREDRAW | CS_VREDRAW; // Redraw the window when changing the vertical och horizontal size
	windowClass.lpszClassName = "GUI"; // Name of the windowclass
	windowClass.lpfnWndProc = window_callback; //Pointer to callback function
	
	// Register Class
	RegisterClass(&windowClass);
	
	// Craete Window
	// Class name, Window name, Style of the window see https://docs.microsoft.com/en-us/windows/desktop/winmsg/window-styles, posx, posy, width, height, for parent window nullptr, for amnu window nullptr, handle to the window instance, no use in this case nullptr.
	HWND window = CreateWindow(windowClass.lpszClassName, "GUI", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME  | WS_MAXIMIZEBOX | WS_VISIBLE , CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window); // Handle to the decive context from window
	// Game loop
	int x, y;
	int deltaX, deltaY;
	x = 200;
	y = 200;
	deltaY = 1;
	deltaX = 1;
	
	Shape* circleCache[200] = {};
	while (running) {
		// Input
		MSG message;
		// Pointer to the message structure, handle to the window where the messages should be retrieved from, filter message, filter message, messages are removed from the qeue after bein processed
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) { // as long as there are messages it should loop through them			
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		// Simulate
		
		clearScreen();
		drawPoint(200, 200, 0xff66ff);
		drawLine(200, 200, 300, 300, 0xff66ff);
		drawPoint(300, 300, 0xff66ff);
		// Render
		// Puts the pixels in the renderbuffer on the screen
		StretchDIBits(hdc, 0, 0, renderState.width, renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory, &renderState.bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	}
}