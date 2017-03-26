#include "wii.h"

void SetupWii(void) {
	// Buffer for data to write
	uint8_t buf[1];
	// Number of tries
	const uint8_t kMaxNumTries = 15;
	uint8_t tries = 0;

	buf[0] = WII_INIT_BYTE1;
	while(WriteRegBlockingI2C(WII_ADDR, WII_INIT_REG1, 1, buf) != I2C_OK && tries++ < kMaxNumTries);
	if(tries == kMaxNumTries) {
		return;
	}

	buf[0] = WII_INIT_BYTE2;
	tries = 0;
	while(WriteRegBlockingI2C(WII_ADDR, WII_INIT_REG2, 1, buf) != I2C_OK && tries++ < kMaxNumTries);
	if(tries == kMaxNumTries) {
		return;
	}
}

NunchuckData ReadWii(void) {
	NunchuckData data;

	data.buf[0] = WII_CMD_DATA_READ;
	if(WriteBlockingI2C(WII_ADDR, 1, data.buf) != I2C_OK) {
		return data;
	}

	// Delay...
	volatile uint32_t i;
	for(i = 0; i < 5000; ++i);

	I2CStatus status = ReadBlockingI2C(WII_ADDR, WII_DATA_LENGTH, data.buf);
	if(status != I2C_OK) {
		return data;
	}

	return data;
}