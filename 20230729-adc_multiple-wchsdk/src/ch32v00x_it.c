#include <ch32v00x_it.h>
#include <stdio.h>
#include "main.h"

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void I2C1_EV_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void I2C1_ER_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

volatile uint8_t i2c_registers[32] = {0x00};

void I2C1_EV_IRQHandler(void)
{
}

void I2C1_ER_IRQHandler(void)
{
}

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  printf("HardFault\r\n");
  while (1)
  {
  }
}
