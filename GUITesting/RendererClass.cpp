#include "RendererClass.h"
#include <algorithm>
#include <string>
#include <fstream>
#include "FlightLogData.h"
using namespace std;

void Renderer::clearScreen() {
	if (hashingPass) {
		renderID += 101010;
		return;
	}
	unsigned int* pixel = (unsigned int*)memory; // Set a pointer to the first memory adress in the memory buffer.
	for (int i = 0; i < sHeight; i++)
		for (int j = 0; j < sWidth; j++)
			*pixel++ = 0; // Set every pixel on the screen to 0
}

void Renderer::drawRectF(int x, int y, int width, int height, unsigned int color, RenderMode renderMode) {

	if (hashingPass) {
		renderID += ((long)((x << 16) ^ y) ^ ((long)((width << 16) ^ height) << 32)) + color;
		return;
	}
	
	//If math mode render things from the middle and greater Y means higher up on screen
	if (renderMode == RenderMode::Math) {
		y *= -1;
		y += sHeight / 2;
		x += sWidth / 2;
	}

	//Clipping the rectangle to the screen
	if (x < 0) {
		width += x;
		if (width <= 0)
			return;
		x = 0;
	}
	if (y < 0) {
		height += y;
		if (height <= 0)
			return;
		y = 0;
	}
	if (x + width + 1 > sWidth) {
		width -= x + width + 1 - sWidth;
		if (width <= 0)
			return;
	}
	if (y + height + 1 > sHeight) {
		height -= y + height + 1 - sHeight;
		if (height <= 0)
			return;
	}
	unsigned int* pixel = (unsigned int*)memory + x + y * sWidth; // Set the memory pointer to the top left of the rectangle
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
			*pixel++ = color;
		pixel += sWidth - width; // Go down to the pixel below then back the width of the rectangle
	}
}

void Renderer::drawRect(int x, int y, int width, int height, unsigned int color, RenderMode renderMode) {
	// Same as drawRectF but not filled in
	if (hashingPass) {
		renderID += ((long)((x << 16) ^ y) ^ ((long)((width << 16) ^ height) << 32)) + color;
		return;
	}

	x += (renderMode * sWidth) / 2;
	if (renderMode == RenderMode::Math) {
		y *= -1;
		y += sHeight / 2;
	}

	bool drawLeft = true, drawRight = true, drawBottom = true, drawTop = true;
	if (x < 0) {
		width += x;
		if (width <= 0)
			return;
		x = 0;
		drawLeft = false;
	}
	if (y < 0) {
		height += y;
		if (height <= 0)
			return;
		drawTop = false;
		y = 0;
	}
	if (x + width + 1 > sWidth) {
		width -= x + width + 1 - sWidth;
		if (width <= 0)
			return;
		drawRight = false;
	}
	if (y + height + 1 > sHeight) {
		height -= y + height + 1 - sHeight;
		if (height <= 0)
			return;
		drawBottom = false;
	}
	unsigned int* pixel = (unsigned int*)memory + x + y * sWidth;

	for (int i = 0; i < width - 1; i++)
		if (drawTop) *pixel++ = color;

	for (int i = 0; i < height - 1; i++) {
		if (drawRight) *pixel = color;
		pixel += sWidth;
	}

	for (int i = 0; i < width - 1; i++)
		if (drawBottom) *pixel-- = color;

	for (int i = 0; i < height - 1; i++) {
		if (drawLeft)*pixel = color;
		pixel -= sWidth;
	}
}

void Renderer::drawCircle(int x, int y, int r, int color, RenderMode renderMode) {
	if (hashingPass) {
		renderID += ((long)((x << 16) ^ y) ^ ((long)((r << 16) ^ (long)circleCache) << 32)) + color;
		return;
	}

	x += (renderMode * sWidth) / 2;
	if (renderMode == RenderMode::Math) {
		y *= -1;
		y += sHeight / 2;
	}

	int xStart = x - r, xStop = x + r, yStart = y - r, yStop = y + r;
	if (xStart >= sWidth || yStart >= sHeight || xStop < 0 || yStop < 0) return;

	unsigned int* pixel = (unsigned int*)memory;

	if (xStart > 0) {
		pixel += xStart;
		xStart = 0;
	}

	if (yStart > 0) {
		pixel += yStart * sWidth;
		yStart = 0;
	}
	if (xStop > sWidth)
		xStop = sWidth + r - x;
	else xStop = 2 * r;

	if (yStop > sHeight)
		yStop = sHeight + r - y;
	else yStop = 2 * r;

	if (!circleCache[r]) {
		circleCache[r] = new Shape(r);
	}
	Shape* c = circleCache[r];

	for (int i = -yStart; i < yStop; i++) {
		for (int j = -xStart; j < xStop; j++) {
			// Because only one quarter of the circle i stored in memory it's mirrored to form a whole circle
			if (c->getBitmap((j >= r) ? r * 2 - 1 - j : j, (i >= r) ? r * 2 - 1 - i : i)) *pixel = color;
			pixel++;
		}
		pixel += sWidth - xStop - xStart;
	}
}

