#include "ch32v003fun.h"
#include "ch32v003_GPIO_branchless.h"
#include <stdio.h>
#include <stdbool.h>

#define PIN_C4 GPIOv_from_PORT_PIN(GPIO_port_C, 4)
#define PIN_C5 GPIOv_from_PORT_PIN(GPIO_port_C, 5)
#define PIN_C6 GPIOv_from_PORT_PIN(GPIO_port_C, 6)
#define PIN_C7 GPIOv_from_PORT_PIN(GPIO_port_C, 7)

#define ENABLE_DEBUGGING 0

uint8_t last_pin = 4;

void setup()
{
	GPIO_port_enable(GPIO_port_C);

	GPIO_pinMode(PIN_C4, GPIO_pinMode_I_pullUp, GPIO_Speed_10MHz);
	GPIO_pinMode(PIN_C5, GPIO_pinMode_I_pullUp, GPIO_Speed_10MHz);
	GPIO_pinMode(PIN_C6, GPIO_pinMode_I_pullUp, GPIO_Speed_10MHz);
	GPIO_pinMode(PIN_C7, GPIO_pinMode_I_pullUp, GPIO_Speed_10MHz);

	Delay_Ms(1);

	last_pin = OB->Data0;
}

// https://github.com/cnlohr/ch32v003fun/blob/master/examples/optiondata/optiondata.c
void FlashOptionData(uint16_t data0, uint16_t data1)
{
	volatile uint16_t hold[6]; // array to hold current values while erasing

	// The entire 64 byte data block of the "User-selected words" will be erased
	// so we need to keep a copy of the content for re-writing after erase.
	// Save a few (20) bytes code space by moving 32 bits at a time.
	// 			hold[0]=OB->RDPR;
	// 			hold[1]=OB->USER;
	// 			hold[2]=data0;
	// 			hold[3]=data1;
	// 			hold[4]=OB->WRPR0;
	// 			hold[5]=OB->WRPR1;
	uint32_t *hold32p = (uint32_t *)hold;
	uint32_t *ob32p = (uint32_t *)OB_BASE;
	hold32p[0] = ob32p[0];				// Copy RDPR and USER
	hold32p[1] = data0 + (data1 << 16); // Copy in the two Data values to be written
	hold32p[2] = ob32p[2];				// Copy WRPR0 and WEPR1

	// Unlock both the general Flash and the User-selected words
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
	FLASH->OBKEYR = FLASH_KEY1;
	FLASH->OBKEYR = FLASH_KEY2;

	FLASH->CTLR |= CR_OPTER_Set; // OBER RW Perform user-selected word erasure
	FLASH->CTLR |= CR_STRT_Set;	 // STRT RW1 Start. Set 1 to start an erase action,hw automatically clears to 0
	while (FLASH->STATR & FLASH_BUSY)
		;						   // Wait for flash operation to be done
	FLASH->CTLR &= CR_OPTER_Reset; // Disable erasure mode

	// Write the held values back one-by-one
	FLASH->CTLR |= CR_OPTPG_Set; // OBG  RW Perform user-selected word programming
	uint16_t *ob16p = (uint16_t *)OB_BASE;
	for (int i = 0; i < sizeof(hold) / sizeof(hold[0]); i++)
	{
		ob16p[i] = hold[i];
		while (FLASH->STATR & FLASH_BUSY)
			; // Wait for flash operation to be done
	}
	FLASH->CTLR &= CR_OPTPG_Reset; // Disable programming mode

	FLASH->CTLR |= CR_LOCK_Set; // Lock flash memories again

	return;
}

#define DURATION_MS 500

void main_loop()
{
	Delay_Ms(DURATION_MS);

	for (uint8_t i = 4; i <= 7; i++)
	{
		if (!GPIO_digitalRead(GPIOv_from_PORT_PIN(GPIO_port_C, i)))
		{
			if (last_pin != i)
			{
				last_pin = i;
				FlashOptionData(last_pin, 0);
				printf("Pin %d pressed\r\n", i);
			}
		}
	}

	printf("last Pin %d\r\n", last_pin);
}

int main()
{
	SystemInit();

	setup();

	printf("start\r\n");

	for (;;)
		main_loop();
}
