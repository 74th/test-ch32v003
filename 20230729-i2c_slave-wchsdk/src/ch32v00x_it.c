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
  // uint16_t STAR1, STAR2 __attribute__((unused));
  // STAR1 = I2C1->STAR1;
  // STAR2 = I2C1->STAR2;

  uint16_t STAR1 = I2C_GetLastEvent(I2C1);
  uint16_t STAR2 = 0;

  printf("EV STAR1: 0x%04x STAR2: 0x%04x\r\n", STAR1, STAR2);

  I2C1->CTLR1 |= I2C_CTLR1_ACK;

  if (STAR1 & I2C_STAR1_ADDR) // 0x0002
  {
    printf("ADDR\r\n");
    // 最初のイベント
    // read でも write でも必ず最初に呼ばれる
  }

  if (STAR1 & I2C_STAR1_RXNE) // 0x0040
  {
    printf("RXNE write \r\n");
    uint8_t data = I2C1->DATAR;
    printf("data: 0x%02x\r\n", data);
  }

  if (STAR1 & I2C_STAR1_TXE) // 0x0080
  {
    // 1byte の read イベント（slave -> master）
    printf("TXE write event:\r\n");
    I2C1->DATAR = 0x01;
  }

  if (STAR1 & I2C_STAR1_STOPF) // 0x0010
  {
    printf("STOPF\r\n");
    I2C_ClearFlag(I2C1, I2C_FLAG_STOPF);
  }
  //
}

void I2C1_ER_IRQHandler(void)
{
  uint16_t STAR1 = I2C1->STAR1;

  printf("ER STAR1: 0x%04x\r\n", STAR1);

  if (STAR1 & I2C_STAR1_BERR)         // 0x0100
  {                                   // Bus error
    I2C1->STAR1 &= ~(I2C_STAR1_BERR); // Clear error
  }

  if (STAR1 & I2C_STAR1_ARLO)         // 0x0200
  {                                   // Arbitration lost error
    I2C1->STAR1 &= ~(I2C_STAR1_ARLO); // Clear error
  }

  if (STAR1 & I2C_STAR1_AF)         // 0x0400
  {                                 // Acknowledge failure
    I2C1->STAR1 &= ~(I2C_STAR1_AF); // Clear error
  }
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
