#include "kinematics.h"


void SetupKinematics(void) {
	float zeros[3] = {0,0,0};
	_KinematicsLastCmd  = KinematicsGetPosition(zeros);
	_KinematicsTarget 	= _KinematicsLastCmd;
	_KinematicsEnable 	= 0;
	_KinematicsType		= kInverseJacobian;
	_KinematicsLastTime = Now();

	// Add systick callback
	AddSystickCallback(KinematicsAdvanceControl, kKinematicsUpdatePeriodMillis);
}

/*----- Jacobians -----*/
void KinematicsComputeJacobian(float* angles, float(*jacobian)[3]) {
	// Assume the Jacobian matrix is 3x3
	float(*J)[3] = jacobian;

	// From matlab the jacobian is:
	// [ a3*sin(th2 + th2_offset)*sin(th1)*sin(th3) - d3*cos(th1) - a2*cos(th2 + th2_offset)*sin(th1) - a3*cos(th2 + th2_offset)*cos(th3)*sin(th1) - Arm[kShoulder].DHd*cos(th1), -cos(th1)*(a2*sin(th2 + th2_offset) + a3*sin(th2 + th3 + th2_offset)), -a3*sin(th2 + th3 + th2_offset)*cos(th1)]
	// [ a2*cos(th2 + th2_offset)*cos(th1) - d3*sin(th1) - d2*sin(th1) + a3*cos(th2 + th2_offset)*cos(th1)*cos(th3) - a3*sin(th2 + th2_offset)*cos(th1)*sin(th3), -sin(th1)*(a2*sin(th2 + th2_offset) + a3*sin(th2 + th3 + th2_offset)), -a3*sin(th2 + th3 + th2_offset)*sin(th1)]
	// [                                                                                                                                                       0,           - a2*cos(th2 + th2_offset) - a3*cos(th2 + th3 + th2_offset),          -a3*cos(th2 + th3 + th2_offset)]
 
	J[0][0] = Arm[kElbow].DHa *sin(angles[kShoulder] + Arm[kShoulder].DHtheta)*sin(angles[kBase])*sin(angles[kElbow]) - Arm[kElbow].DHd*cos(angles[kBase]) - Arm[kShoulder].DHa*cos(angles[kShoulder] + Arm[kShoulder].DHtheta)*sin(angles[kBase]) - Arm[kElbow].DHa *cos(angles[kShoulder] + Arm[kShoulder].DHtheta)*cos(angles[kElbow])*sin(angles[kBase]) - Arm[kShoulder].DHd*cos(angles[kBase]);
	J[0][1] = -cos(angles[kBase])*(Arm[kShoulder].DHa*sin(angles[kShoulder] + Arm[kShoulder].DHtheta) + Arm[kElbow].DHa *sin(angles[kShoulder] + angles[kElbow] + Arm[kShoulder].DHtheta));
	J[0][2] = -Arm[kElbow].DHa *sin(angles[kShoulder] + angles[kElbow] + Arm[kShoulder].DHtheta)*cos(angles[kBase]);
	J[1][0] = Arm[kShoulder].DHa*cos(angles[kShoulder] + Arm[kShoulder].DHtheta)*cos(angles[kBase]) - Arm[kElbow].DHd*sin(angles[kBase]) - Arm[kShoulder].DHd*sin(angles[kBase]) + Arm[kElbow].DHa *cos(angles[kShoulder] + Arm[kShoulder].DHtheta)*cos(angles[kBase])*cos(angles[kElbow]) - Arm[kElbow].DHa *sin(angles[kShoulder] + Arm[kShoulder].DHtheta)*cos(angles[kBase])*sin(angles[kElbow]);
	J[1][1] = -sin(angles[kBase])*(Arm[kShoulder].DHa*sin(angles[kShoulder] + Arm[kShoulder].DHtheta) + Arm[kElbow].DHa *sin(angles[kShoulder] + angles[kElbow] + Arm[kShoulder].DHtheta));
	J[1][2] = -Arm[kElbow].DHa *sin(angles[kShoulder] + angles[kElbow] + Arm[kShoulder].DHtheta)*sin(angles[kBase]);
	J[2][0] = 0;
	J[2][1] = Arm[kShoulder].DHa*cos(angles[kShoulder] + Arm[kShoulder].DHtheta) - Arm[kElbow].DHa *cos(angles[kShoulder] + angles[kElbow] + Arm[kShoulder].DHtheta);
	J[2][2] = -Arm[kElbow].DHa *cos(angles[kShoulder] + angles[kElbow] + Arm[kShoulder].DHtheta);
}

