#include <string.h>

#include "ledmat.h"

inline uint8_t _isValidColor(uint8_t color) {
	return (color == LEDMAT_LED_OFF || color == LEDMAT_LED_RED
		|| color == LEDMAT_LED_GREEN || color == LEDMAT_LED_YELLOW);
}

inline uint8_t _octantOf(Point2 pt) {
	if (pt.y > 0) {
		if (pt.x > 0) {
			if (pt.x >= pt.y) {
				return 0;
			}
			else {
				return 1;
			}
		}
		else {
			if (-pt.x >= pt.y) {
				return 3;
			}
			else {
				return 2;
			}
		}
	}
	else {
		if (pt.x > 0) {
			if (pt.x >= -pt.y) {
				return 7;
			}
			else {
				return 6;
			}
		}
		else {
			if (-pt.x >= -pt.y) {
				return 4;
			}
			else {
				return 5;
			}
		}
	}
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
	Point2 origin;
	uint8_t targetOctant;
	int deltaX;
	int deltaY;
	int D;
	int currY;
	int swap;
	int drawX;
	int drawY;

	origin = CastPoint2((int)ptStart.x, (int)ptStart.y);

	// Calculate difference between points and truncate
	deltaX = (int)(ptEnd.x - ptStart.x);
	deltaY = (int)(ptEnd.y - ptStart.y);

	// Treat the start as the origin and determine the octant of the final line
	targetOctant = _octantOf(CastPoint2((double)deltaX, (double)deltaY));

	// Shift into the first octant (+x, +y, x >= y)
	switch (targetOctant) {
	case 0:
	default:
		break;
	case 1:
		swap = deltaX;
		deltaX = deltaY;
		deltaY = swap;
		break;
	case 2:
		swap = deltaX;
		deltaX = deltaY;
		deltaY = -swap;
		break;
	case 3:
		deltaX = -deltaX;
		break;
	case 4:
		deltaX = -deltaX;
		deltaY = -deltaY;
		break;
	case 5:
		swap = deltaX;
		deltaX = -deltaY;
		deltaY = -swap;
		break;
	case 6:
		swap = deltaX;
		deltaX = -deltaY;
		deltaY = swap;
		break;
	case 7:
		deltaY = -deltaY;
		break;
	}

	// Error tracking term for Bresenham's Algorithm
	D = 2 * deltaY - deltaX;

	// Start at the origin
	currY = 0;
	
	for (int currX = 0; currX < deltaX; currX++) {
		// Convert plot space coordinate to the destination octant
		// NOTE: yes this looks similar to the earlier code, but octants 2 and 6
		//       have different conversions (swapped)
		switch (targetOctant) {
		case 0:
		default:
			drawX = currX;
			drawY = currY;
			break;
		case 1:
			drawX = currY;
			drawY = currX;
			break;
		case 2:
			drawX = -currY;
			drawY = currX;
			break;
		case 3:
			drawX = -currX; 
			drawY = currY;
			break;
		case 4:
			drawX = -currX;
			drawY = -currY;
			break;
		case 5:
			drawX = -currY;
			drawY = -currX;
			break;
		case 6:
			drawX = currY;
			drawY = -currX;
			break;
		case 7:
			drawX = currX;
			drawY = -currY;
			break;
		}

		// Add to the original starting point and draw
		LedmatDrawPixel(CastPoint2(origin.x + (double)drawX, origin.y + (double)drawY), color);

		// Bresenham's Algorithm, integer math version
		if (D > 0) {
			currY++;
			D -= 2 * deltaX;
		}
		D += 2 * deltaY;
	}
}

void LedmatDrawPolygon(Point2* ptList, int nCoords, uint8_t color) {
	if (nCoords < 0) {
		return;
	}
	else if (nCoords == 1) {
		LedmatDrawPixel(*ptList, color);
		return;
	}
	else if (nCoords == 2) {
		LedmatDrawLine(ptList[0], ptList[1], color);
		return;
	}

	// Draw forward through the list
	for (int i = 1; i < nCoords; i++) {
		LedmatDrawLine(ptList[i - 1], ptList[i], color);
	}
	// Then connect the first and last points
	LedmatDrawLine(ptList[nCoords - 1], ptList[0], color);
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

