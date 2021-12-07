#include <math.h>
#include <algorithm>

void clearScreen() {
	unsigned int* pixel = (unsigned int*)renderState.memory;
	for (int i = 0; i < renderState.height; i++)
		for (int j = 0; j < renderState.width; j++)
			*pixel++ = 0;
}

void drawRectF(int x, int y, int width, int height, unsigned int color){
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
	if (x + width + 1 > renderState.width) {
		width -= x + width + 1 - renderState.width;
		if (width <= 0)
			return;
	}
	if (y + height + 1 > renderState.height) {
		height -= y + height + 1 - renderState.height; 
		if (height <= 0)
			return;
	}
	unsigned int* pixel = (unsigned int*)renderState.memory + x + y * renderState.width;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
			*pixel++ = color;
		pixel += renderState.width - width;
	}
}

void drawRect(int x, int y, int width, int height, unsigned int color) {
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
	if (x + width + 1 > renderState.width) {
		width -= x + width + 1 - renderState.width;
		if  (width <= 0)
			return;
		drawRight = false;
	}
	if (y + height + 1 > renderState.height) {
		height -= y + height + 1 - renderState.height;
		if (height <= 0)
			return;
		drawBottom = false;
	}
	unsigned int* pixel = (unsigned int*)renderState.memory + x + y * renderState.width;
	
	for (int i = 0; i < width- 1; i++) {
		if (drawTop) *pixel = color;
		pixel++;
	}

	for (int i = 0; i < height - 1; i++) {
		if(drawRight) *pixel = color;
		pixel += renderState.width;
	}

	for (int i = 0; i < width - 1; i++) {
		if(drawBottom) *pixel = color;
		pixel--;
	}
	
	for (int i = 0; i < height - 1; i++) {
		if(drawLeft)*pixel = color;
		pixel -= renderState.width;
	}
}

void drawCircle(int x, int y, int r, int color, Shape **circleCache) {
	int xStart = x - r, xStop = x + r, yStart = y - r, yStop = y + r;
	if (xStart >= renderState.width || yStart >= renderState.height || xStop < 0 || yStop < 0) return;

	unsigned int* pixel = (unsigned int*)renderState.memory;

	if (xStart > 0) {
		pixel += xStart;
		xStart = 0;
	}

	if (yStart > 0) {
		pixel += yStart * renderState.width;
		yStart = 0;
	}
	if (xStop > renderState.width)
		xStop = renderState.width + r - x;
	else xStop = 2 * r;

	if (yStop > renderState.height)
		yStop = renderState.height + r - y;
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
		pixel += renderState.width - xStop - xStart;
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

	if (x1 == x2) {
		unsigned int* pixel = (unsigned int*)renderState.memory + x1 + ((y1 < y2) ? y1 : y2) * renderState.width;
		int l = abs(y1 - y2) + 1;
			for (int i = 0; i < l; i++) {
				*pixel = color;
				pixel += renderState.width;
			}
		return;
	}

	if (y1 == y2) {
		unsigned int* pixel = (unsigned int*)renderState.memory + ((x1 < x2) ? x1 : x2) + y1 * renderState.width;
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
	unsigned int* pixel = (unsigned int*)renderState.memory + x1 + y1 * renderState.width;

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