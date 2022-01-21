#pragma once
class Point2D {
public:
	Point2D(double xC, double yC) : x(xC), y(yC) {}
private:
	double x, y;
public:
	double X() { return x; }
	double Y() { return y; }
	void X(int a) { x = a; }
	void Y(int a) { y = a; }
	double rX() { return x; }
	double rY() { return y; }
	void rX(int a) { x = a; }
	void rY(int a) { y = a; }
};