#include "ch32v003fun.h"
#include "ch32v003_GPIO_branchless.h"
#include <stdio.h>
#include <stdbool.h>

#define P1 GPIOv_from_PORT_PIN(GPIO_port_D, 4)
#define P2 GPIOv_from_PORT_PIN(GPIO_port_D, 5)
#define P3 GPIOv_from_PORT_PIN(GPIO_port_D, 6)
#define P4 GPIOv_from_PORT_PIN(GPIO_port_D, 7)
#define P5 GPIOv_from_PORT_PIN(GPIO_port_A, 1)
#define P6 GPIOv_from_PORT_PIN(GPIO_port_A, 2)
// P7 VSS
#define P8 GPIOv_from_PORT_PIN(GPIO_port_D, 0)
// P9 VDD
#define P10 GPIOv_from_PORT_PIN(GPIO_port_C, 0)
#define P11 GPIOv_from_PORT_PIN(GPIO_port_C, 1)
#define P12 GPIOv_from_PORT_PIN(GPIO_port_C, 2)
#define P13 GPIOv_from_PORT_PIN(GPIO_port_C, 3)
#define P14 GPIOv_from_PORT_PIN(GPIO_port_C, 4)
#define P15 GPIOv_from_PORT_PIN(GPIO_port_C, 5)
#define P16 GPIOv_from_PORT_PIN(GPIO_port_C, 6)
#define P17 GPIOv_from_PORT_PIN(GPIO_port_C, 7)
// P18 SWDIC
// #define P18 GPIOv_from_PORT_PIN(GPIO_port_D, 1)
#define P19 GPIOv_from_PORT_PIN(GPIO_port_D, 2)
#define P20 GPIOv_from_PORT_PIN(GPIO_port_D, 3)

#define ENABLE_DEBUGGING 0

bool enable_led = false;

void setup()
{
	GPIO_port_enable(GPIO_port_A);
	GPIO_port_enable(GPIO_port_C);
	GPIO_port_enable(GPIO_port_D);

	GPIO_pinMode(P1, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(P2, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(P3, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(P4, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(P5, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(P6, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(P8, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(P10, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(P11, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(P12, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(P13, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(P14, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(P15, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(P16, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(P17, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	// GPIO_pinMode(P18, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(P19, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(P20, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);

	Delay_Ms(1);

	GPIO_digitalWrite_0(P1);
	GPIO_digitalWrite_0(P2);
	GPIO_digitalWrite_0(P3);
	GPIO_digitalWrite_0(P4);
	GPIO_digitalWrite_0(P5);
	GPIO_digitalWrite_0(P6);
	GPIO_digitalWrite_0(P8);
	GPIO_digitalWrite_0(P10);
	GPIO_digitalWrite_0(P11);
	GPIO_digitalWrite_0(P12);
	GPIO_digitalWrite_0(P13);
	GPIO_digitalWrite_0(P14);
	GPIO_digitalWrite_0(P15);
	GPIO_digitalWrite_0(P16);
	GPIO_digitalWrite_0(P17);
	GPIO_digitalWrite_0(P19);
	GPIO_digitalWrite_0(P20);
}

#define DURATION_MS 200

void main_loop()
{
	GPIO_digitalWrite_0(P20);
	GPIO_digitalWrite_1(P1);
	Delay_Ms(DURATION_MS);

	GPIO_digitalWrite_0(P1);
	GPIO_digitalWrite_1(P2);
	Delay_Ms(DURATION_MS);

	GPIO_digitalWrite_0(P2);
	GPIO_digitalWrite_1(P3);
	Delay_Ms(DURATION_MS);

	GPIO_digitalWrite_0(P3);
	GPIO_digitalWrite_1(P4);
	Delay_Ms(DURATION_MS);

	GPIO_digitalWrite_0(P4);
	GPIO_digitalWrite_1(P5);
	Delay_Ms(DURATION_MS);

	GPIO_digitalWrite_0(P5);
	GPIO_digitalWrite_1(P6);
	Delay_Ms(DURATION_MS);

	GPIO_digitalWrite_0(P6);
	GPIO_digitalWrite_1(P8);
	Delay_Ms(DURATION_MS);

	GPIO_digitalWrite_0(P8);
	GPIO_digitalWrite_1(P10);
	Delay_Ms(DURATION_MS);

	GPIO_digitalWrite_0(P10);
	GPIO_digitalWrite_1(P11);
	Delay_Ms(DURATION_MS);

	GPIO_digitalWrite_0(P11);
	GPIO_digitalWrite_1(P12);
	Delay_Ms(DURATION_MS);

	GPIO_digitalWrite_0(P12);
	GPIO_digitalWrite_1(P13);
	Delay_Ms(DURATION_MS);

	GPIO_digitalWrite_0(P13);
	GPIO_digitalWrite_1(P14);
	Delay_Ms(DURATION_MS);

	GPIO_digitalWrite_0(P14);
	GPIO_digitalWrite_1(P15);
	Delay_Ms(DURATION_MS);

	GPIO_digitalWrite_0(P15);
	GPIO_digitalWrite_1(P16);
	Delay_Ms(DURATION_MS);

	GPIO_digitalWrite_0(P16);
	GPIO_digitalWrite_1(P17);
	Delay_Ms(DURATION_MS);

	GPIO_digitalWrite_0(P17);
	GPIO_digitalWrite_1(P19);
	Delay_Ms(DURATION_MS);

	GPIO_digitalWrite_0(P19);
	GPIO_digitalWrite_1(P20);
	Delay_Ms(DURATION_MS);
}

int main()
{
	SystemInit();

	setup();

	for (;;)
		main_loop();
}
