#include "RendererClass.h"
#include <algorithm>
#include <string>

void Renderer::clearScreen() {
	if (hashingPass) {
		renderID += 101010;
		return;
	}
	unsigned int* pixel = (unsigned int*)memory;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			*pixel++ = 0;
}

void Renderer::drawRectF(int x, int y, int width, int height, unsigned int color, RenderMode renderMode) {

	if (hashingPass) {
		renderID += ((long)((x << 16) ^ y) ^ ((long)((width << 16) ^ height) << 32)) + color;
		return;
	}

	x += (renderMode * width) / 2;
	y += (renderMode * height) / 2;

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
	if (x + width + 1 > width) {
		width -= x + width + 1 - width;
		if (width <= 0)
			return;
	}
	if (y + height + 1 > height) {
		height -= y + height + 1 - height;
		if (height <= 0)
			return;
	}
	unsigned int* pixel = (unsigned int*)memory + x + y * width;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
			*pixel++ = color;
		pixel += width - width;
	}
}

void Renderer::drawRect(int x, int y, int width, int height, unsigned int color, RenderMode renderMode) {
	if (hashingPass) {
		renderID += ((long)((x << 16) ^ y) ^ ((long)((width << 16) ^ height) << 32)) + color;
		return;
	}

	x += (renderMode * width) / 2;
	y += (renderMode * height) / 2;

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
	if (x + width + 1 > width) {
		width -= x + width + 1 - width;
		if (width <= 0)
			return;
		drawRight = false;
	}
	if (y + height + 1 > height) {
		height -= y + height + 1 - height;
		if (height <= 0)
			return;
		drawBottom = false;
	}
	unsigned int* pixel = (unsigned int*)memory + x + y * width;

	for (int i = 0; i < width - 1; i++) {
		if (drawTop) *pixel = color;
		pixel++;
	}

	for (int i = 0; i < height - 1; i++) {
		if (drawRight) *pixel = color;
		pixel += width;
	}

	for (int i = 0; i < width - 1; i++) {
		if (drawBottom) *pixel = color;
		pixel--;
	}

	for (int i = 0; i < height - 1; i++) {
		if (drawLeft)*pixel = color;
		pixel -= width;
	}
}

void Renderer::drawCircle(int x, int y, int r, int color, RenderMode renderMode) {
	if (hashingPass) {
		renderID += ((long)((x << 16) ^ y) ^ ((long)((r << 16) ^ (long)circleCache) << 32)) + color;
		return;
	}

	x += (renderMode * width) / 2;
	y += (renderMode * height) / 2;

	int xStart = x - r, xStop = x + r, yStart = y - r, yStop = y + r;
	if (xStart >= width || yStart >= height || xStop < 0 || yStop < 0) return;

	unsigned int* pixel = (unsigned int*)memory;

	if (xStart > 0) {
		pixel += xStart;
		xStart = 0;
	}

	if (yStart > 0) {
		pixel += yStart * width;
		yStart = 0;
	}
	if (xStop > width)
		xStop = width + r - x;
	else xStop = 2 * r;

	if (yStop > height)
		yStop = height + r - y;
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
		pixel += width - xStop - xStart;
	}
}

void Renderer::drawCircleF(int x, int y, int r, int color, RenderMode renderMode) {
	if (hashingPass) {
		renderID += ((long)((x << 16) ^ y) ^ ((long)((r << 16) ^ (long)circleCache) << 32)) + color;
		return;
	}

	x += (renderMode * width) / 2;
	y += (renderMode * height) / 2;

	int xStart = x - r, xStop = x + r, yStart = y - r, yStop = y + r;
	if (xStart >= width || yStart >= height || xStop < 0 || yStop < 0) return;

	unsigned int* pixel = (unsigned int*)memory;

	if (xStart > 0) {
		pixel += xStart;
		xStart = 0;
	}

	if (yStart > 0) {
		pixel += yStart * width;
		yStart = 0;
	}
	if (xStop > width)
		xStop = width + r - x;
	else xStop = 2 * r;

	if (yStop > height)
		yStop = height + r - y;
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
		pixel += width - xStop - xStart;
	}
}

