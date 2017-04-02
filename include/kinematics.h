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
static const float kKinematicsJInvGain = 1;
static const float kKinematicsUpdatePeriod = 0.02; // Seconds
static const uint32_t kKinematicsUpdatePeriodMillis = 20; // 50 Hz

/*----- Private vars -----*/
Point3 _KinematicsTarget;
Point3 _KinematicsLastCmd;
uint8_t _KinematicsEnable;
KinematicsControlType _KinematicsType;
uint32_t _KinematicsLastTime;

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
