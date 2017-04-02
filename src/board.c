#include "board.h"

void SetupBoard(void) {
	// Setup clock
	rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
	
	// Setup components
	//	Note systick should be setup first for timeouts
	//		While some intterupts may occur durring initilization
	//		They will not be executed until the RunOnce calls
	//		DoSystickCallbacks so there is no need to worry that
	//		something will be uninitilized before its callback is called	
	SetupSystick();		// Should be first

	SetupDebugLeds();
	SetupUsb();
	SetupServos();
	SetupI2C();
	SetupLedmat();	// After i2c
	SetupWii();		// After i2c
	SetupArm();
	SetupKinematics();	// After setup arm
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

