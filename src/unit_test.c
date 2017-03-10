#include "board.h"
#include "debug_leds.h"

// Test USB debug comms
int8_t TestUsb(void)
{
	while(!is_usb_ready());

	UsbWriteString("\n\r\n\rStarting USB test\n\r");
	UsbWriteString("\n\rTODO: Write a USB read funciton");
	UsbWriteString("\n\rUSB test results: Unkown");
	return 0;
}

// main funciton
int main(void)
{
	SetupBoard();
	SetHeartbeatLed(kDebugBlueLed);
	ClearDebugLed(kDebugAllLeds);

	// Begin tests
	int8_t result = 
		TestUsb(); // | 
		// NextTest() | ...

	if(!result)
	{
		UsbWriteString("\n\r\n\rAll test complete succesfully!");
		gpio_set(GPIOD, GPIO12);
	}
	else
	{
		 UsbWriteString("\n\r\n\rSome test failed!");
		 gpio_set(GPIOD, GPIO14);
	}

	UsbWriteString("\n\rFinished");

	
	// Infinite loop
	while(true)
	{
	}
}