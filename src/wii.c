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

// Return 0 on successful read, 1 on fail
uint8_t ReadWii(NunchuckData* retData) {
	_NunchuckData rawData;

	rawData.buf[0] = WII_CMD_DATA_READ;
	if(WriteBlockingI2C(WII_ADDR, 1, rawData.buf) != I2C_OK) {
		return 1;
	}

	// Delay...
	volatile uint32_t i;
	for(i = 0; i < 5000; ++i);

	I2CStatus status = ReadBlockingI2C(WII_ADDR, WII_DATA_LENGTH, rawData.buf);
	if(status != I2C_OK) {
		return 1;
	}

	// Successfully retrieved data, time to parse
	// Convert joysticks to a [-1.0, 1.0] scale
	if (rawData.fields.joy_x > WII_JOY_X_ZERO) {
		retData->joy_x = (float)(rawData.fields.joy_x - WII_JOY_X_ZERO) / (float)(WII_JOY_X_MAX - WII_JOY_X_ZERO);
	}
	else if (rawData.fields.joy_x < WII_JOY_X_ZERO) {
		retData->joy_x = (float)(rawData.fields.joy_x - WII_JOY_X_ZERO) / (float)(WII_JOY_X_ZERO - WII_JOY_X_MIN);
	}
	else {
		retData->joy_x = 0.0;
	}
	if (rawData.fields.joy_y > WII_JOY_Y_ZERO) {
		retData->joy_y = (float)(rawData.fields.joy_y - WII_JOY_Y_ZERO) / (float)(WII_JOY_Y_MAX - WII_JOY_Y_ZERO);
	}
	else if (rawData.fields.joy_y < WII_JOY_Y_ZERO) {
		retData->joy_y = (float)(rawData.fields.joy_y - WII_JOY_Y_ZERO) / (float)(WII_JOY_Y_ZERO - WII_JOY_Y_MIN);
	}
	else {
		retData->joy_y = 0.0;
	}
	// Invert buttons to make them report active high (physically active low)
	retData->button_z = rawData.fields.button_z ? 0 : 1;
	retData->button_c = rawData.fields.button_c ? 0 : 1;
	retData->accel_x = (rawData.fields.accel_x_high << 2) | (rawData.fields.accel_x_low);
	retData->accel_y = (rawData.fields.accel_y_high << 2) | (rawData.fields.accel_y_low);
	retData->accel_z = (rawData.fields.accel_z_high << 2) | (rawData.fields.accel_z_low);

	return 0;
}
