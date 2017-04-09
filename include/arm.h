#ifndef ARM_H
#define ARM_H

#include <math.h>

#include "servos.h"
#include "helpers.h"

#define ARM_NJOINTS					4

// Servo standard has 1.5ms center, 2ms max, delta = 2 - 1.5 = 0.5ms
// MG996R servo has 120deg throw, 60 on each side of center
//
// (ticks_per_period * delta_pulse_width / period) / delta_angle
// (840000  * 0.5ms / 20ms) / 60deg
#define ARM_PWM_TICKS_PER_RAD	(21000.0 / (M_PI / 3.0))

// Unified naming across servos and the joints
typedef ServoIndex ArmIndex;

typedef struct {
	// Offset between joint 0 and servo 0
	float JointAngleOffset;
	// Rotation direction when viewed from gearhead
	//		 1 = ccw
	//		-1 = cw
	int8_t JointDirection;
	// Denavit-Hartenberg params
	float DHtheta;
	float DHd;
	float DHa;
	float DHalpha;
} ArmSegment;

static const ArmSegment Arm[ARM_NJOINTS] = {
	// offset,		dir,	theta,		d,			a,			alpha
	{0.0000,		-1,		0.0000,		1.0000,		0.0000,		-M_PI / 2},	// Base
	{0.0000,		1,		-M_PI / 2,	1.5000,		6.0000,		0.0000},	// Shoulder
	{0.0000,		-1,		0.0000,		-1.5000,	6.0000,		0.0000},	// Elbow
	{0.0000,		-1,		0.0000,		0.0000,		0.0000,		0.0000}		// Wrist
};

// Current joint angles being commanded of the servos
float ArmJointAngles[ARM_NJOINTS];



void SetupArm(void);

void ArmSetJointAngle(ArmIndex joint, float angle);
void ArmSetJointAngles(ArmIndex* joints, float* angles, uint8_t length);
void ArmSetJointAnglesAll(float* angles);

float ArmGetJointAngle(ArmIndex joint);
void ArmGetJointAngles(ArmIndex* joints, float* angles, uint8_t length);
void ArmGetJointAnglesAll(float* angles);


#endif // ARM_H
