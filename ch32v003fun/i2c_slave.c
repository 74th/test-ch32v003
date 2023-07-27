#include "ch32v003fun.h"
#include "i2c_slave.h"
#include <stdio.h>

// The I2C slave library uses a one byte address so you can extend the size of this array up to 256 registers
// note that the register set is modified by interrupts, to prevent the compiler from accidently optimizing stuff
// away make sure to declare the register array volatile

volatile uint8_t i2c_registers[32] = {0x00};

void I2C1_EV_IRQHandler(void) __attribute__((interrupt));
void I2C1_ER_IRQHandler(void) __attribute__((interrupt));

void I2C1_EV_IRQHandler(void)
{
    uint16_t STAR1, STAR2 __attribute__((unused));
    STAR1 = I2C1->STAR1;
    STAR2 = I2C1->STAR2;

    printf("EV STAR1: %x STAR2: %x\r\n", STAR1, STAR2);

    I2C1->CTLR1 |= I2C_CTLR1_ACK;

    if (STAR1 & I2C_STAR1_ADDR)
    {
        printf("ADDR\r\n");
        // Start event
        i2c_slave_state.first_write = 1;                   // Next write will be the offset
        i2c_slave_state.position = i2c_slave_state.offset; // Reset position
    }

    if (STAR1 & I2C_STAR1_RXNE)
    {
        printf("RXNE write event: pos:%d\r\n", i2c_slave_state.position);
        // Write event
        if (i2c_slave_state.first_write)
        { // First byte written, set the offset
            i2c_slave_state.offset = I2C1->DATAR;
            i2c_slave_state.position = i2c_slave_state.offset;
            i2c_slave_state.first_write = 0;
        }
        else
        { // Normal register write
            if (i2c_slave_state.position < i2c_slave_state.size)
            {
                i2c_slave_state.registers[i2c_slave_state.position] = I2C1->DATAR;
                i2c_slave_state.position++;
            }
        }
    }

    if (STAR1 & I2C_STAR1_TXE)
    { // Read event
        printf("TXE write event: pos:%d\r\n", i2c_slave_state.position);
        if (i2c_slave_state.position < i2c_slave_state.size)
        {
            I2C1->DATAR = i2c_slave_state.registers[i2c_slave_state.position];
            i2c_slave_state.position++;
        }
        else
        {
            I2C1->DATAR = 0x00;
        }
    }
}

void I2C1_ER_IRQHandler(void)
{
    printf("ER\r\n");

    uint16_t STAR1 = I2C1->STAR1;

    if (STAR1 & I2C_STAR1_BERR)
    {                                     // Bus error
        I2C1->STAR1 &= ~(I2C_STAR1_BERR); // Clear error
    }

    if (STAR1 & I2C_STAR1_ARLO)
    {                                     // Arbitration lost error
        I2C1->STAR1 &= ~(I2C_STAR1_ARLO); // Clear error
    }

    if (STAR1 & I2C_STAR1_AF)
    {                                   // Acknowledge failure
        I2C1->STAR1 &= ~(I2C_STAR1_AF); // Clear error
    }
}

int main()
{
    SystemInit();

    printf("initialize\r\n");

    SetupI2CSlave(0x9, i2c_registers, sizeof(i2c_registers));

    // Enable GPIOD and set pin 0 to output
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOD;
    GPIOD->CFGLR &= ~(0xf << (4 * 0));
    GPIOD->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP) << (4 * 0);

    printf("initialize done\r\n");

    while (1)
    {
        if (i2c_registers[0] & 1)
        { // Turn on LED (PD0) if bit 1 of register 0 is set
            GPIOD->BSHR |= 1 << 16;
        }
        else
        {
            GPIOD->BSHR |= 1;
        }
    }
}
