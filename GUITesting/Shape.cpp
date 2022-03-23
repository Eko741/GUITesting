#include "Shape.h"
#include <math.h>

// Creates the top left half of a circle in a bitmap
Shape::Shape(int r) : width(r), height(r), bitmap(new bool[r * r]), bitmapF(new bool[r * r]) {
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < r; j++) {
			double d = sqrt(pow(i - r  + 1, 2) + pow(j - r + 1, 2));
			if (d >= r - 1 && d <= r) bitmap[i * r + j] = true;
			else bitmap[i * r + j] = false;
			if (d <= r) bitmapF[i * r + j] = true;
			else bitmapF[i * r + j] = false;
		}
	}
}


Shape::Shape(int w, int h) : bitmap(new bool[w * h]), bitmapF(new bool[w * h]), width(w), height(h) {
	for (int i = 0; i < w * h; i++) { bitmap[i] = false; bitmapF[i] = false; }
};

void Shape::setBitmapF(int x, int y, bool bit) {
	bitmapF[x + y * width] = bit;
}