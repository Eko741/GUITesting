#include "Point2D.h"
#include <math.h>
const double distance(const Point2D& a, const Point2D& b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}