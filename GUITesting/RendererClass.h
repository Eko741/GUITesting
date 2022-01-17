#pragma once
#include "Shape.h"
#include <windows.h>
class Renderer {
public:
	BITMAPINFO bitmapInfo;
	void* memory;
private:
	int height, width;
	bool hashingPass;
	long renderID = 0;
	long prevRenderID = 0;
	Shape* circleCache[200];
public:
	int Height() { return height; }
	int Width() { return width; }
	void Height(int h) { height = h; }
	void Width(int w) { width = w; }
	void startHashingPass() { hashingPass = true; }
	bool render();


	void generateFunctionID(int fucnID, int a = 100, int b = 100, int c = 100, int d = 100, int e = 100, int f = 100){}
	void clearScreen();
	void drawRectF(int x, int y, int width, int height, unsigned int color);
	void drawRect(int x, int y, int width, int height, unsigned int color);
	void drawCircle(int x, int y, int r, int color);
	void drawCircleF(int x, int y, int r, int color);
	void drawCircle(int x, int y, int color, Shape* circle);
	void drawShapeF(int x, int y, Shape* s, int color);
	void drawLine(int x1, int  y1, int  x2, int  y2, int color);
	void drawPoint(int x, int y, int color, int size = 1);
};
 