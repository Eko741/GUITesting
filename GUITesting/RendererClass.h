#pragma once
#include "Shape.h"
#include <windows.h>
#include "Point.h"
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
	
	void drawRectF(Point2D point, int width, int height, unsigned int color, RenderMode renderMode = RenderMode::Math) 
	{ drawRectF(point.X(), point.Y(), width, height, color, renderMode); }
	void drawRect(Point2D point, int width, int height, unsigned int color, RenderMode renderMode = RenderMode::Math)
	{ drawRect(point.X(), point.Y(), width, height, color, renderMode); }
	void drawCircle(Point2D point, int r, int color, RenderMode renderMode = RenderMode::Math)
	{ drawCircle(point.X(), point.Y(), r, color, renderMode); }
	void drawCircleF(Point2D point, int r, int color, RenderMode renderMode = RenderMode::Math)
	{ drawCircleF(point.X(), point.Y(), r, color, renderMode); }
	void drawCircle(Point2D point, int color, Shape* circle, RenderMode renderMode = RenderMode::Math)
	{ drawCircle(point.X(), point.Y(), color, circle, renderMode); }
	void drawShapeF(Point2D point, Shape* s, int color, RenderMode renderMode = RenderMode::Math)
	{ drawShapeF(point.X(), point.Y(), s, color, renderMode); }
	void drawLine(Point2D point1, Point2D point2, int color, RenderMode renderMode = RenderMode::Math)
	{ drawLine(point1.X(), point1.Y(), point2.X(), point2.Y(), color, renderMode); }
	void drawPoint(Point2D point, int color = 0xffffff, int size = 1, RenderMode renderMode = RenderMode::Math)
	{ drawPoint(point.X(), point.Y(), color, size, renderMode); }
};
 