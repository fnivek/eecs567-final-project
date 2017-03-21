#ifndef I2C_H
#define I2C_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>

#define SCL_PIN GPIO8
#define SDA_PIN GPIO7

void SetupI2C(void);

void ReadRegI2C(uint8_t i2c_addr, uint8_t reg, uint8_t size,
			uint8_t *buf);

void WriteRegI2C(uint8_t i2c_addr, uint8_t reg, uint8_t size,
			uint8_t *buf);

void ReadRegBlockingI2C(uint8_t i2c_addr, uint8_t reg, uint8_t size,
			uint8_t *buf);

void WriteRegBlockingI2C(uint8_t i2c_addr, uint8_t reg, uint8_t size,
			uint8_t *buf);


void ReadI2C(uint8_t i2c_addr, uint8_t size,
			uint8_t *buf);

void WriteI2C(uint8_t i2c_addr, uint8_t size,
			uint8_t *buf);

void ReadBlockingI2C(uint8_t i2c_addr, uint8_t size,
			uint8_t *buf);

void WriteBlockingI2C(uint8_t i2c_addr, uint8_t size,
			uint8_t *buf);

uint8_t CheckBusyFlag(void);

void ClearAddrFlag(void);

#endif