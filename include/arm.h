#ifndef ARM_H
#define ARM_H

#include "servos.h"

#define ARM_NJOINTS					4

// Servo standard has 1.5ms center, 2ms max, delta = 2 - 1.5 = 0.5ms
// MG996R servo has 120deg throw, 60 on each side of center
//
// (ticks_per_period * delta_pulse_width / period) / delta_angle
// (840000  * 0.5ms / 20ms) / 60deg
#define ARM_PWM_TICKS_PER_DEGREE	(21000.0 / 60.0)

// Unified naming across servos and the joints
typedef ServoIndex ArmIndex;

// Static offsets to convert between 0 of joint space and 0 of servo
static const double ArmJointOffset[ARM_NJOINTS] = {
	0.0, 	// Base
	0.0, 	// Shoulder
	0.0,	// Elbow
	0.0		// Wrist
};

// Rotation directions for each servo when viewed from the gear head
//		1	= counter-clockwise
//		-1	= clockwise
static const int8_t ArmJointDirection[ARM_NJOINTS] = {
	-1,		// Base
	-1,		// Shoulder
	-1,		// Elbow
	-1		// Wrist
};

// Current joint angles being commanded of the servos
double ArmJointAngles[ARM_NJOINTS];



void SetupArm(void);

void ArmSetJointAngle(ArmIndex joint, double angle);
void ArmSetJointAngles(ArmIndex* joints, double* angles, uint8_t length);
void ArmSetJointAnglesAll(double* angles);


#endif // ARM_H
