#include "ch32v003fun.h"
#include "ch32v003_GPIO_branchless.h"
#include <stdio.h>
#include <stdbool.h>

#define SWDIO_PIN GPIOv_from_PORT_PIN(GPIO_port_D, 1)
#define NRST_PIN GPIOv_from_PORT_PIN(GPIO_port_D, 7)
#define SAMPLE_PIN GPIOv_from_PORT_PIN(GPIO_port_C, 5)

#define TEST_PIN1 GPIOv_from_PORT_PIN(GPIO_port_C, 4)
#define TEST_PIN2 GPIOv_from_PORT_PIN(GPIO_port_C, 6)
#define TEST_PIN3 GPIOv_from_PORT_PIN(GPIO_port_C, 7)

#define ENABLE_NRST_TO_GPIO
// #define ENABLE_SWDIO_TO_GPIO

// #define TEST_INPUT

void test_output()
{
	GPIO_pinMode(TEST_PIN1, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(TEST_PIN2, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(TEST_PIN3, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);

	GPIO_pinMode(SAMPLE_PIN, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
#ifdef ENABLE_NRST_TO_GPIO
	GPIO_pinMode(NRST_PIN, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
#endif
#ifdef ENABLE_SWDIO_TO_GPIO
	GPIO_pinMode(SWDIO_PIN, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
#endif

	while (1)
	{
		GPIO_digitalWrite_0(TEST_PIN1);
		GPIO_digitalWrite_0(TEST_PIN2);
		GPIO_digitalWrite_0(TEST_PIN3);
		GPIO_digitalWrite_0(SAMPLE_PIN);
#ifdef ENABLE_NRST_TO_GPIO
		GPIO_digitalWrite_0(NRST_PIN);
#endif
#ifdef ENABLE_SWDIO_TO_GPIO
		GPIO_digitalWrite_0(SWDIO_PIN);
#endif

		Delay_Ms(300);

		GPIO_digitalWrite_1(TEST_PIN1);
		GPIO_digitalWrite_1(TEST_PIN2);
		GPIO_digitalWrite_1(TEST_PIN3);
		GPIO_digitalWrite_1(SAMPLE_PIN);
#ifdef ENABLE_NRST_TO_GPIO
		GPIO_digitalWrite_1(NRST_PIN);
#endif
#ifdef ENABLE_SWDIO_TO_GPIO
		GPIO_digitalWrite_1(SWDIO_PIN);
#endif
		Delay_Ms(300);
	}
}

void test_input()
{
	GPIO_pinMode(TEST_PIN1, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(TEST_PIN2, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(TEST_PIN3, GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);

#ifdef ENABLE_NRST_TO_GPIO
	GPIO_pinMode(NRST_PIN, GPIO_pinMode_I_pullUp, GPIO_Speed_10MHz);
#endif
#ifdef ENABLE_SWDIO_TO_GPIO
	GPIO_pinMode(SWDIO_PIN, GPIO_pinMode_I_pullUp, GPIO_Speed_10MHz);
#endif
	GPIO_pinMode(SAMPLE_PIN, GPIO_pinMode_I_pullUp, GPIO_Speed_10MHz);

	while (1)
	{
#ifdef ENABLE_NRST_TO_GPIO
		bool v1 = GPIO_digitalRead(NRST_PIN);
		if (v1)
		{
			GPIO_digitalWrite_1(TEST_PIN1);
		}
		else
		{
			GPIO_digitalWrite_0(TEST_PIN1);
		}
#endif

#ifdef ENABLE_SWDIO_TO_GPIO
		bool v2 = GPIO_digitalRead(SWDIO_PIN);
		if (v2)
		{
			GPIO_digitalWrite_1(TEST_PIN2);
		}
		else
		{
			GPIO_digitalWrite_0(TEST_PIN2);
		}
#endif

		bool v3 = GPIO_digitalRead(SAMPLE_PIN);
		if (v3)
		{
			GPIO_digitalWrite_1(TEST_PIN3);
		}
		else
		{
			GPIO_digitalWrite_0(TEST_PIN3);
		}

		Delay_Ms(100);
	}
}

int main()
{
	SystemInit();
	funGpioInitAll();

#ifdef ENABLE_SWDIO_TO_GPIO
	// AFIOを有効化
	RCC->APB2PCENR |= RCC_APB2Periph_AFIO;

	// AFIOでSWDIOをDisable
	AFIO->PCFR1 |= AFIO_PCFR1_SWJ_CFG_DISABLE;
#endif

#ifdef TEST_INPUT
	test_input();
#else
	test_output();
#endif
}