void Renderer::drawCircleF(int x, int y, int r, int color, RenderMode renderMode) {
	if (hashingPass) {
		renderID += ((long)((x << 16) ^ y) ^ ((long)((r << 16) ^ (long)circleCache) << 32)) + color;
		return;
	}

	x += (renderMode * sWidth) / 2;
	if (renderMode == RenderMode::Math) {
		y *= -1;
		y += sHeight / 2;
	}

	int xStart = x - r, xStop = x + r, yStart = y - r, yStop = y + r;
	if (xStart >= sWidth || yStart >= sHeight || xStop < 0 || yStop < 0) return;

	unsigned int* pixel = (unsigned int*)memory;

	if (xStart > 0) {
		pixel += xStart;
		xStart = 0;
	}

	if (yStart > 0) {
		pixel += yStart * sWidth;
		yStart = 0;
	}
	if (xStop > sWidth)
		xStop = sWidth + r - x;
	else xStop = 2 * r;

	if (yStop > sHeight)
		yStop = sHeight + r - y;
	else yStop = 2 * r;

	if (!circleCache[r]) {
		circleCache[r] = new Shape(r);
	}

	Shape* c = circleCache[r];

	for (int i = -yStart; i < yStop; i++) {
		for (int j = -xStart; j < xStop; j++) {
			if (c->getBitmapF((j >= r) ? r * 2 - 1 - j : j, (i >= r) ? r * 2 - 1 - i : i)) *pixel = color;
			pixel++;
		}
		pixel += sWidth - xStop - xStart;
	}
}

void Renderer::drawCircle(int x, int y, int color, Shape* circle, RenderMode renderMode) {
	if (hashingPass) {
		renderID += ((long)((x << 16) ^ y) ^ ((long)(((int)circle << 16) ^ circle->getHeight()) << 32)) + color;
		return;
	}
	x += (renderMode * sWidth) / 2;
	if (renderMode == RenderMode::Math) {
		y *= -1;
		y += sHeight / 2;
	}

	if (!circle)
		return;
	int r = circle->getWidth();
	if (x - r > sWidth || y - r > sHeight) return;
	int xOverlap = x + r - sWidth;
	if (xOverlap < 0) xOverlap = 0;
	else xOverlap++;
	int yOverlap = y + r - sHeight;
	if (yOverlap < 0) yOverlap = 0;
	else yOverlap++;

	unsigned int* pixel = (unsigned int*)memory + x - r + (y - r) * sWidth;
	for (int i = 0; i < r * 2 - yOverlap; i++) {
		for (int j = 0; j < r * 2 - xOverlap; j++) {
			if (circle->getBitmap((j >= r) ? r * 2 - 1 - j : j, (i >= r) ? r * 2 - 1 - i : i)) *pixel = color;
			pixel++;
		}
		pixel += sWidth - 2 * r + xOverlap;
	}
}

void Renderer::drawShapeF(int x, int y, Shape* s, int color, RenderMode renderMode) {
	if (hashingPass) {
		renderID += ((long)((x << 16) ^ y) ^ ((long)(((int)s << 16) ^ s->getHeight()) << 32)) + color;
		return;
	}

	x += (renderMode * sWidth) / 2;
	if (renderMode == RenderMode::Math) {
		y *= -1;
		y += sHeight / 2;
	}

	unsigned int* pixel = (unsigned int*)memory + x + y * sWidth;
	if (!s) return;
	for (int i = 0; i < s->getHeight(); i++) {
		for (int j = 0; j < s->getWidth(); j++) {
			if (s->getBitmapF(j, i)) *pixel = color;
			pixel++;
		}
		pixel += sWidth - s->getWidth();
	}
}

