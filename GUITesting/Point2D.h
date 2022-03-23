#pragma once
class Point2D {
public:
	Point2D(double xC = 0, double yC = 0) : x(xC), y(yC) {}
	//Point2D(Point3D a) : x(a.X()), y(a.Y()){}
private:
	double x, y;
public:
	double X() { return x; }
	double Y() { return y; }
	void X(double a) { x = a; }
	void Y(double a) { y = a; }
	friend const double distance(const Point2D& a, const Point2D& b);
};