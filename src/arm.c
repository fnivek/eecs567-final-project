#include "arm.h"

void SetupArm(void) {
	// Servos will default to their 0
	// So set out joint angles to the offset
	for (int q = 0; q < ARM_NJOINTS; q++) {
		ArmJointAngles[q] = Arm[q].JointDirection * Arm[q].JointAngleOffset;
	}
}

void ArmSetJointAngle(ArmIndex joint, double angle) {
	double servoAngle;
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

void ArmSetJointAngles(ArmIndex* joints, double* angles, uint8_t length) {
	for (uint8_t i = 0; i < length; i++)  {
		ArmSetJointAngle(joints[i], angles[i]);
	}
}

void ArmSetJointAnglesAll(double* angles) {
	for (uint8_t i = 0; i < ARM_NJOINTS; i++) {
		ArmSetJointAngle((ArmIndex)i, angles[i]);
	}
}

