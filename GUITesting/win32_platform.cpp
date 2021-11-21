#include <windows.h>
#include <cstdlib>
#include "Circle.h"
using namespace std;
bool running = true;

struct RenderState {
	int height, width;
	void* memory;

	BITMAPINFO bitmapInfo;
} renderState; 
Circle* circleCache[200] = {};
#include "renderer.cpp"

LRESULT CALLBACK window_callback(HWND   hwnd, UINT   uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY: {
		running = false;
	} break;

	case WM_SIZE: {

		RECT rect;
		GetClientRect(hwnd, &rect);
		renderState.width = rect.right - rect.left;
		renderState.height = rect.bottom - rect.top;

		if (renderState.memory) VirtualFree(renderState.memory, 0, MEM_RELEASE);

		int bufferSize = renderState.width * renderState.height * sizeof(unsigned int);
		renderState.memory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		renderState.bitmapInfo.bmiHeader.biSize = sizeof(renderState.bitmapInfo.bmiHeader);
		renderState.bitmapInfo.bmiHeader.biWidth = renderState.width;
		renderState.bitmapInfo.bmiHeader.biHeight = -renderState.height;
		renderState.bitmapInfo.bmiHeader.biPlanes = 1;
		renderState.bitmapInfo.bmiHeader.biBitCount = 32;
		renderState.bitmapInfo.bmiHeader.biCompression = BI_RGB;
	}

	default: {
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
	return result;
}


int  WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	// Create Window Class
	WNDCLASS windowClass = {};
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpszClassName = "GUI";
	windowClass.lpfnWndProc = window_callback;
	
	// Register Class
	RegisterClass(&windowClass);
	
	// Craete Window
	HWND window = CreateWindow(windowClass.lpszClassName, "GUI", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME  | WS_MAXIMIZEBOX | WS_VISIBLE , CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);
	// Game loop
	int x = 200;
	Circle c(100);
	while (running) {
		// Input
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		// Simulate
		
		clearScreen();
		drawRect(300, 100, 75, 75, 0xffaaff);
		
		drawRectF(400, 300, 201, 1, 0xffffff);
		
		drawCircle(500, 300, 50, 0xaaffff);
		drawCircleF(500, 500, 100, 0xffaaff);
		
		x++;
		// Render
		StretchDIBits(hdc, 0, 0, renderState.width, renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory, &renderState.bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	}
	
}