void Renderer::drawLine(int x1, int  y1, int  x2, int  y2, int color, RenderMode renderMode) {
	if (hashingPass) {
		renderID += ((long)((x1 << 16) ^ y1) ^ ((long)((x2 << 16) ^ y2) << 32)) + color + (int)renderMode << 24;
		return;
	}

	if (renderMode == RenderMode::Math) {
		x1  += sWidth / 2;
		x2 +=  sWidth / 2;
		y1 *= -1;
		y1 += sHeight / 2;
		y2 *= -1;
		y2 += sHeight / 2;
		
	}

	if (x1 > sWidth - 1 || x1 < 0 || x2 > sWidth - 1 || x2 < 0 || y1 > sHeight - 1 || y1 < 0 || y2 > sHeight - 1 || y2 < 0) return;

	if (x1 == x2) {
		unsigned int* pixel = (unsigned int*)memory + x1 + ((y1 < y2) ? y1 : y2) * sWidth;
		int l = abs(y1 - y2) + 1;
		for (int i = 0; i < l; i++) {
			*pixel = color;
			pixel += sWidth;
		}
		return;
	}

	if (y1 == y2) {
		unsigned int* pixel = (unsigned int*)memory + ((x1 < x2) ? x1 : x2) + y1 * sWidth;
		int l = abs(x1 - x2) + 1;
		for (int i = 0; i < l; i++) {
			*pixel++ = color;
		}
		return;
	}

	if (x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	double k = deltaY / (double)deltaX;
	double a = 0;
	unsigned int* pixel = (unsigned int*)memory + x1 + y1 * sWidth;

	if (k > 1 || k < -1) {
		k = deltaX / (double)deltaY;
		deltaY = abs(deltaY);
		for (int i = 0; i < deltaY + 1; i++) {
			a += k;
			if (a >= 1) {
				pixel++;
				a--;
			}
			if (a <= -1) {
				pixel++;
				a++;
			}
			*pixel = color;
			pixel += k < 0 ? -sWidth : sWidth;
		}
		*((unsigned int*)memory + x2 + y2 * sWidth) = color;
		return;
	}


	for (int i = 0; i < deltaX + 1; i++) {
		a += k;
		if (a >= 1) {
			pixel += sWidth;
			a--;
		}
		if (a <= -1) {
			pixel -= sWidth;
			a++;
		}
		*pixel++ = color;
	}
	*((unsigned int*)memory + x2 + y2 * sWidth) = color;


}

void Renderer::drawPoint(int x, int y, int color, int size, RenderMode renderMode) {
	if (hashingPass) {
		renderID += (((long)x << 16) ^ (long)y) ^ ((((long)size << 16) ^ (long)color) << 32);
		return;
	}

	if (size < 1) 
		return;
	
	drawCircleF(x, y, size, color, renderMode);
	
}

void Renderer::graphFromFile(std::string file, int type, int color)
{
	if (type > 6 || type < 1) return;
	ifstream data(file, ios::binary);
	if (!data.is_open())
		return;

	int length = 0;
	data.read((char*)&length, 4);
	FlightLogData* FLD = new FlightLogData[length];
	data.read(((char*)FLD), (unsigned long)length * 40);

	double max = 0;
	for (int i = 0; i < length; i++)
		if (FLD[i].failRate > max)
			max = FLD[i].failRate;

	if (!(max && length)) return;
	//Type 1 = longestWaitTimeAir  2 = longestWaitTimeGround 3 = averageWaitTimeAir 4 = averageWaitTimeGround 5 = averageTimeWasted 6 = failRate
	int offset = 0;
	if (type > 1)
		offset += 4 + (type - 2) * 8;
		
	double ratioHeight = (sHeight - 100) / max;
	double ratioWidth = ((double)sWidth - 100) / length;
	for (int i = 0; i < length - 1; i++)
		drawLine(i * ratioWidth + 50, -FLD[i].failRate * ratioHeight + sHeight - 50, (i + 1) * ratioWidth + 50, -FLD[i + 1].failRate * ratioHeight + sHeight - 50, color, RenderMode::Game);
	

}

bool Renderer::render() {
	bool r = renderID == prevRenderID;
	prevRenderID = renderID;
	renderID = 0;
	hashingPass = false;
	//return r;
	return false;
}

void Renderer::drawLine(Point3D point1, Point3D point2, int color) {
	// Project 3D points to 2D points
	Point2D a, b;
	if (!point1.Z()) return;

	a.X(((point1.X() * aspectRatio / point1.Z()) + 1) * 0.5 * (double)sWidth);
	a.Y(((point1.Y() / point1.Z()) + 1) * 0.5 * (double)sHeight);
	b.X((((point2.X() * aspectRatio) / point2.Z()) + 1) * 0.5 * (double)sWidth);
	b.Y(((point2.Y() / point2.Z()) + 1) * 0.5 * (double)sHeight);

	drawLine(a, b, 0xffffff, RenderMode::Game);
}

/*void Renderer::drawCube(Point3D point, int length, int color) {
	drawLine(point, point);
}*/