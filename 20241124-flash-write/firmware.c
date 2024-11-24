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

	last_pin = 4;
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

	for (;;)
		main_loop();
}
