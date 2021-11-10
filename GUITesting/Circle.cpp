#include "Circle.h"
#include <math.h>

Circle:: Circle(int r) : radius(r), bitmap(new bool[(r * 2 + 1) * (r * 2 + 1)]), bitmapF(new bool[(r * 2 + 1) * (r * 2 + 1)]) {
	for (int i = 0; i < r * 2 + 1; i++) 
		for (int j = 0; j < r * 2 + 1; j++) {
			double d = sqrt(pow(i - r, 2) + pow(j - r, 2));
			if (d >= r - 1 && d <= r) bitmap[i * (r*2 + 1) + j] = true;
			else bitmap[i * (r * 2 + 1) + j] = false;
			if (d <= r) bitmapF[i * (r * 2 + 1) + j] = true;
			else bitmapF[i * (r * 2 + 1) + j] = false;
			
		}
}
