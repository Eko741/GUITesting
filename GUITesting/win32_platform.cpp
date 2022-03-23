#include <windows.h>
#include <Windows.h>
#include <cstdlib>
#include "Shape.h"
#include "RendererClass.h"
#include <chrono>
#include <string>
#include "Point.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Figure2D.h"

//using namespace std;
bool running = true;
Renderer renderer;


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
		renderer.Width(rect.right - rect.left);  // the width is the upmost of the windown minus th downmost of the window
		renderer.Height(rect.bottom - rect.top); // the width is the leftmost of the windown minus th rightmost of the window 
		renderer.updateAspectRatio();

		if (renderer.memory) VirtualFree(renderer.memory, 0, MEM_RELEASE); // if there already is a memory for the window it should be deleted

		// Buffersize is the area, in terms of pixels, times the size of an interger.
		renderer.memory = VirtualAlloc(0, (long)renderer.Width() * (long)renderer.Height() * sizeof(unsigned int), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); // Starts at a location the computer decides becuase it's NULL, size of the allocation, se förklaring https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc, only able to read and write data not excecute code

		renderer.bitmapInfo.bmiHeader.biSize = sizeof(renderer.bitmapInfo.bmiHeader); // How many bytes are requried by the structure
		renderer.bitmapInfo.bmiHeader.biWidth = renderer.Width(); // The width of the picture
		renderer.bitmapInfo.bmiHeader.biHeight = -renderer.Height(); // The hieght od the picture, negative because it should render from up to down
		renderer.bitmapInfo.bmiHeader.biPlanes = 1; // "The number of planes for the target device. This value must be set to 1."
		renderer.bitmapInfo.bmiHeader.biBitCount = 32;  // How much memory one pixel requires
		renderer.bitmapInfo.bmiHeader.biCompression = BI_RGB; // Which kind of compression it's using, in this case no compression
	}

	default: {
		result = DefWindowProc(hwnd, uMsg, wParam, lParam); // When defualt is hit do nothing
	}
	}
	return result;
}


int  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	// Create Window Class
	WNDCLASS windowClass = {};
	windowClass.style = CS_HREDRAW | CS_VREDRAW; // Redraw the window when changing the vertical och horizontal size
	windowClass.lpszClassName = "GUI"; // Name of the windowclass
	windowClass.lpfnWndProc = window_callback; //Pointer to callback function

	// Register Class
	RegisterClass(&windowClass);

	// Craete Window
	// Class name, Window name, Style of the window see https://docs.microsoft.com/en-us/windows/desktop/winmsg/window-styles, posx, posy, width, height, for parent window nullptr, for amnu window nullptr, handle to the window instance, no use in this case nullptr.
	HWND window = CreateWindow(windowClass.lpszClassName, "GUI", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window); // Handle to the decive context from window


	int x = 0;
	int framecounter = 0;
	Point3D a(3, 1, 5);
	Point3D b(4, 1, 5);
	Point3D c(4, 2, 5);
	Point3D d(3, 2, 5);
	Point3D e(3, 1, 6);
	Point3D f(4, 1, 6);
	Point3D g(4, 2, 6);
	Point3D h(3, 2, 6);

	Figure2D* circle = new Circle(100, -100, 140, -140);
	Figure2D* rect = new Rectanglee(120, -210, 140, -250);

	Point3DList list;
	list.get2DFLData("binData.FL");
	
	// Game loop
	while (running) {
		// Input
		MSG message;
		// Pointer to the message structure, handle to the window where the messages should be retrieved from, filter message, filter message, messages are removed from the qeue after bein processed
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) { // as long as there are messages it should loop through them			
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		// Simulate
		renderer.startHashingPass();
		for (int i = 0; i < 2; i++) {
			renderer.clearScreen();


			renderer.drawLine(0, (renderer.Height() - 1) / 2, renderer.Width() - 1, (renderer.Height() - 1) / 2, 0xffffff, RenderMode::Game);
			renderer.drawLine((renderer.Width() - 1) / 2, 0, (renderer.Width() - 1) / 2, renderer.Height() - 1, 0xffffff, RenderMode::Game);
			// Draws a cube
			renderer.drawLine(a, b, 0xffffff);
			renderer.drawLine(b, c, 0xffffff);
			renderer.drawLine(c, d, 0xffffff);
			renderer.drawLine(a, d, 0xffffff);
			renderer.drawLine(a, e, 0xffffff);
			renderer.drawLine(b, f, 0xffffff);
			renderer.drawLine(c, g, 0xffffff);
			renderer.drawLine(d, h, 0xffffff);
			renderer.drawLine(e, f, 0xffffff);
			renderer.drawLine(f, g, 0xffffff);
			renderer.drawLine(g, h, 0xffffff);
			renderer.drawLine(e, h, 0xffffff);

			// Draws two figures
			renderer << *rect;
			renderer << *circle;

			// Draws three graphs.
			renderer.graphFromFile("goodbinData.FL", 1, 0xffaaff); 
			renderer.graphFromFile("asd.FL", 1, 0xaaffaa);
			renderer.graphFromFile("cbinData.FL", 1); 

			// Draws a red point
			renderer.drawPoint(Point2D(200, -200), 2, 0xff1111);

				
			if (!i) {
				if (renderer.render())
					break;
			}
			else
				StretchDIBits(hdc, 0, 0, renderer.Width(), renderer.Height(), 0, 0, renderer.Width(), renderer.Height(), renderer.memory, &renderer.bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
				// Makes the cpu send the vidoe memory data to screen kind of. It also does some work
				// In the middle
		}
	}
}