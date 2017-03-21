#ifndef I2C_H
#define I2C_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include "systick_handler.h"

#define SCL_PIN GPIO8
#define SDA_PIN GPIO7
#define DEFAULT_TIMEOUT 10	//Mills

typedef enum I2CStatus
{
	I2C_OK,
	I2C_TIMEOUT,
	I2C_BUSY,
	I2C_ERROR,
	I2C_ACK_FAIL,
	I2C_NOT_IMPLEMENTED
} I2CStatus;

void SetupI2C(void);

I2CStatus ReadRegI2C(uint8_t i2c_addr, uint8_t reg, uint8_t size,
			uint8_t *buf);

I2CStatus WriteRegI2C(uint8_t i2c_addr, uint8_t reg, uint8_t size,
			uint8_t *buf);

I2CStatus ReadRegBlockingI2C(uint8_t i2c_addr, uint8_t reg, uint8_t size,
			uint8_t *buf);

I2CStatus WriteRegBlockingI2C(uint8_t i2c_addr, uint8_t reg, uint8_t size,
			uint8_t *buf);


I2CStatus ReadI2C(uint8_t i2c_addr, uint8_t size,
			uint8_t *buf);

I2CStatus WriteI2C(uint8_t i2c_addr, uint8_t size,
			uint8_t *buf);

I2CStatus ReadBlockingI2C(uint8_t i2c_addr, uint8_t size,
			uint8_t *buf);

I2CStatus WriteBlockingI2C(uint8_t i2c_addr, uint8_t size,
			uint8_t *buf);

void ClearAddrFlag(void);

I2CStatus WaitForFlag(volatile uint32_t* reg, uint32_t flag, uint32_t set_clear, uint32_t timeout);

I2CStatus WaitForAddr(uint32_t timeout);

#endif