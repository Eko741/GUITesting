#include "Point3D.h"
#include <math.h>

double Point3D::distance(Point3D a){
	return sqrt(pow(x - a.X(), 2) + pow(y - a.Y(), 2) + pow(z - a.Z(), 2));
}

const Point3D& Point3D::operator+(const Point3D& op) const {
	return Point3D(x + op.x, y + op.y, z + op.z);
}
