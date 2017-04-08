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
uint_8_t mx;
uint_8_t my;
uint_8_t jnew[3]; //Motor Coords
unit_8_t jold[3];
unit_8_t jsend[3];
unit_8_t zmag;
float stepsize = .05;

float l1 = 6;
float l2 = 6;
float th1;
float th2;

		
int game0(void){
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
			zjdata = zmag;
		}else if(znegdata == 1){
			zjdata = -1*zmag;
		}else{
			zjdata = 0;
		}
		jnew = jold + stepsize*[xjdata yjdata zjdata]; //Psuedo xdot ydot zdot
		jold = jnew;
	//Write: to Screen
		LedmatDrawPixel(CastPoint2(ledmat_x, ledmat_y), LEDMAT_LED_GREEN);
		LedmatRefreshDisplay();
	//Read: Motor
		ArmGetJointAngles([kBase,kShoulder,kElbow],jold,3))
	//Write: to Motor 
		Jinv = [sin(th12)/(l1*sin(th2)) cos(th12)/(l1*sin(th2));
				(-(l2*sin(th12))+l1*sin(th1))/(l1*l2*sin(th2)) (-l1*cos(th12) + l1*cos(th1))/(l1*l2*sin(th2))];
		jnew(0) = zjdata
		jsend = ([jnew(0),Jinv*jnew(1:2)]);
		ArmSetJointAngles([kBase,kShoulder,kElbow],jsend,3)
}