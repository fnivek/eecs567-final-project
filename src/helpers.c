#include "helpers.h"

Point2 CastPoint2(float x, float y) {
	Point2 newPoint;
	newPoint.x = x;
	newPoint.y = y;

	return newPoint;
}

Point3 CastPoint3(float x, float y, float z) {
	Point3 newPoint;
	newPoint.x = x;
	newPoint.y = y;
	newPoint.z = z;

	return newPoint;
}

void Point3ToFloatArray(Point3 p, float* array) {
	array[0] = p.x;
	array[1] = p.y;
	array[2] = p.z;
}

void swap(void* a, void* b, int size) {
	uint8_t c[size];

	memcpy(c, a, size);
	memcpy(a, b, size);
	memcpy(b, c, size);
}

