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
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
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

void drawCircleF(unsigned int x, unsigned int y, int radius, unsigned int color) {
	if (x - radius > renderState.width || y - radius > renderState.height) return;
	int xOverlap = x + radius - renderState.width;
	if (xOverlap < 0) xOverlap = 0;
	else xOverlap++;
	int yOverlap = y + radius - renderState.height;
	if (yOverlap < 0) yOverlap = 0;
	else yOverlap++;

	unsigned int* pixel = (unsigned int*)renderState.memory + x - radius + (y - radius) * renderState.width;
	for (int i = 0; i < radius * 2 + 1 - yOverlap; i++) {
		for (int j = 0; j < radius * 2 + 1 - xOverlap; j++) {
			if (sqrt(pow(i - radius, 2) + pow(j - radius, 2)) <= radius) *pixel = color;
			pixel++;
		}
		pixel += renderState.width - 2*radius - 1 + xOverlap;
	}
}
void drawCircle(unsigned int x, unsigned int y, int radius, unsigned int color) {
	if (x - radius > renderState.width || y - radius > renderState.height) return;
	int xOverlap = x + radius - renderState.width;
	if (xOverlap < 0) xOverlap = 0;
	else xOverlap++;
	int yOverlap = y + radius - renderState.height;
	if (yOverlap < 0) yOverlap = 0;
	else yOverlap++;
	
	unsigned int* pixel = (unsigned int*)renderState.memory + x - radius + (y - radius) * renderState.width;
	for (int i = 0; i < radius * 2  + 1 - yOverlap; i++) {
		for (int j = 0; j < radius * 2 + 1 - xOverlap; j++) {
			double d = sqrt(pow(i - radius, 2) + pow(j - radius, 2));
			if (d >= radius - 1 && d <= radius + 1) *pixel = color;
			pixel++;
		}
		pixel += renderState.width - 2 * radius - 1 + xOverlap;
	}
}

void drawCircle(int x, int y, int r, int color) {
	if (!circleCache[r]) {
		circleCache[r] = new Circle(r);
	}
	Circle* c = circleCache[r];
	if (x - r > renderState.width || y - r > renderState.height) return;
	int xOverlap = x + r - renderState.width;
	if (xOverlap < 0) xOverlap = 0;
	else xOverlap++;
	int yOverlap = y + r - renderState.height;
	if (yOverlap < 0) yOverlap = 0;
	else yOverlap++;

	unsigned int* pixel = (unsigned int*)renderState.memory + x - r + (y - r) * renderState.width;
	for (int i = 0; i < r * 2 + 1 - yOverlap; i++) {
		for (int j = 0; j < r * 2 + 1 - xOverlap; j++) {
			if (c->getBitmap(j, i)) *pixel = color;
			pixel++;
		}
		pixel += renderState.width - 2 * r - 1 + xOverlap;
	}
}

void drawCircleF(int x, int y, int r, int color) {
	if (!circleCache[r]) {
		circleCache[r] = new Circle(r);
	}
	Circle* c = circleCache[r];
	if (x - r > renderState.width || y - r > renderState.height) return;
	int xOverlap = x + r - renderState.width;
	if (xOverlap < 0) xOverlap = 0;
	else xOverlap++;
	int yOverlap = y + r - renderState.height;
	if (yOverlap < 0) yOverlap = 0;
	else yOverlap++;

	unsigned int* pixel = (unsigned int*)renderState.memory + x - r + (y - r) * renderState.width;
	for (int i = 0; i < r * 2 + 1 - yOverlap; i++) {
		char f = false;
		bool p = false;
		for (int j = 0; j < r * 2 + 1 - xOverlap; j++) {
			if (c->getBitmapF(j, i)) *pixel = color;
			pixel++;
		}
			pixel += renderState.width - 2 * r - 1 + xOverlap;
		
	}
}

void drawLine(int x1, int  y1, int  x2, int  y2, int color) {
	unsigned int* pixel = (unsigned int*)renderState.memory + x1 + y1 * renderState.width;
	if (x1 == x2) {
		if (y1 > y2) {
			for (int i = 0; i <= abs(y1 - y2); i++) {
				*pixel = color;
				pixel += renderState.width;
			}
		}
		else {
			for (int i = 0; i <= abs(y1 - y2); i++) {
				*pixel = color;
				pixel -= renderState.width;
			}
		}
		return;
	}
	if (x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	int k = deltaY / deltaX;
	int deltaK = deltaY % deltaX;
	if (deltaK == 0) deltaK = 1;

	if (k == 0) {
		k = deltaX / deltaY;
		deltaK = deltaX % deltaY;
		for (int i = 0; i < deltaY; i++) {
			for (int j = 0; j < deltaX; j++) {
				*pixel++ = color;
			}
			pixel += k < 0 ? -renderState.width : renderState.width;
		}
		return;
	}

	for (int i = 0; i < deltaY; i++) {
		for (int j = 0; j < k; j++) {
			*pixel++ = color;
			if (deltaY % (deltaY / deltaK) == 0) {
				*pixel++ = color;
			}
		}
		pixel += k > 0 ? -renderState.width : renderState.width;
	}
}