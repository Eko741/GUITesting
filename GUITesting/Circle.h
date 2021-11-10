#pragma once
class Circle { 
public:
	Circle(int r);
	~Circle() { delete[] bitmap; delete[] bitmapF; }
	int getRadius(){ return radius; }
	bool getBitmap(int x, int y) { return bitmap[(y * (radius * 2 + 1)) + x]; }
	bool getBitmapF(int x, int y) { return bitmapF[(y * (radius * 2 + 1)) + x]; }
private:
	int radius;
	bool* bitmap;
	bool* bitmapF;
};