#include "Point3D.h"
#include <math.h>

double Point3D::distance(Point3D a)
{
	return sqrt(pow(x - a.X(), 2) + pow(y - a.Y(), 2) + pow(z - a.Z(), 2));
}