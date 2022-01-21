#include "Point3D.h"
#include <math.h>

double Point3D::distance(Point3D a)
{
	return sqrt(pow(x - a.getX(), 2) + pow(y - a.getY(), 2) + pow(z - a.getZ(), 2));
}