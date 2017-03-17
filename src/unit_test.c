#include "board.h"
#include "debug_leds.h"
#include <string.h>

#define kNumTest 2

// All test functions return 0 if still running 1 on success and -1 on failure

// Timing vars
uint32_t last_time;
const uint32_t kWaitTime = 1000; // Milliseconds

// Read callback
char usb_buf[64] = "";
void ReadCallback(void* buf, uint16_t len) {
	// Copy the usb read
	strncpy(usb_buf, buf, len);

	// Null terminate the string
	if(len >= 64) {
		usb_buf[63] = '\0';
	} else {
		usb_buf[len] = '\0';
	}
}

// Check if the usb buf contains the c string str
//	if it does usb_buf is cleared
//	Returns 0 if the string is not in usb_buf
//	Returns 1 if the string matches usb_buf
uint8_t CheckUsbBuf(const char* str) {
	if(strcmp(usb_buf, str)) {
		return 0;
	}

	// Clear usb_buf
	usb_buf[0] = '\0';

	return 1;
}

// Test USB debug comms
int8_t TestUsb(void) {
	// Send start message every 1 second
	if(Now() - last_time > kWaitTime) {
		last_time = Now();
		UsbWriteString("\n\rPlease send start:\n\r");
		ToggleDebugLed(kDebugOrangeLed);
	}

	// Wait until start is sent
	if(!CheckUsbBuf("start")) return 0;
	ClearDebugLed(kDebugOrangeLed);


	UsbWriteString("\n\r\n\rStarting USB test\n\r");
	UsbWriteString("\n\rUSB test results: Success");
	return 1;
}

// Test Servos
uint8_t state = 0;
uint8_t servo_test_fail = 0;
int8_t TestServos(void) {
	// State 0 setup and move to zero
	if(state == 0) {
		UsbWriteString("\n\r\n\rStarting Servos test\n\r");
		UsbWriteString("\n\rMoving Servos to zero position");
		UsbWriteString("\n\rDid the servos move to 0 (y/n):");
		ServosZero();
		last_time = Now();
		++state;
	// State 1 wait for user to say continue
	} else if(state == 1) {
		if(CheckUsbBuf("y")) {
			++state;
			UsbWriteString("\n\rMove to minimum position (y/n):");
		} else if(CheckUsbBuf("n")) {
			++state;
			UsbWriteString("\n\rMove to minimum position (y/n):");
			servo_test_fail = 1;
		}
	// State 2 wait for response to move to min
	} else if(state == 2) {
		if(CheckUsbBuf("y")) {
			state++;
			UsbWriteString("\n\rMoving to minimum servo positions");
			ServoIndex i;
			for(i = 0; i < kNumServos; ++i) {
				ServosSetPWM(i, kMinPW);
			}
			UsbWriteString("\n\rDid the servos move to minimum position (y/n):");
		} else if(CheckUsbBuf("n")) {
			UsbWriteString("\n\rSkiping moving to minimum positions");
			UsbWriteString("\n\rMove to maximum position (y/n):");
			state = 4;
		}
	// State 3 check if motors moved to minimum position
	} else if(state == 3) {
		if(CheckUsbBuf("y")) {
			++state;
			UsbWriteString("\n\rMove to maximum position (y/n):");
		} else if(CheckUsbBuf("n")) {
			++state;
			UsbWriteString("\n\rMove to maximum position (y/n):");
			servo_test_fail = 1;
		}
	// State 4 check if user wants to move to to max position
	} else if(state == 4) {
		if(CheckUsbBuf("y")) {
			state++;
			UsbWriteString("\n\rMoving to maximum servo positions");
			ServoIndex i;
			for(i = 0; i < kNumServos; ++i) {
				ServosSetPWM(i, kMaxPW);
			}
			UsbWriteString("\n\rDid the servos move to maximum position (y/n):");
		} else if(CheckUsbBuf("n")) {
			UsbWriteString("\n\rSkiping moving to maximum positions");
			state = 6;
		}
	// State 5 check if servos moved to max position
	} else if(state == 5) {
		if(CheckUsbBuf("y")) {
			++state;
		} else if(CheckUsbBuf("n")) {
			++state;
			servo_test_fail = 1;
		}
	// State 6 end the test
	} else if(state == 6) {
		UsbWriteString("\n\rRezeroing servos");
		ServosZero();
		if(servo_test_fail) {
			UsbWriteString("\n\rServo test result: Failure");
			return -1;
		} else {
			UsbWriteString("\n\rServo test result: Success");
			return 1;
		}
		
	}

	return 0;
}

// main funciton
int main(void) {
	SetupBoard();
	SetHeartbeatLed(kDebugBlueLed);
	ClearDebugLed(kDebugAllLeds);
	UsbAddReadCallback(ReadCallback);

	int8_t (*tests[kNumTest])(void) = {
		TestUsb,
		TestServos
	};

	// Loop through all test
	uint8_t failed = 0;
	uint8_t current_test = 0;
	last_time = Now();
	while(current_test < kNumTest) {
		// Update the board
		RunOnce();

		// Run the current test
		int8_t result = tests[current_test]();

		// Check if result is still running 0
		if(result != 0) {
			// Check for failure
			if(result < 0) {
				failed = 1;
			}
			current_test++;
		}
	}

	if(!failed) {
		UsbWriteString("\n\r\n\rAll test complete succesfully!");
		SetDebugLed(kDebugGreenLed);
	} else {
		UsbWriteString("\n\r\n\rSome test failed!");
		SetDebugLed(kDebugRedLed);
	}

	UsbWriteString("\n\rFinished");

	Run();
}