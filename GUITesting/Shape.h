#pragma once
class Shape {
public:
	Shape(int w, int h);
	Shape(int r);
	~Shape() { delete[] bitmap; delete[] bitmapF; }
	void setBitmapF(int x, int y, bool bit);
	bool getBitmap(int x, int y) { return bitmap[(y *  (height)) + x]; }
	bool getBitmapF(int x, int y) { return bitmapF[(y * (height)) + x]; }
	int getHeight() { return height; }
	int getWidth() { return width; }
private:
	bool* bitmap, *bitmapF; 
	int width, height;
};

// Kan man initiera ett fält till nollor