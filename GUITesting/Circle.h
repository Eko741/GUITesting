#pragma once
#include "Figure2D.h"
#include <math.h>
class Circle : public Figure2D {
public:
	Circle(double x1, double y1, double x2, double y2) : Figure2D(x1, y1, x2, y2) {}
	Circle(Point2D& a1, Point2D& b1) : Figure2D(a1, b1) {}
	double area();

private:
};