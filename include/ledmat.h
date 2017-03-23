#ifndef LEDMAT_H
#define LEDMAT_H

#include <stdint.h>

#include "helpers.h"

// Backpack configured with A0~A2 set to 0
#define LEDMAT_I2C_ADDR	0x70

// System oscillator control
// Must be enabled first on device boot
#define LEDMAT_SYSCLOCK_REG				0x20
#define LEDMAT_SYSCLOCK_DISABLE			0x00
#define LEDMAT_SYSCLOCK_ENABLE			0x01

// LED matrix display controls
#define LEDMAT_DISPLAY_REG				0x80
#define LEDMAT_DISPLAY_OFF				0x00
#define LEDMAT_DISPLAY_ON				0x01
#define LEDMAT_DISPLAY_BLINK_OFF		0x00
#define LEDMAT_DISPLAY_BLINK_2HZ		0x02
#define LEDMAT_DISPLAY_BLINK_1HZ		0x04
#define LEDMAT_DISPLAY_BLINK_05HZ		0x06

// LED matrix brightness controls
// Second nibble is increasing brightness on [0x0, 0xF]
// in increments of 1/16th
#define LEDMAT_BRIGHTNESS_REG			0xE0
#define LEDMAT_BRIGHTNESS_MIN			0x00
#define LEDMAT_BRIGHTNESS_MAX			0x0F

// Display buffer memory access
#define LEDMAT_BUFFER_REG				0x00

// LED color control enums
#define LEDMAT_LED_OFF					0
#define LEDMAT_LED_RED					1
#define LEDMAT_LED_GREEN				2
#define LEDMAT_LED_YELLOW				3

// Dimensions of the LED matrix
#define LEDMAT_NROWS					8
#define LEDMAT_NCOLS					8

// Full-screen number look-up table
// Generated using http://xantorohara.github.io/led-matrix-editor/
static const uint8_t _LedmatNumTable[10][LEDMAT_NROWS] = {
	{0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38}, // 0
	{0x10, 0x18, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38}, // 1
	{0x38, 0x44, 0x40, 0x40, 0x20, 0x10, 0x08, 0x7c}, // 2
	{0x38, 0x44, 0x40, 0x30, 0x40, 0x40, 0x44, 0x38}, // 3
	{0x40, 0x60, 0x50, 0x48, 0x44, 0x7c, 0x40, 0x40}, // 4
	{0x7c, 0x04, 0x04, 0x3c, 0x40, 0x40, 0x44, 0x38}, // 5
	{0x38, 0x44, 0x04, 0x3c, 0x44, 0x44, 0x44, 0x38}, // 6
	{0x7c, 0x40, 0x40, 0x20, 0x10, 0x08, 0x08, 0x08}, // 7
	{0x38, 0x44, 0x44, 0x38, 0x44, 0x44, 0x44, 0x38}, // 8
	{0x38, 0x44, 0x44, 0x44, 0x78, 0x40, 0x44, 0x38}  // 9
};

// Buffer of pixel data matching the memory map of the LED matrix
// First byte of each row controls red LEDs,
// second byte controls green LEDs
union {
	// Modification-friendly layout by row and color
	struct {
		uint8_t green;
		uint8_t red;
	} row[LEDMAT_NROWS];
	// Memory-mapped layout identical to on the LED matrix
	uint16_t mmapped[LEDMAT_NROWS];
	// Easy-output format for i2c
	uint8_t bytestream[2 * LEDMAT_NROWS];
} _LedmatDisplayBuff;

// Buffer modification macros
#define _RED_ON(x,y)			_LedmatDisplayBuff.row[y].red |= (1 << x)
#define _RED_OFF(x,y)			_LedmatDisplayBuff.row[y].red &= ~(1 << x)
#define _GREEN_ON(x,y)			_LedmatDisplayBuff.row[y].green |= (1 << x)
#define _GREEN_OFF(x,y)			_LedmatDisplayBuff.row[y].green &= ~(1 << x)
#define _LED_SET_OFF(x,y)		{_RED_OFF(x,y); _GREEN_OFF(x,y);}
#define _LED_SET_RED(x,y)		{_RED_ON(x,y); _GREEN_OFF(x,y);}
#define _LED_SET_GREEN(x,y)		{_RED_OFF(x,y); _GREEN_ON(x,y);}
#define _LED_SET_YELLOW(x,y)	{_RED_ON(x,y); _GREEN_ON(x,y);}


// Initialize the LED matrix display
void SetupLedmat(void);

// Send the entire current display buffer to the LED matrix
// This takes a nontrivial amount of time
// TODO: double buffering eventually maybe?
void LedmatRefreshDisplay(void);

// Clear the local display buffer, does not refresh screen
void LedmatClearBuffer(void);

// Drawing primitives, affect changes on _LedmatDisplayBuffer
// NOTE: LedmatDraw* functions do NOT refresh the display, call LedmatRefreshDisplay()
//       to push your changes to the display
void LedmatDrawPixel(Point2 pt, uint8_t color);
void LedmatDrawLine(Point2 ptStart, Point2 ptEnd, uint8_t color);
void LedmatDrawPolygon(Point2* ptList, int nCoords, uint8_t color);
void LedmatDrawNumber(uint8_t number, uint8_t color);
void LedmatDrawFloodfill(uint8_t color);

// Command a single pixel to change color immediately 
void LedmatCommandPixel(Point2 pt, uint8_t color);

#endif // LEDMAT_H
