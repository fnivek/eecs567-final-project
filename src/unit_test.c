#include "board.h"
#include "debug_leds.h"

#define kNumTest 1

// All test functions return 0 if still running 1 on success and -1 on failure

// Test USB debug comms
int8_t TestUsb(void)
{
	if(!is_usb_ready()) return 0;

	UsbWriteString("\n\r\n\rStarting USB test\n\r");
	UsbWriteString("\n\rTODO: Write a USB read funciton");
	UsbWriteString("\n\rUSB test results: Unkown");
	return 1;
}

// main funciton
int main(void)
{
	SetupBoard();
	SetHeartbeatLed(kDebugBlueLed);
	ClearDebugLed(kDebugAllLeds);

	int8_t (*tests[kNumTest])(void) = {
		TestUsb
	};

	// Loop through all test
	uint8_t failed = 0;
	uint8_t current_test = 0;
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

	
	// Infinite loop
	while(true) {
		RunOnce();
		UsbWriteString("\n\rFinished");
	}
}