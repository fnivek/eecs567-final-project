#include "arm.h"

void SetupArm(void) {
	// Servos will default to their 0
	float zeros[4] = {0,0,0,0};
	ArmSetJointAnglesAll(zeros);
}

void ArmSetJointAngle(ArmIndex joint, float angle) {
	float servoAngle;
	uint32_t pwmCommand;

	// Angle of the servo to get the desired joint angle
	servoAngle = Arm[joint].JointDirection * angle + Arm[joint].JointAngleOffset;

	// Convert angle command to a PWM count
	pwmCommand = servoAngle * ARM_PWM_TICKS_PER_DEGREE + kZeroPW;

	// Keep track of our new angle
	ArmJointAngles[joint] = angle;

	// Command the servo
	ServosSetPWM(joint, pwmCommand);
}

void ArmSetJointAngles(ArmIndex* joints, float* angles, uint8_t length) {
	for (uint8_t i = 0; i < length; i++)  {
		ArmSetJointAngle(joints[i], angles[i]);
	}
}

void ArmSetJointAnglesAll(float* angles) {
	for (uint8_t i = 0; i < ARM_NJOINTS; i++) {
		ArmSetJointAngle((ArmIndex)i, angles[i]);
	}
}

float ArmGetJointAngle(ArmIndex joint) {
	return ArmJointAngles[joint];
}

void ArmGetJointAngles(ArmIndex* joints, float* angles, uint8_t length) {
	uint8_t i;
	for(i = 0; i < length; ++i) {
		angles[i] = ArmJointAngles[joints[i]];
	}
}

void ArmGetJointAnglesAll(float* angles) {
	uint8_t i;
	for (i = 0; i < ARM_NJOINTS; i++) {
		angles[i] = ArmJointAngles[i];
	}
}


