#include "board.h"
#include "debug_leds.h"

#define kNumTest 1

// All test functions return 0 if still running 1 on success and -1 on failure

uint8_t next = 0;
#define kMatchWordSize 4
char match_word[kMatchWordSize] = "next";
void ReadCallback(void* buf, uint16_t len) {
	// Search the buf for the word next
	char* cbuf = (char*)buf;
	uint16_t i = 0;
	uint16_t j = 0;
	for(i = 0; i < len; ++i) {
		for(j = 0; j < kMatchWordSize && j + i < len; ++j) {
			if(cbuf[i + j] != match_word[j]) {
				break;
			}
			if(j == kMatchWordSize - 1) {
				ToggleDebugLed(kDebugOrangeLed);
				next = 1;
				return;
			}
		}
	}
}

uint8_t CheckNext(void) {
	if(next) {
		next = 0;
		return 1;
	} else {
		return 0;
	}
}

// Test USB debug comms
int8_t TestUsb(void)
{
	if(!CheckNext()) return 0;

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
	UsbAddReadCallback(ReadCallback);

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

	UsbWriteString("\n\rFinished");

	Run();
}