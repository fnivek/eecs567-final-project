#ifndef HELPERS_H
#define HELPERS_H

#include <stdint.h>

typedef struct {
	uint32_t port; 
	uint16_t number;
} Pin;

// 2-dimensional point in a float space
typedef struct {
	float x;
	float y;
} Point2;

// 3-dimensional point in a float space
typedef struct {
	float x;
	float y;
	float z;
} Point3;

Point2 CastPoint2(float x, float y);
Point3 CastPoint3(float x, float y, float z);

#endif