void Renderer::drawCircle(int x, int y, int color, Shape* circle, RenderMode renderMode) {
	if (hashingPass) {
		renderID += ((long)((x << 16) ^ y) ^ ((long)(((int)circle << 16) ^ circle->getHeight()) << 32)) + color;
		return;
	}
	x += (renderMode * width) / 2;
	y += (renderMode * height) / 2;

	if (!circle)
		return;
	int r = circle->getWidth();
	if (x - r > width || y - r > height) return;
	int xOverlap = x + r - width;
	if (xOverlap < 0) xOverlap = 0;
	else xOverlap++;
	int yOverlap = y + r - height;
	if (yOverlap < 0) yOverlap = 0;
	else yOverlap++;

	unsigned int* pixel = (unsigned int*)memory + x - r + (y - r) * width;
	for (int i = 0; i < r * 2 - yOverlap; i++) {
		for (int j = 0; j < r * 2 - xOverlap; j++) {
			if (circle->getBitmap((j >= r) ? r * 2 - 1 - j : j, (i >= r) ? r * 2 - 1 - i : i)) *pixel = color;
			pixel++;
		}
		pixel += width - 2 * r + xOverlap;
	}
}

void Renderer::drawShapeF(int x, int y, Shape* s, int color, RenderMode renderMode) {
	if (hashingPass) {
		renderID += ((long)((x << 16) ^ y) ^ ((long)(((int)s << 16) ^ s->getHeight()) << 32)) + color;
		return;
	}

	x += (renderMode * width) / 2;
	y += (renderMode * height) / 2;

	unsigned int* pixel = (unsigned int*)memory + x + y * width;
	if (!s) return;
	for (int i = 0; i < s->getHeight(); i++) {
		for (int j = 0; j < s->getWidth(); j++) {
			if (s->getBitmapF(j, i)) *pixel = color;
			pixel++;
		}
		pixel += width - s->getWidth();
	}
}

void Renderer::drawLine(int x1, int  y1, int  x2, int  y2, int color, RenderMode renderMode) {
	if (hashingPass) {
		renderID += ((long)((x1 << 16) ^ y1) ^ ((long)((x2 << 16) ^ y2) << 32)) + color;
		return;
	}
	x1 += (renderMode * width) / 2;
	y1 += (renderMode * height) / 2;
	x2 += (renderMode * width) / 2;
	y2 += (renderMode * height) / 2;

	if (x1 > width || x1 < 0 || x2 > width || x2 < 0 || y1 > height || y1 < 0 || y2 > height || y2 < 0) return;

	if (x1 == x2) {
		unsigned int* pixel = (unsigned int*)memory + x1 + ((y1 < y2) ? y1 : y2) * width;
		int l = abs(y1 - y2) + 1;
		for (int i = 0; i < l; i++) {
			*pixel = color;
			pixel += width;
		}
		return;
	}

	if (y1 == y2) {
		unsigned int* pixel = (unsigned int*)memory + ((x1 < x2) ? x1 : x2) + y1 * width;
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
	unsigned int* pixel = (unsigned int*)memory + x1 + y1 * width;

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
			pixel += k < 0 ? -width : width;
		}
		*((unsigned int*)memory + x2 + y2 * width) = color;
		return;
	}


	for (int i = 0; i < deltaX + 1; i++) {
		a += k;
		if (a >= 1) {
			pixel += width;
			a--;
		}
		if (a <= -1) {
			pixel -= width;
			a++;
		}
		*pixel++ = color;
	}
	*((unsigned int*)memory + x2 + y2 * width) = color;


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

bool Renderer::render() {
	bool r = renderID == prevRenderID;
	prevRenderID = renderID;
	renderID = 0;
	hashingPass = false;
	return r;
}