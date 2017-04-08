#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <math.h>

#include "helpers.h"
#include "arm.h"
#include "matrix_math.h"
#include "systick_handler.h"

typedef enum KinematicsControlType {
	kInverseJacobian,
	kMITRule
} KinematicsControlType;

/*----- Constants -----*/
static const float kKinematicsJInvGain = 10;
static const float kKinematicsUpdatePeriod = 0.02; // Seconds
static const uint32_t kKinematicsUpdatePeriodMillis = 20; // 50 Hz
static const float kKinematicsMaxJointChange = 0.1; // rads/period
static const float kKinematicsMinJointChange = -0.1; // rads/period

/*----- Private vars -----*/
Point3 _KinematicsTarget;
float _KinematicsLastCmd[3];
uint8_t _KinematicsEnable;
KinematicsControlType _KinematicsType;

void SetupKinematics(void);

/*----- Jacobians -----*/
void KinematicsComputeJacobian(float* angles, float(*jacobian)[3]);

/*----- Forward Kinematics -----*/
Point3 KinematicsGetPosition(float* angles);
Point3 KinematicsGetArmPosition(void);


/*----- Inverse Kinematics -----*/
void KinematicsSetControlEnable(uint8_t enable);
void KinematicsSetControlType(KinematicsControlType control);
void KinematicsSetTargetPosition(Point3 xd);
void KinematicsAdvanceControl(void);

/*----- Helper functions -----*/
void KinematicsGetArmAngles(float* angles);


#endif // KINEMATICS_H
