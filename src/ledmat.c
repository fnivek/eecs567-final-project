#include <string.h>

#include "ledmat.h"

inline uint8_t _isValidColor(uint8_t color) {
	return (color == LEDMAT_LED_OFF || color == LEDMAT_LED_RED
		|| color == LEDMAT_LED_GREEN || color == LEDMAT_LED_YELLOW);
}

void LedmatInit(void) {
	// Enable LED matrix system clock
	// i2cSendByte(LEDMAT_I2C_ADDR, LEDMAT_SYSCLOCK_REG | LEDMAT_SYSCLOCK_ON);

	// Set initial display brightness
	// i2cSendByte(LEDMAT_I2C_ADDR, LEDMAT_BRIGHTNESS_REG | LED_BRIGHTNESS_MIN);

	// Initialize the display buffer to full off
	LedmatClearBuffer();
	// Write out to display memory
	LedmatRefreshDisplay();

	// Set blink rate and enable screen
	// i2cSendByte(LEDMAT_I2C_ADDR, LEDMAT_DISPLAY_REG | LEDMAT_DISPLAY_ON | LEDMAT_DISPLAY_BLINK_OFF);
}

void LedmatRefreshDisplay(void) {
	// Begin writing data to the start of display buffer memory
	// i2cSendByte(LEDMAT_I2C_ADDR, LEDMAT_BUFFER_REG | 0x00);
}

void LedmatClearBuffer(void) {
	memset(_LedmatDisplayBuff.mmapped, 0, sizeof(_LedmatDisplayBuff));
}

void LedmatDrawPixel(Point2 pt, uint8_t color) {
	// We're being given a pixel in specific, disallow anything outside the range of our display
	uint8_t x = (uint8_t)pt.x;
	uint8_t y = (uint8_t)pt.y;

	// LED matrix coordinates defined with (0,0) in the top left
	// Increasing x to the right, increasing y down
	if (pt.x >= LEDMAT_NCOLS || pt.y >= LEDMAT_NROWS) {
		// Invalid coordinate
		return;
	}

	switch (color) {
	case LEDMAT_LED_OFF:
		_LED_SET_OFF(x, y);
		break;
	case LEDMAT_LED_RED:
		_LED_SET_RED(x, y);
		break;
	case LEDMAT_LED_GREEN:
		_LED_SET_GREEN(x, y);
		break;
	case LEDMAT_LED_YELLOW:
		_LED_SET_YELLOW(x, y);
		break;
	default:
		// Invalid color
		return;
	}	
}

void LedmatDrawLine(Point2 ptStart, Point2 ptEnd, uint8_t color) {
}

void LedmatDrawPolygon(Point2* ptList, int nCoords, uint8_t color) {
}

void LedmatDrawNumber(uint8_t number, uint8_t color) {
}

void LedmatDrawFloodfill(uint8_t color) {
	uint8_t redVal = 0xFF;
	uint8_t greenVal = 0xFF;

	switch (color) {
	case LEDMAT_LED_OFF:
		redVal = 0;
		greenVal = 0;
		break;
	case LEDMAT_LED_RED:
		greenVal = 0;
		break;
	case LEDMAT_LED_GREEN:
		redVal = 0;
		break;
	case LEDMAT_LED_YELLOW:
		break;
	default:
		// Invalid color
		return;
	}

	for (int q = 0; q < LEDMAT_NROWS; q++) {
		_LedmatDisplayBuff.row[q].red = redVal;
		_LedmatDisplayBuff.row[q].green = greenVal;
	}
}

void LedmatCommandPixel(Point2 pt, uint8_t color) {
	//uint8_t x = (uint8_t)pt.x;
	//uint8_t y = (uint8_t)pt.y;

	LedmatDrawPixel(pt, color);

	// Calculate the memory address of the pixel changed

	// Write to that single memory address
}

