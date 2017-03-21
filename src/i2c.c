#include "i2c.h"

void SetupI2C(void) {
	// Setup GPIO
	rcc_periph_clock_enable(RCC_GPIOB);
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, SCL_PIN | SDA_PIN);
	gpio_set_af(GPIOB, GPIO_AF4, SCL_PIN | SDA_PIN);

	// Setup i2c
	// Enable periph clk
	rcc_periph_clock_enable(RCC_I2C1);

	// Reset and disable
	i2c_reset(I2C1);
	i2c_peripheral_disable(I2C1);

	// Setup for 400khz
	i2c_set_fast_mode(I2C1);
	i2c_set_dutycycle(I2C1, I2C_CCR_DUTY_16_DIV_9);
	i2c_set_clock_frequency(I2C1, I2C_CR2_FREQ_40MHZ);
	// CCR = 1/(400khz * (25 * Tpclk1)) = 1/(400khz * (25 * (1/40Mhz)) = 4
	i2c_set_ccr(I2C1, 4);
	// trise = (300 ns / Tpclk1) + 1= (300ns / (1/40Mhz)) + 1 = 13
	i2c_set_trise(I2C1, 13);

	// Enable
	i2c_peripheral_enable(I2C1);
}

// Check the busy flag
uint8_t CheckBusyFlag(void) {
	return I2C_SR2(I2C1) & I2C_SR2_BUSY;
}

void ClearAddrFlag(void) {
	//	Clear addr bit by reading sr1 then sr2
	volatile uint32_t temp = I2C_SR1(I2C1);
	temp = I2C_SR2(I2C1);
}

void ReadRegI2C(uint8_t i2c_addr, uint8_t reg, uint8_t size,
			uint8_t *buf) {

}

void WriteRegI2C(uint8_t i2c_addr, uint8_t reg, uint8_t size,
			uint8_t *buf) {

}

/**
 *	Reads an amount of data from a register on a device
 *	The data is writen to buf
**/
void ReadRegBlockingI2C(uint8_t i2c_addr, uint8_t reg, uint8_t size,
			uint8_t *buf) {
	// Wait for busy flag to be cleared
	while(CheckBusyFlag());

	// Disable pos
	I2C_CR1(I2C1) &= ~I2C_CR1_POS;

	// Write slave addr and reg
	//	Enable acknowledge
	i2c_enable_ack(I2C1);
	// 	Send start and enter master mode
	i2c_send_start(I2C1);
	//	Wait until SB flag set
	while(!(I2C_SR1(I2C1) & I2C_SR1_SB));
	//	Send the slave address
	i2c_send_7bit_address(I2C1, i2c_addr, I2C_READ);
	//	Wait for addr to be sent
	while(!(I2C_SR1(I2C1) & I2C_SR1_ADDR));
	//	Clear addr bit
	ClearAddrFlag();
	//	Wait for TXE
	while(!(I2C_SR1(I2C1) & I2C_SR1_TxE));
	//	Send register address
	i2c_send_data(I2C1, reg);
	// 	Wait for TXE
	while(!(I2C_SR1(I2C1) & I2C_SR1_TxE));

	// Generate a restart and write the address of the device again
	i2c_send_start(I2C1);
	//	Wait until SB flag set
	while(!(I2C_SR1(I2C1) & I2C_SR1_SB));
	//	Send the slave address
	i2c_send_7bit_address(I2C1, i2c_addr, I2C_READ);
	//	Wait for addr to be sent
	while(!(I2C_SR1(I2C1) & I2C_SR1_ADDR));

	// Read the data!
	if(size == 1) {
		// Disable acknowledge
		i2c_disable_ack(I2C1);
		// Clear address flag
		ClearAddrFlag();
		// Generate a stop
		i2c_send_stop(I2C1);
	} else if(size == 2) {
		// Disable acknowledge
		i2c_disable_ack(I2C1);
		// Enable Pos
		I2C_CR1(I2C1) |= I2C_CR1_POS;
		// Clear address flag
		ClearAddrFlag();
	} else {
		ClearAddrFlag();
	}

	while(size > 0) {
		if(size <= 3) {
			// One byte left
			if(size == 1) {
				// Wait for RXNE
				while(!(I2C_SR1(I2C1) & I2C_SR1_RxNE));

				// Copy data to buf
				(*buf++) = i2c_get_data(I2C1);
				--size;
			// Two bytes left
			} else if(size == 2) {
				// Wait for BTF flag
				while(!(I2C_SR1(I2C1) & I2C_SR1_BTF));

				// Send stop
				i2c_send_stop(I2C1);

				// Copy data to buf
				(*buf++) = i2c_get_data(I2C1);
				--size;
				(*buf++) = i2c_get_data(I2C1);
				--size;
			// Last 3 bytes
			} else {
				// Wait for BTF flag
				while(!(I2C_SR1(I2C1) & I2C_SR1_BTF));

				// Disable ack
				i2c_disable_ack(I2C1);

				// Copy data to buf
				(*buf++) = i2c_get_data(I2C1);
				--size;

				// Wait for BTF flag
				while(!(I2C_SR1(I2C1) & I2C_SR1_BTF));

				// Send stop
				i2c_send_stop(I2C1);

				// Copy data to buf
				(*buf++) = i2c_get_data(I2C1);
				--size;
				(*buf++) = i2c_get_data(I2C1);
				--size;
			}
		// More than 3 bytes
		} else {
			// Wait for RXNE
			while(!(I2C_SR1(I2C1) & I2C_SR1_RxNE));

			// Copy data to buf
			(*buf++) = i2c_get_data(I2C1);
			--size;

			// Check if btf set
			if(I2C_SR1(I2C1) & I2C_SR1_BTF) {
				// Copy data to buf
				(*buf++) = i2c_get_data(I2C1);
				--size;
			}
		}
	}
}

void WriteRegBlockingI2C(uint8_t i2c_addr, uint8_t reg, uint8_t size,
			uint8_t *buf) {
	// Wait for busy flag to be cleared
	while(CheckBusyFlag());

	// Disable pos
	I2C_CR1(I2C1) &= ~I2C_CR1_POS;

	// Send slave address and memory address
	//	Send start
	i2c_send_start(I2C1);
	//	Wait until SB flag set
	while(!(I2C_SR1(I2C1) & I2C_SR1_SB));
	//	Send addr
	i2c_send_7bit_address(I2C1, i2c_addr, I2C_WRITE);
	//	Wait for addr to be sent
	while(!(I2C_SR1(I2C1) & I2C_SR1_ADDR));
	//	Clear addr bit
	ClearAddrFlag();
	//	Wait for TXE
	while(!(I2C_SR1(I2C1) & I2C_SR1_TxE));
	//	Send register address
	i2c_send_data(I2C1, reg);

	// Send the data
	while(size > 0) {
		// 	Wait for TXE
		while(!(I2C_SR1(I2C1) & I2C_SR1_TxE));

		// Write data
		i2c_send_data(I2C1, (*buf++));
		--size;

		// Check if btf set
		if((I2C_SR1(I2C1) & I2C_SR1_BTF) && size != 0)  {
			// Write data
			i2c_send_data(I2C1, (*buf++));
			--size;
		}
	}


}


void ReadI2C(uint8_t i2c_addr, uint8_t size,
			uint8_t *buf) {

}

void WriteI2C(uint8_t i2c_addr, uint8_t size,
			uint8_t *buf) {

}

void ReadBlockingI2C(uint8_t i2c_addr, uint8_t size,
			uint8_t *buf) {

}

void WriteBlockingI2C(uint8_t i2c_addr, uint8_t size,
			uint8_t *buf) {

}