/*----- Forward Kinematics -----*/
Point3 KinematicsGetPosition(float* angles) {
	// From matlab we have:
	// a2*cos(th2 + th2_offset)*cos(th1) - d3*sin(th1) - d2*sin(th1) + a3*cos(th2 + th2_offset)*cos(th1)*cos(th3) - a3*sin(th2 + th2_offset)*cos(th1)*sin(th3)
	// d2*cos(th1) + d3*cos(th1) + a2*cos(th2 + th2_offset)*sin(th1) + a3*cos(th2 + th2_offset)*cos(th3)*sin(th1) - a3*sin(th2 + th2_offset)*sin(th1)*sin(th3)
	// d1 - a2*sin(th2 + th2_offset) - a3*sin(th2 + th3 + th2_offset)
	
	Point3 p;
	p.x = Arm[kShoulder].DHa*cos(angles[kShoulder] + Arm[kShoulder].DHtheta)*cos(angles[kBase]) - Arm[kElbow].DHd*sin(angles[kBase]) - Arm[kShoulder].DHd*sin(angles[kBase]) + Arm[kElbow].DHa*cos(angles[kShoulder] + Arm[kShoulder].DHtheta)*cos(angles[kBase])*cos(angles[kElbow]) - Arm[kElbow].DHa*sin(angles[kShoulder] + Arm[kShoulder].DHtheta)*cos(angles[kBase])*sin(angles[kElbow]);
	p.y = Arm[kShoulder].DHd*cos(angles[kBase]) + Arm[kElbow].DHd*cos(angles[kBase]) + Arm[kShoulder].DHa*cos(angles[kShoulder] + Arm[kShoulder].DHtheta)*sin(angles[kBase]) + Arm[kElbow].DHa*cos(angles[kShoulder] + Arm[kShoulder].DHtheta)*cos(angles[kElbow])*sin(angles[kBase]) - Arm[kElbow].DHa*sin(angles[kShoulder] + Arm[kShoulder].DHtheta)*sin(angles[kBase])*sin(angles[kElbow]);
	p.z = Arm[kBase].DHd - Arm[kShoulder].DHa*sin(angles[kShoulder] + Arm[kShoulder].DHtheta) - Arm[kElbow].DHa*sin(angles[kShoulder] + angles[kElbow] + Arm[kShoulder].DHtheta);
	return p;
}

Point3 KinematicsGetArmPosition(void) {
	float angles[3];
	KinematicsGetArmAngles(angles);
	return KinematicsGetPosition(angles);
}



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
		// Calculate the jacobian
		float J[3][3];
		float angles[3];
		KinematicsGetArmAngles(angles);
		KinematicsComputeJacobian(angles, J);

		// Calculate error
		float error[3];

		// Perform specific control loop
		float dq[3] = {0, 0, 0};
		switch (_KinematicsType) {
			case kInverseJacobian:
				{
					// Calculate derivative of command
					float last_cmd[3];
					float target[3];
					float d_cmd[3];
					Point3ToFloatArray(_KinematicsLastCmd, last_cmd);
					Point3ToFloatArray(_KinematicsTarget, target);
					ProductScalarV3(-1.0f, last_cmd, last_cmd);
					SumV3V3(target, last_cmd, d_cmd);
					ProductScalarV3(1 / kKinematicsUpdatePeriod, d_cmd, d_cmd);

					// J^-1 * (k * err + d_cmd)
					float J_inv[3][3];
					if(!InvertM3x3(J, J_inv)) {
						// TODO: Deal with sigularity
					}
					float temp[3];
					ProductScalarV3(kKinematicsJInvGain, error, temp);
					SumV3V3(d_cmd, temp, temp);
					ProductM3x3V3(J_inv, temp, dq);
				}
				break;
			case kMITRule:
				// TODO
				break;
			default:
				break;
		}

		// Perform numerical integration
		// q = angles + dq * dt
		float q[3];
		ProductScalarV3(kKinematicsUpdatePeriod, dq, dq);
		SumV3V3(angles, dq, q);

		// Calculate the upright position of the led matrix
		float q_wrist = -q[kElbow] - q[kShoulder];

		// Update new joint angles
		float cat_q[4] = {q[kBase], q[kShoulder], q[kElbow], q_wrist};
		ArmSetJointAnglesAll(cat_q);

		// Update time
		_KinematicsLastTime = Now();
	}
}


/*----- Helper functions -----*/
void KinematicsGetArmAngles(float* angles) {
	ArmIndex joints[3] = {kBase, kShoulder, kElbow};
	ArmGetJointAngles(joints, angles, 3);
}