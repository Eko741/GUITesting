#pragma once
#pragma once
#include "Point3D.h"
#include <iostream>
#include <fstream>

class Point3DL {
public:
	Point3DL(const Point3D& d) : data(d) {}

	Point3D data;
	Point3DL* next = nullptr;
	Point3DL* prev = nullptr;
};

class Point3DList {
public:
	Point3DList() { numberOfLists++; }
	~Point3DList();
	void addPoint(const Point3D& point);
	void removePoint(const int point);
	Point3D& getPoint(const int point) const;
	int Length() { return length; }

	const static int getNumberOfLists() {
		return numberOfLists;
	}

	std::ostream* operator>>(std::ostream* cout)const;
	bool get2DFLData(std::string fileName);
	Point3D& operator[](const unsigned int point);


private:
	int length = 0;
	Point3DL* start = nullptr;
	static int numberOfLists;
};