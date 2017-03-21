#ifndef HELPERS_H
#define HELPERS_H

#include <stdint.h>

typedef struct {
	uint32_t port; 
	uint16_t number;
} Pin;

// 2-dimensional point in a float space
typedef struct {
	double x;
	double y;
} Point2;

// 3-dimensional point in a float space
typedef struct {
	double x;
	double y;
	double z;
} Point3;

Point2 CastPoint2(double x, double y);
Point3 CastPoint3(double x, double y, double z);

#endif
