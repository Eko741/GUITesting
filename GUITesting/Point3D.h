#pragma once
#include "Point2D.h"
class Point3D {
public:
	Point3D(double xC, double yC, double zC) : x(xC), y(yC), z(zC) {}
	Point3D(double xC) : x(xC), y(0), z(0) {}
	Point3D() : x(0), y(0), z(0) {}
	Point3D(Point2D a) : x(a.X()), y(a.Y()), z(0) {}
private:
	double x, y, z;
public:
	double getX() { return x; }
	double getY() { return y; }
	double getZ() { return z; }
	void setX(int a) { x = a; }
	void setY(int a) { y = a; }
	void setZ(int a) { z = a; }
	double distance(Point3D a);
};