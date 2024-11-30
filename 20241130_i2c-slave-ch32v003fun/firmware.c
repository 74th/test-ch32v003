#include "ch32v003fun.h"
#include <stdio.h>
#include <stdbool.h>

volatile uint8_t i2c_registers[32] = {0x00};

void I2C1_EV_IRQHandler(void) __attribute__((interrupt));
void I2C1_ER_IRQHandler(void) __attribute__((interrupt));

void init_rcc(void)
{
	RCC->CFGR0 &= ~(0x1F << 11);

	RCC->APB2PCENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_ADC1;
	RCC->APB1PCENR |= RCC_APB1Periph_I2C1;
}

void init_i2c_slave(uint8_t address)
{
	// https://github.com/cnlohr/ch32v003fun/blob/master/examples/i2c_slave/i2c_slave.h

	// PC1 is SDA, 10MHz Output, alt func, open-drain
	GPIOC->CFGLR &= ~(0xf << (4 * 1));
	GPIOC->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_OD_AF) << (4 * 1);

	// PC2 is SCL, 10MHz Output, alt func, open-drain
	GPIOC->CFGLR &= ~(0xf << (4 * 2));
	GPIOC->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_OD_AF) << (4 * 2);

	// Reset I2C1 to init all regs
	RCC->APB1PRSTR |= RCC_APB1Periph_I2C1;
	RCC->APB1PRSTR &= ~RCC_APB1Periph_I2C1;

	I2C1->CTLR1 |= I2C_CTLR1_SWRST;
	I2C1->CTLR1 &= ~I2C_CTLR1_SWRST;

	// Set module clock frequency
	uint32_t prerate = 2000000; // I2C Logic clock rate, must be higher than the bus clock rate
	I2C1->CTLR2 |= (FUNCONF_SYSTEM_CORE_CLOCK / prerate) & I2C_CTLR2_FREQ;

	// Enable interrupts
	I2C1->CTLR2 |= I2C_CTLR2_ITBUFEN;
	I2C1->CTLR2 |= I2C_CTLR2_ITEVTEN; // Event interrupt
	I2C1->CTLR2 |= I2C_CTLR2_ITERREN; // Error interrupt

	NVIC_EnableIRQ(I2C1_EV_IRQn); // Event interrupt
	NVIC_SetPriority(I2C1_EV_IRQn, 2 << 4);
	NVIC_EnableIRQ(I2C1_ER_IRQn); // Error interrupt
	// Set clock configuration
	uint32_t clockrate = 1000000;																	 // I2C Bus clock rate, must be lower than the logic clock rate
	I2C1->CKCFGR = ((FUNCONF_SYSTEM_CORE_CLOCK / (3 * clockrate)) & I2C_CKCFGR_CCR) | I2C_CKCFGR_FS; // Fast mode 33% duty cycle
	// I2C1->CKCFGR = ((APB_CLOCK/(25*clockrate))&I2C_CKCFGR_CCR) | I2C_CKCFGR_DUTY | I2C_CKCFGR_FS; // Fast mode 36% duty cycle
	// I2C1->CKCFGR = (APB_CLOCK/(2*clockrate))&I2C_CKCFGR_CCR; // Standard mode good to 100kHz

	// Set I2C address
	I2C1->OADDR1 = address << 1;

	// Enable I2C
	I2C1->CTLR1 |= I2C_CTLR1_PE;

	// Acknowledge the first address match event when it happens
	I2C1->CTLR1 |= I2C_CTLR1_ACK;
}

uint16_t i2c_scan_start_position = 0;
uint16_t i2c_scan_length = 0;
bool i2c_firse_byte = true;
bool i2c_mosi_event = false;
bool on_mosi_event = false;
bool address2matched = false;

