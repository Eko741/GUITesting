#pragma once
#include "Point2D.h"
class Point3D {
public:
	Point3D(double xC = 0, double yC = 0, double zC = 0) : x(xC), y(yC), z(zC) {}
	Point3D(Point2D a) : x(a.X()), y(a.Y()), z(0) {}
private:
	double x, y, z;
public:
	double X() { return x; }
	double Y() { return y; }
	double Z() { return z; }
	void X(double a) { x = a; }
	void Y(double a) { y = a; }
	void Z(double a) { z = a; }
	double distance(Point3D a);

	const Point3D& operator+(const Point3D & op) const;
	operator Point2D() const;
	const Point3D& operator-()const;
};