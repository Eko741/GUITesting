#pragma once
#include "Point3DList.h"
int Point3DList::numberOfLists = 0;

Point3DList::~Point3DList()
{
	numberOfLists--;
	Point3DL* current;
	if (current = start) {
		while (current->prev) {
			current = current->prev;
			delete current->next;
		}
		delete current;
	}
}

void Point3DList::addPoint(const Point3D& point)
{
	if (!length)
		start = new Point3DL(point);

	else {
		Point3DL* last = start;
		for (int i = 0; i < length - 1; i++)
			last = last->next;

		last->next = new Point3DL(point);
		last->next->prev = last;
	}
	length++;
}

void Point3DList::removePoint(const int point)
{
	Point3DL* current = start;

	if (point) {
		for (int i = 0; i < point; i++)
			current = current->next;
		(current->prev)->next = current->next;
	}

	if (length - 1 != point)
		(current->next)->prev = current->prev;

	delete current;
	length--;
}

Point3D& Point3DList::getPoint(const int point) const
{
	Point3DL* current = start;
	for (int i = 0; i < point; i++)
		current = current->next;
	return current->data;
}

Point3D& Point3DList::operator[](const unsigned int point) {
	/*if (point > length - 1)
		return Point3D(0 , 0, 0);*/
	Point3DL* current = start;
	for (int i = 0; i < point; i++)
		current = current->next;
	return current->data;
}

std::ostream* Point3DList::operator>>(std::ostream* cout) const
{
	Point3DL* current = start;
	for (int i = 0; i < length; i++) {
		*cout << current->data.X() << "  " << current->data.Y() << "  " << current->data.Z() << std::endl;
		current = current->next;
	}
	return cout;
}

bool Point3DList::get2DFLData(std::string fileName)
{
	std::ifstream file(fileName, std::ios::binary);
	if (!file.is_open())
		return false;

	int length = 0;
	file.read((char*)&length, 4);
	double* data = new double[length];
	file.read(((char*)data), (unsigned long)length * 8);
	for (int i = 0; i < length; i++)
		addPoint(Point3D(i, data[i], 0));
	return true;
}

