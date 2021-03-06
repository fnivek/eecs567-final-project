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

void WriteV3(float* v3) {
	char buf[64];
	snprintf(buf, 64, "[%.3g, %.3g, %.3g]", v3[0], v3[1], v3[2]);
	UsbWriteString(buf);
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
	float det = m3x3[0][0]*m3x3[1][1]*m3x3[2][2] -
				m3x3[0][0]*m3x3[1][2]*m3x3[2][1] -
				m3x3[0][1]*m3x3[1][0]*m3x3[2][2] +
				m3x3[0][1]*m3x3[1][2]*m3x3[2][0] +
				m3x3[0][2]*m3x3[1][0]*m3x3[2][1] -
				m3x3[0][2]*m3x3[1][1]*m3x3[2][0];

	if (fabs(det) < 0.0001) {
		// Too close to singular
		return 1;
	}

	result[0][0] =  (m3x3[1][1]*m3x3[2][2] - m3x3[1][2]*m3x3[2][1])/det;
	result[0][1] = -(m3x3[0][1]*m3x3[2][2] - m3x3[0][2]*m3x3[2][1])/det;
	result[0][2] =  (m3x3[0][1]*m3x3[1][2] - m3x3[0][2]*m3x3[1][1])/det;
	result[1][0] = -(m3x3[1][0]*m3x3[2][2] - m3x3[1][2]*m3x3[2][0])/det;
	result[1][1] =  (m3x3[0][0]*m3x3[2][2] - m3x3[0][2]*m3x3[2][0])/det;
	result[1][2] = -(m3x3[0][0]*m3x3[1][2] - m3x3[0][2]*m3x3[1][0])/det;
	result[2][0] =  (m3x3[1][0]*m3x3[2][1] - m3x3[1][1]*m3x3[2][0])/det;
	result[2][1] = -(m3x3[0][0]*m3x3[2][1] - m3x3[0][1]*m3x3[2][0])/det;
	result[2][2] =  (m3x3[0][0]*m3x3[1][1] - m3x3[0][1]*m3x3[1][0])/det;

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

void WriteM3x3(float(*m3x3)[3]) {
	char buf[64];
	for(uint8_t i = 0; i < 3; ++i) {
		int chars = snprintf(buf, 64, "[%.3g, %.3g, %.3g],\n", m3x3[i][0], m3x3[i][1], m3x3[i][2]);
		if(i == 2 && chars < 64 && chars > 0) {
			// Remove the last \n
			buf[chars - 1] = '\0';
		}
		UsbWriteString(buf);
	}
}