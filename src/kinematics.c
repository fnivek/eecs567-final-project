#include "kinematics.h"


void SetupKinematics(void) {
	// TODO: base this on FK?
	_KinematicsTarget 	= CastPoint3(0, 0, 0);
	_KinematicsEnable 	= 0;
	_KinematicsType		= kInverseJacobian;
}

/*----- Jacobians -----*/
void KinematicsComputeJacobian(double* angles, double** jacobian) {
	// Assume the Jacobian matrix is 3x3

	
}

int KinematicsInvertJacobian(double** jacobian, double** inverse) {
	double** J = jacobian;
	double det;

	// We have a 3x3 Jacobian, abuse that
	det = J[0][0] * (J[1][1] * J[2][2] - J[1][2] * J[2][1])
		- J[0][1] * (J[1][0] * J[2][2] - J[1][2] * J[2][0])
		+ J[0][2] * (J[1][0] * J[2][1] - J[1][1] * J[2][0]);
	if (fabs(det) < 0.01) {
		// Too close to singular
		return 1;
	}

	// Store the cofactors in the inverse matrix
	for (uint8_t i = 0; i < 3; i++) {
		for (uint8_t j = 0; j < 3; j++) {
			inverse[i][j] = J[(i + 1) % 3][(j + 1) % 3] * J[(i + 2) % 3][(j + 2) % 3]
				- J[(i + 1) % 3][(j + 2) % 3] * J[(i + 2) % 3][(j + 1) % 3];
		}
	}

	// Transpose cofactor matrix into the adjugate/adjoint
	KinematicsTranspose(inverse);

	// Multiply in 1 / determinant
	for (uint8_t i = 0; i < 3; i++) {
		for (uint8_t j = 0; j < 3; j++) {
			inverse[i][j] *= (1 / det);
		}
	}

	return 0;
}

void KinematicsTranspose(double** mat) {
	for (uint8_t i = 1; i < 3; i++) {
		for (uint8_t j = i; j < 3; j++) {
			swap(&(mat[i][j]), &(mat[j][i]), sizeof(double));
		}
	}
}

/*----- Forward Kinematics -----*/
Point3 KinematicsGetPosition(double* angles, uint8_t nAngles);
Point3 KinematicsGetArmPosition(void);


/*----- Inverse Kinematics -----*/
void KinematicsSetControlEnable(uint8_t enable) {
	_KinematicsEnable = enable;
}

void KinematicsSetControlType(KinematicsControlType control) {
	_KinematicsType = control;
}

void KinematicsSetTargetPosition(Point3 xd) {
	_KinematicsTarget = xd;
}

void KinematicsAdvanceControl(void) {
	if (_KinematicsEnable) {
		switch (_KinematicsType) {
			case kInverseJacobian:
				break;
			case kMITRule:
				break;
			default:
				break;
		}
	}
}