void I2C1_EV_IRQHandler(void)
{
	uint16_t STAR1, STAR2 __attribute__((unused));
	STAR1 = I2C1->STAR1;
	STAR2 = I2C1->STAR2;

#ifdef FUNCONF_USE_UARTPRINTF
	printf("EV STAR1: 0x%04x STAR2: 0x%04x\r\n", STAR1, STAR2);
#endif

	I2C1->CTLR1 |= I2C_CTLR1_ACK;

	if (STAR1 & I2C_STAR1_ADDR) // 0x0002
	{
		// 最初のイベント
		// read でも write でも必ず最初に呼ばれる
		address2matched = !!(STAR2 & I2C_STAR2_DUALF);
		printf("ADDR %d\r\n", address2matched);
		i2c_firse_byte = true;
		i2c_mosi_event = false;
		i2c_scan_start_position = 0;
		i2c_scan_length = 0;
	}

	if (STAR1 & I2C_STAR1_RXNE) // 0x0040
	{
		if (i2c_firse_byte)
		{
			i2c_scan_start_position = I2C1->DATAR;
			i2c_scan_length = 0;
			i2c_firse_byte = false;
			i2c_mosi_event = true;
			printf("RXNE write event: first_write %x\r\n", i2c_scan_start_position);
		}
		else
		{
			uint16_t pos = i2c_scan_start_position + i2c_scan_length;
			uint8_t v = I2C1->DATAR;
			printf("RXNE write event: pos:%x v:%x\r\n", pos, v);
			if (pos < 32)
			{
				i2c_registers[pos] = v;
				i2c_scan_length++;
			}
		}
	}

	if (STAR1 & I2C_STAR1_TXE) // 0x0080
	{
		if (i2c_firse_byte)
		{
			i2c_scan_start_position = I2C1->DATAR;
			i2c_scan_length = 0;
			i2c_firse_byte = false;
			i2c_mosi_event = true;
			printf("TXE write event: first_write:%x\r\n", i2c_scan_start_position);
		}
		else
		{
			// 1byte の read イベント（slave -> master）
			uint16_t pos = i2c_scan_start_position + i2c_scan_length;
			printf("TXE write event: pos:%x\r\n", pos);
			if (pos < 32)
			{
				// 1byte 送信
				uint8_t data = i2c_registers[pos];
				I2C1->DATAR = data;
				i2c_scan_length++;
			}
			else
			{
				// 1byte 送信
				I2C1->DATAR = 0x00;
			}
		}
	}

	if (STAR1 & I2C_STAR1_STOPF)
	{
		I2C1->CTLR1 &= ~(I2C_CTLR1_STOP);
		if (i2c_mosi_event)
		{
			printf("on_mosi_event\r\n");
			on_mosi_event = true;
		}
	}
}

void I2C1_ER_IRQHandler(void)
{
	uint16_t STAR1 = I2C1->STAR1;

#ifdef FUNCONF_USE_UARTPRINTF
	printf("ER STAR1: 0x%04x\r\n", STAR1);
#endif

	if (STAR1 & I2C_STAR1_BERR)			  // 0x0100
	{									  // Bus error
		I2C1->STAR1 &= ~(I2C_STAR1_BERR); // Clear error
	}

	if (STAR1 & I2C_STAR1_ARLO)			  // 0x0200
	{									  // Arbitration lost error
		I2C1->STAR1 &= ~(I2C_STAR1_ARLO); // Clear error
	}

	if (STAR1 & I2C_STAR1_AF)			// 0x0400
	{									// Acknowledge failure
		I2C1->STAR1 &= ~(I2C_STAR1_AF); // Clear error
	}
}

void main_loop()
{
}

int main()
{
	SystemInit();
	funGpioInitAll();

	init_rcc();
	init_i2c_slave(0x20);

	volatile int i = 0;
	while (1)
	{
		Delay_Ms(500);
		uint8_t a = i2c_registers[0x12];
		uint8_t b = i2c_registers[0x13];
		printf("0x12:%x 0x13:%x\r\n", a, b);
	}
}