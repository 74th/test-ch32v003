/* Template app on which you can build your own. */

#include "ch32v003fun.h"
#include "ch32v003_GPIO_branchless.h"
#include <stdio.h>

#define LED_PIN GPIOv_from_PORT_PIN(GPIO_port_C, 1)

int main()
{
	SystemInit();

	GPIO_port_enable(GPIO_port_C);
	GPIO_pinMode(LED_PIN, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);

	while (1)
	{
		Delay_Ms(500);
		printf("Hello\r\n");
		GPIO_digitalWrite_1(LED_PIN);
		Delay_Ms(500);
		printf("World\r\n");
		GPIO_digitalWrite_0(LED_PIN);
	}
}
