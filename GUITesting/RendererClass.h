#pragma once
#include "Shape.h"
#include <windows.h>
enum RenderMode {
	Game, Math
};
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


	void clearScreen();
	void drawRectF(int x, int y, int width, int height, unsigned int color, RenderMode renderMode = RenderMode::Math);
	void drawRect(int x, int y, int width, int height, unsigned int color, RenderMode renderMode = RenderMode::Math);
	void drawCircle(int x, int y, int r, int color, RenderMode renderMode = RenderMode::Math);
	void drawCircleF(int x, int y, int r, int color, RenderMode renderMode = RenderMode::Math);
	void drawCircle(int x, int y, int color, Shape* circle, RenderMode renderMode = RenderMode::Math);
	void drawShapeF(int x, int y, Shape* s, int color, RenderMode renderMode = RenderMode::Math);
	void drawLine(int x1, int  y1, int  x2, int  y2, int color, RenderMode renderMode = RenderMode::Math);
	void drawPoint(int x, int y, int color, int size = 1, RenderMode renderMode = RenderMode::Math);
};
 