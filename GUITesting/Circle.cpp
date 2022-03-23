#include "Circle.h"
double Circle::area()
{
	return (pow(a.X() - b.X(), 2) + pow(a.Y() - b.Y(), 2)) * 3.14159265359;
}
