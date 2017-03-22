#ifndef WII_H
#define WII_H

#include "i2c.h"

#define WII_ADDR 		0x52
#define WII_INIT_REG1       0xF0
#define WII_INIT_REG2       0xFB
#define WII_INIT_BYTE1      0x55
#define WII_INIT_BYTE2      0x00
#define WII_CMD_DATA_READ   0x00
#define WII_DATA_LENGTH     6

typedef union {
    uint8_t buffer[WII_DATA_LENGTH];
    struct {
        uint8_t joy_x;
        uint8_t joy_y;
        uint16_t accel_x_high : 8;
        uint16_t accel_y_high : 8;
        uint16_t accel_z_high : 8;
        // Note the following byte is reversed from the online spec
        // The BBB is likely little-endian and reverses the data storage
        // This is the one downside of bit-packed structs
        uint8_t button_z : 1;
        uint8_t button_c : 1;
        uint16_t accel_x_low : 2;
        uint16_t accel_y_low : 2;
        uint16_t accel_z_low : 2;
    } fields;
} NunchuckData;

void SetupWii(void);

NunchuckData ReadWii(void);

#endif