#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <math.h>

#include "helpers.h"
#include "arm.h"

typedef enum KinematicsControlType {
	kInverseJacobian,
	kMITRule
} KinematicsControlType;

Point3 _KinematicsTarget;
uint8_t _KinematicsEnable;
KinematicsControlType _KinematicsType;

void SetupKinematics(void);

/*----- Jacobians -----*/
void KinematicsComputeJacobian(double* angles, double** jacobian);
int KinematicsInvertJacobian(double** jacobian, double** inverse);
void KinematicsTranspose(double** mat);

/*----- Forward Kinematics -----*/
Point3 KinematicsGetPosition(double* angles, uint8_t nAngles);
Point3 KinematicsGetArmPosition(void);


/*----- Inverse Kinematics -----*/
void KinematicsSetControlEnable(uint8_t enable);
void KinematicsSetControlType(KinematicsControlType control);
void KinematicsSetTargetPosition(Point3 xd);
void KinematicsAdvanceControl(void);


#endif // KINEMATICS_H
