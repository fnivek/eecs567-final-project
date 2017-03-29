#include "helpers.h"

Point2 CastPoint2(double x, double y) {
	Point2 newPoint;
	newPoint.x = x;
	newPoint.y = y;

	return newPoint;
}

Point3 CastPoint3(double x, double y, double z) {
	Point3 newPoint;
	newPoint.x = x;
	newPoint.y = y;
	newPoint.z = z;

	return newPoint;
}

void swap(void* a, void* b, int size) {
	uint8_t c[size];

	memcpy(c, a, size);
	memcpy(a, b, size);
	memcpy(b, c, size);
}

