#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H

#include <stdint.h>
#include <math.h>

#include "helpers.h"

// Vector 3
void ProductScalarV3(float scalar, float* v3, float* result);
void SumV3V3(float* v3_a, float* v3_b, float* result);

// M3x3 * v3
void ProductM3x3V3(float(*m3x3)[3], float* v3, float* result);

// Matrix 3x3
int InvertM3x3(float(*m3x3)[3], float(*result)[3]);
void TransposeM3x3(float(*m3x3)[3], float(*result)[3]);
void TransposeInPlaceM3x3(float(*m3x3)[3]);
void ProductScalarM3x3(float scalar, float(*m3x3)[3], float(*result)[3]);

#endif