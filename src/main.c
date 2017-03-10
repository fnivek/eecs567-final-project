/*
 * This is the main file that runs first
 */
#include "board.h"


// main funciton
int main(void)
{
	SetupBoard();

	// Infinite loop
	while(true)
	{
		// Temp solution to polling to slow
		UsbPoll();
	}
}