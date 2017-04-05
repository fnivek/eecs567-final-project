#include "wii.h"
#include "ledmat.h"
#include "i2c.h"
#include "servos.h"
#include "arm.h"
//Joystick
uint8_t xjoy_max = 230;
uint8_t xjoy_center = 130;
uint8_t xjoy_min = 30;
uint8_t yjoy_max = 230;
uint8_t yjoy_center = 130;
uint8_t yjoy_min = 30;
float xdivs, ydivs;

//Screen
//http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
//35 to 228 right 27 to 220 y;
uint8_t ledmat_x;
uint8_t ledmat_y;

//Motor
uint_8_t motorx;
uint_8_t motory;
uint_8_t motorcoords[2];
unit_8_t motorcoordsold[2];
unit_8_t zmotormag;
float stepsize = .05;

float l1 = 6;
float l2 = 6;
float th1;
float th2;

		
int Game0(void){
	//Read: Joystick
		NunchuckData data = ReadWii();
		uint8_t xjdata = data.fields.joy_x;
		uint8_t yjdata = data.fields.joy_y;
		uint8_t zposdata = data.fields.button_z;
		uint8_t znegdata = data.fields.button_c;
	//Decompose: Joystick to LED Space (8x8)
		xdivs = (xjoy_max-xjoy_min)/8;
		ydivs = (yjoy_max-yjow_min)/8;
		ledmat_x = round(xjdata/xdivs);
		ledmat_y = round(yjdata/ydivs);
	//Decompose: Joystick to Motor Space
		if(zposdata == 1){
			zjdata = zmotormag;
		}else if(znegdata == 1){
			zjdata = -1*zmotormag;
		}else{
			zjdata = 0;
		}
		motorcoords = motorcoords_old + stepsize*[xjdata yjdata zjdata]; //Psuedo xdot ydot zdot
		motorcoords_old = motorcoords;
	//Write: to Screen
		LedmatDrawPixel(CastPoint2(ledmat_x, ledmat_y), LEDMAT_LED_GREEN);
		LedmatRefreshDisplay();
	//Read: Motor
	//need current motor angles
		jspace = motorread()
		th1 = jspace(1);
		th2 = jspace(2);
		float th12 = th1 + th2;
	//Write: to Motor 
	//Inverse 2x2 
	// J = [l1cth1+l2cth12, l2cth12; -l1sth1-l2sth12 -lsth12]
		Jinv = [sin(th12)/(l1*sin(th2)) cos(th12)/(l1*sin(th2));
				(-(l2*sin(th12))+l1*sin(th1))/(l1*l2*sin(th2)) (-l1*cos(th12) + l1*cos(th1))/(l1*l2*sin(th2))];
		motormove = ([Jinv*motorcoords(0:1),motorcoords(2)]);
		ArmSetJointAngle(0,motormove(0));
		ArmSetJointAngle(1,motormove(1));
		ArmSetJointAngle(2,motormove(2));
}