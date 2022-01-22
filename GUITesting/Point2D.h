#pragma once
class Point2D {
public:
	Point2D(double xC = 0, double yC = 0) : x(xC), y(yC) {}
private:
	double x, y;
public:
	double X() { return x; }
	double Y() { return y; }
	void X(int a) { x = a; }
	void Y(int a) { y = a; }
};