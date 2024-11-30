#include "ch32v003fun.h"
#include "ch32v003_GPIO_branchless.h"
#include "i2c_slave.h"
#include <stdio.h>
#include <stdbool.h>

volatile uint8_t i2c_registers[32] = {0x00};

void onWrite(uint8_t reg, uint8_t length)
{
}

void setup()
{
	// Initialize I2C slave
	funPinMode(PC1, GPIO_CFGLR_OUT_10Mhz_AF_OD); // SDA
	funPinMode(PC2, GPIO_CFGLR_OUT_10Mhz_AF_OD); // SCL
	SetupI2CSlave(0x20, i2c_registers, sizeof(i2c_registers), onWrite, NULL, false);
}

void main_loop()
{
}

int main()
{
	SystemInit();
	funGpioInitAll();

	setup();

	while (1)
	{
		main_loop();
	}
}