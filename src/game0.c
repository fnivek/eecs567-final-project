#include "game0.h"

// Global vars
float jold[3] = {0, 8, 0};

// Global const
const float kGame0Stepsize = .05;
const float kZmag = 0.5; // kZmag gets multiplied by kGame0Stepsize so scale appropriatly
		
void game0(void) {
	//Read: Joystick
		NunchuckData data;
		ReadWii(&data);
		float joy_data[3] = {data.joy_x, data.joy_y, 0};
		if(data.button_z == 1) {
			joy_data[2] = kZmag;
		} else if(data.button_c == 1) {
			joy_data[2] = -kZmag;
		}

	//Decompose: Joystick to LED Space (8x8)
		Point2 led_coord = {
			round((joy_data[0] + 1) * 4),
			round((joy_data[1] + 1) * 4)
		};
	//Write: to Screen
        LedmatClearBuffer();
		LedmatDrawPixel(led_coord, LEDMAT_LED_GREEN);
		LedmatRefreshDisplay();

	//Decompose: Joystick to base frame
		float jnew[3]; // Base frame Coords
		ProductScalarV3(kGame0Stepsize, joy_data, joy_data);
		SumV3V3(joy_data, jold, jnew);	// Integrate
		memcpy(jold, jnew, 3 * sizeof(float));
	// Use homagraphy to transform points in kSholder rotated by pi about z frame to base frame
		float th1 = jnew[2];
		float z_offset = Arm[kShoulder].DHd + Arm[kElbow].DHd + Arm[kWrist].DHd;
        Point3 target = {
        	-jnew[0] * cos(th1) - z_offset * sin(th1),
        	z_offset * cos(th1) - jnew[0] * sin(th1),
        	Arm[kBase].DHd + jnew[1]
        };
        KinematicsSetTargetPosition(target);
}