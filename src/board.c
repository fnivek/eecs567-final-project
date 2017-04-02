#include "board.h"

void SetupBoard(void) {
	// Setup clock
	rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

	// This is risky?
	SetupSystick();

	// Setup components
	SetupDebugLeds();
	SetupUsb();
	SetupMatlabComms();
	SetupServos();
	SetupI2C();
	SetupLedmat();	// After i2c
	SetupWii();		// After i2c
	SetupArm();

}

void Run(void) {
	// Infinite loop
	while(true) {
		RunOnce();
	}
}

void RunOnce(void) {
	// Do all the system tick callbacks
	DoSystickCallbacks();
}

