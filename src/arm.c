#include "arm.h"

void SetupArm(void) {
	// Servos will default to their 0
	// So set out joint angles to the offset
	for (int q = 0; q < ARM_NJOINTS; q++) {
		ArmJointAngles[q] = ArmJointDirection[q] * ArmJointOffset[q];
	}
}

void ArmSetJointAngle(ArmIndex joint, double angle) {
	double servoAngle;
	uint32_t pwmCommand;

	// Angle of the servo to get the desired joint angle
	servoAngle = ArmJointDirection[joint] * angle + ArmJointOffset[joint];

	// Convert angle command to a PWM count
	pwmCommand = servoAngle * ARM_PWM_TICKS_PER_DEGREE + kZeroPW;

	// Keep track of our new angle
	ArmJointAngles[joint] = angle;

	// Command the servo
	ServosSetPWM(joint, pwmCommand);
}
