#include <math.h>
#include <algorithm>

void clearScreen() {
	unsigned int* pixel = (unsigned int*)renderState.memory;
	for (int i = 0; i < renderState.height; i++)
		for (int j = 0; j < renderState.width; j++)
			*pixel++ = 0;
}

inline int clamp(int a, int min, int max) {
	const double t = a < min ? min : a;
	return t > max ? max : t;
}
void drawRectF(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color){
	if (x > renderState.width || y > renderState.height) return;
	if (x + width > renderState.width) width -= x + width - renderState.width;
	if (y + height > renderState.height) height -= y + height - renderState.height;
	
	unsigned int* pixel = (unsigned int*)renderState.memory + x + y* renderState.width;
	for (int i = 0; i < height + 1; i++) {
		for (int j = 0; j < width + 1; j++)
			*pixel++ = color;
		pixel += renderState.width - width;
	}
}

void drawRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color) {
	if (x > renderState.width || y > renderState.height) return;
	if (x + width > renderState.width) width -= x + width - renderState.width;
	if (y + height > renderState.height) height -= y + height - renderState.height;

	unsigned int* pixel = (unsigned int*)renderState.memory + x + y * renderState.width;
	for (int i = 0; i < width; i++) *pixel++ = color;
	for (int i = 0; i < height; i++) {
		*pixel = color;
		pixel += renderState.width;
	}
	for (int i = 0; i < width; i++) *pixel-- = color;
	for (int i = 0; i < height; i++) {
		*pixel = color;
		pixel -= renderState.width;
	}
}

void drawCircle(int x, int y, int r, int color, Shape **circleCache) {
	if (!circleCache[r]) {
		circleCache[r] = new Shape(r);
	}
	Shape* c = circleCache[r];
	if (x - r > renderState.width || y - r > renderState.height) return;
	int xOverlap = x + r - renderState.width;
	if (xOverlap < 0) xOverlap = 0;
	else xOverlap++;
	int yOverlap = y + r - renderState.height;
	if (yOverlap < 0) yOverlap = 0;
	else yOverlap++;

	unsigned int* pixel = (unsigned int*)renderState.memory + x - r + (y - r) * renderState.width;
	for (int i = 0; i < r * 2 - yOverlap; i++) {
		for (int j = 0; j < r * 2 - xOverlap; j++) {
			if (c->getBitmap((j >= r) ? r * 2 - 1 - j : j, (i >= r) ? r * 2 - 1 - i : i)) *pixel = color;
			pixel++;
		}
		pixel += renderState.width - 2 * r + xOverlap;
	}
}

void drawCircleF(int x, int y, int r, int color, Shape** circleCache) {
	if (!circleCache[r]) {
		circleCache[r] = new Shape(r);
	}
	Shape* c = circleCache[r];
	if (x - r > renderState.width || y - r > renderState.height) return;
	int xOverlap = x + r - renderState.width;
	if (xOverlap < 0) xOverlap = 0;
	else xOverlap++;
	int yOverlap = y + r - renderState.height;
	if (yOverlap < 0) yOverlap = 0;
	else yOverlap++;

	unsigned int* pixel = (unsigned int*)renderState.memory + x - r + (y - r) * renderState.width;
	for (int i = 0; i < r * 2 - yOverlap; i++) {
		for (int j = 0; j < r * 2 - xOverlap; j++) {
			if (c->getBitmapF((j >= r) ? r * 2 - 1 - j : j, (i >= r) ? r * 2 - 1 - i: i)) *pixel = color;
			pixel++;
		}
		pixel += renderState.width - 2 * r + xOverlap;
	}
}

void drawShapeF(int x, int y, Shape* s, int color) {
	unsigned int* pixel = (unsigned int*)renderState.memory + x + y * renderState.width;
	if (!s) return;
	for (int i = 0; i < s->getHeight(); i++) {
		for (int j = 0; j < s->getWidth(); j++) {
			if (s->getBitmapF(j, i)) *pixel = color;
			pixel++;
		}
		pixel += renderState.width - s->getWidth();
	}
}

void drawLine(int x1, int  y1, int  x2, int  y2, int color) {
	unsigned int* pixel = (unsigned int*)renderState.memory + x1 + y1 * renderState.width;
	if (x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}
	if (x1 == x2) {
		int l = abs(y1 - y2) + 1;
		if (y1 > y2) {
			for (int i = 0; i <= l; i++) {
				*pixel = color;
				pixel += renderState.width;
			}
		}
		else {
			for (int i = 0; i <= l; i++) {
				*pixel = color;
				pixel -= renderState.width;
			}
		}
		return;
	}

	if (y1 == y2) {
		return;
	}
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	double k = deltaY / (double)deltaX;
	double a = 0;
	

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
			pixel += k < 0 ? -renderState.width : renderState.width ;
		}
		*((unsigned int*)renderState.memory + x2 + y2 * renderState.width) = color;
		return;
	}
	
	
	for (int i = 0; i < deltaX + 1; i++) {
		a += k;
		if (a >= 1) {
			pixel += renderState.width;
			a--;
		}
		if (a <= -1) {
			pixel -= renderState.width;
			a++;
		}
		*pixel++ = color;
	}
	*((unsigned int*)renderState.memory + x2 + y2 * renderState.width) = color;
	

}