#include "matrix_math.h"

// Vector 3
void ProductScalarV3(float scalar, float* v3, float* result) {
	for(uint8_t i = 0; i < 3; ++i) {
		result[i] = scalar * v3[i];
	}
}

void SumV3V3(float* v3_a, float* v3_b, float* result) {
	for(uint8_t i = 0; i < 3; ++i) {
		result[i] = v3_a[i] + v3_b[i];
	}
}


// M3x3 * v3
void ProductM3x3V3(float(*m3x3)[3], float* v3, float* result) {
	for (uint8_t row = 0; row < 3; row++) {
		result[row] = 0;
		for (uint8_t j = 0; j < 3; j++) {
			result[row] += m3x3[row][j] * v3[j];
		}
	}
}


// Matrix 3x3
int InvertM3x3(float(*m3x3)[3], float(*result)[3]) {
	// We have a 3x3, abuse that
	float det = m3x3[0][0] * (m3x3[1][1] * m3x3[2][2] - m3x3[1][2] * m3x3[2][1])
		- m3x3[0][1] * (m3x3[1][0] * m3x3[2][2] - m3x3[1][2] * m3x3[2][0])
		+ m3x3[0][2] * (m3x3[1][0] * m3x3[2][1] - m3x3[1][1] * m3x3[2][0]);
	if (fabs(det) < 0.01) {
		// Too close to singular
		return 1;
	}

	// Store the cofactors in the inverse matrix
	for (uint8_t i = 0; i < 3; i++) {
		for (uint8_t j = 0; j < 3; j++) {
			result[i][j] = m3x3[(i + 1) % 3][(j + 1) % 3] * m3x3[(i + 2) % 3][(j + 2) % 3]
				- m3x3[(i + 1) % 3][(j + 2) % 3] * m3x3[(i + 2) % 3][(j + 1) % 3];
		}
	}

	// Transpose cofactor matrix into the adjugate/adjoint
	TransposeInPlaceM3x3(result);

	// Multiply in 1 / determinant
	ProductScalarM3x3(1 / det, result, result);

	return 0;
}

void TransposeM3x3(float(*m3x3)[3], float(*result)[3]) {
	for (uint8_t i = 0; i < 3; i++) {
		for (uint8_t j = 0; j < 3; j++) {
			result[i][j] = m3x3[j][i];
		}
	}
}

void TransposeInPlaceM3x3(float(*m3x3)[3]) {
	for (uint8_t i = 1; i < 3; i++) {
		for (uint8_t j = i; j < 3; j++) {
			swap(&(m3x3[i][j]), &(m3x3[j][i]), sizeof(float));
		}
	}
}

void ProductScalarM3x3(float scalar, float(*m3x3)[3], float(*result)[3]) {
	for (uint8_t i = 0; i < 3; i++) {
		for (uint8_t j = 0; j < 3; j++) {
			result[i][j] = m3x3[i][j] * scalar;
		}
	}
}