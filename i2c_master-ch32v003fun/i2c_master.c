#include "ch32v003fun.h"
#include "i2c_master.h"
#include <stdio.h>

#define I2C_SLAVE_ADDRESS 0x74
#define LOOP_MS 1000
// #define LED_PIN GPIOv_from_PORT_PIN(GPIO_port_C, 3)
#define LED_PIN GPIOv_from_PORT_PIN(GPIO_port_D, 4)
#define SELECT_U1_PIN GPIOv_from_PORT_PIN(GPIO_port_A, 1)
#define SELECT_U2_PIN GPIOv_from_PORT_PIN(GPIO_port_A, 2)
#define SELECT_U3_PIN GPIOv_from_PORT_PIN(GPIO_port_D, 0)
#define SELECT_U4_PIN GPIOv_from_PORT_PIN(GPIO_port_C, 0)
#define BTN1_PIN GPIOv_from_PORT_PIN(GPIO_port_D, 2)
#define BTN2_PIN GPIOv_from_PORT_PIN(GPIO_port_C, 7)
#define BTN3_PIN GPIOv_from_PORT_PIN(GPIO_port_C, 6)
#define BTN4_PIN GPIOv_from_PORT_PIN(GPIO_port_C, 5)

void I2C1_EV_IRQHandler(void) __attribute__((interrupt));
void I2C1_ER_IRQHandler(void) __attribute__((interrupt));

void init_rcc(void)
{
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD;
    RCC->APB1PCENR |= RCC_APB1Periph_I2C1;
}

/**
 * set up i2c master
 * https://github.com/cnlohr/ch32v003fun/blob/ee60fd756aa015be799e430d230a8f33266421de/examples/i2c_oled/i2c.h#L321
 * https://github.com/cnlohr/ch32v003fun/blob/ee60fd756aa015be799e430d230a8f33266421de/examples/i2c_oled/i2c.h#L40
 */
void init_i2c_master(uint8_t address)
{

    // PC1 is SDA, 10MHz Output, alt func, open-drain
    GPIOC->CFGLR &= ~(0xf << (4 * 1));
    GPIOC->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_OD_AF) << (4 * 1);

    // PC2 is SCL, 10MHz Output, alt func, open-drain
    GPIOC->CFGLR &= ~(0xf << (4 * 2));
    GPIOC->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_OD_AF) << (4 * 2);

    uint16_t tempreg;

    // Reset I2C1 to init all regs
    RCC->APB1PRSTR |= RCC_APB1Periph_I2C1;
    RCC->APB1PRSTR &= ~RCC_APB1Periph_I2C1;

    I2C1->CTLR1 |= I2C_CTLR1_SWRST;
    I2C1->CTLR1 &= ~I2C_CTLR1_SWRST;

    uint32_t prerate = 2000000;
    uint32_t clkrate = 1000000;

    // Set module clock frequency
    tempreg = I2C1->CTLR2;
    tempreg &= ~I2C_CTLR2_FREQ;
    tempreg |= (FUNCONF_SYSTEM_CORE_CLOCK / prerate) & I2C_CTLR2_FREQ;
    I2C1->CTLR2 = tempreg;

    // Set clock config
    tempreg = 0;
    tempreg = (FUNCONF_SYSTEM_CORE_CLOCK / (25 * clkrate)) & I2C_CKCFGR_CCR;
    tempreg |= I2C_CKCFGR_DUTY;
    tempreg |= I2C_CKCFGR_FS;
    I2C1->CKCFGR = tempreg;

    // Enable I2C
    I2C1->CTLR1 |= I2C_CTLR1_PE;

    // set ACK mode
    I2C1->CTLR1 |= I2C_CTLR1_ACK;
}

#define TIMEOUT_MAX 100000
// https://github.com/cnlohr/ch32v003fun/blob/ee60fd756aa015be799e430d230a8f33266421de/examples/i2c_oled/i2c.h#L115
#define I2C_EVENT_MASTER_MODE_SELECT ((uint32_t)0x00030001)               /* BUSY, MSL and SB flag */
#define I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ((uint32_t)0x00070082) /* BUSY, MSL, ADDR, TXE and TRA flags */
#define I2C_EVENT_MASTER_BYTE_TRANSMITTED ((uint32_t)0x00070084)          /* TRA, BUSY, MSL, TXE and BTF flags */
#define I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ((uint32_t)0x00030002)    /* BUSY, SB, ADDR flags */
#define I2C_EVENT_MASTER_BYTE_RECEIVED ((uint32_t)0x00030040)             /* TRA, SB, ADDR flags */

/*
 * check for 32-bit event codes
 * https://github.com/cnlohr/ch32v003fun/blob/ee60fd756aa015be799e430d230a8f33266421de/examples/i2c_oled/i2c.h#L122
 */
uint8_t i2c_chk_evt(uint32_t event_mask)
{
    /* read order matters here! STAR1 before STAR2!! */
    uint32_t status = I2C1->STAR1 | (I2C1->STAR2 << 16);
    return (status & event_mask) == event_mask;
}

/*
 * low-level packet send for blocking polled operation via i2c
 * https://github.com/cnlohr/ch32v003fun/blob/ee60fd756aa015be799e430d230a8f33266421de/examples/i2c_oled/i2c.h#L242
 */
uint8_t i2c_send(uint8_t addr, uint8_t *data, uint8_t sz)
{
    int32_t timeout;

    I2C1->CTLR1 |= I2C_CTLR1_ACK;

    // wait for not busy
    timeout = TIMEOUT_MAX;
    while ((I2C1->STAR2 & I2C_STAR2_BUSY) && (timeout--))
        ;
    if (timeout == -1)
    {
        printf("i2c error: waiting for not BUSY is timeout\r\n");
        return -1;
    }

    // Set START condition
    I2C1->CTLR1 |= I2C_CTLR1_START;

    // wait for master mode select
    timeout = TIMEOUT_MAX;
    while ((!i2c_chk_evt(I2C_EVENT_MASTER_MODE_SELECT)) && (timeout--))
        ;
    if (timeout == -1)
    {
        printf("i2c error: waiting for master select is timeout\r\n");
        return -1;
    }

    // send 7-bit address + write flag
    I2C1->DATAR = addr << 1;

    // wait for transmit condition
    timeout = TIMEOUT_MAX;
    while ((!i2c_chk_evt(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && (timeout--))
        ;
    if (timeout == -1)
    {
        printf("i2c error: waiting for transmit condition is timeout\r\n");
        return -1;
    }

    // send data one byte at a time
    while (sz--)
    {
        printf("i2c send: [%2d] 0x%02x\r\n", sz, *data);
        // wait for TX Empty
        timeout = TIMEOUT_MAX;
        while (!(I2C1->STAR1 & I2C_STAR1_TXE) && (timeout--))
            ;
        if (timeout == -1)
        {
            printf("i2c error: waiting for data send is timeout\r\n");
            return -1;
        }

        // send command
        I2C1->DATAR = *data++;
    }

    // wait for tx complete
    timeout = TIMEOUT_MAX;
    while ((!i2c_chk_evt(I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--))
        ;
    if (timeout == -1)
    {
        printf("i2c error: waiting for tx complete is timeout\r\n");
        return -1;
    }

    // set STOP condition
    I2C1->CTLR1 |= I2C_CTLR1_STOP;

    // we're happy
    return 0;
}

int i2c_receive(uint8_t addr, uint8_t *buf, uint8_t sz)
{
    int32_t timeout;

    I2C1->CTLR1 |= I2C_CTLR1_ACK;

    // wait for not busy
    timeout = TIMEOUT_MAX;
    while ((I2C1->STAR2 & I2C_STAR2_BUSY) && (timeout--))
        ;
    if (timeout == -1)
    {
        printf("i2c error: waiting for not BUSY is timeout\r\n");
        return -1;
    }

    // Set START condition
    I2C1->CTLR1 |= I2C_CTLR1_START;

    // wait for master mode select
    timeout = TIMEOUT_MAX;
    while ((!i2c_chk_evt(I2C_EVENT_MASTER_MODE_SELECT)) && (timeout--))
        ;
    if (timeout == -1)
    {
        printf("i2c error: waiting for master select is timeout\r\n");
        return -1;
    }

    // send 7-bit address + receive flag
    I2C1->DATAR = addr << 1 | 0x1;

    // wait for transmit condition
    timeout = TIMEOUT_MAX;
    while ((!i2c_chk_evt(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) && (timeout--))
        ;
    if (timeout == -1)
    {
        printf("i2c error: waiting for transmit condition is timeout %04x, %04x\r\n", I2C1->STAR1, I2C1->STAR2);
        return -1;
    }

    // send data one byte at a time
    while (sz--)
    {
        if (sz == 0)
        {
            printf("i2c receive last byte\r\n");
            I2C1->CTLR1 &= ~I2C_CTLR1_ACK;
        }
        // wait for TX Empty
        timeout = TIMEOUT_MAX;
        while (!(I2C1->STAR1 & I2C_STAR1_RXNE) && (timeout--))
            ;
        if (timeout == -1)
        {
            printf("i2c error: receiving for data send is timeout\r\n");
            return -1;
        }

        // receive command
        *buf = I2C1->DATAR;
        printf("i2c receive: [%2d] 0x%02x\r\n", sz, *buf);
        buf++;
    }

    // set STOP condition
    I2C1->CTLR1 |= I2C_CTLR1_STOP;

    // // wait for tx complete
    // timeout = TIMEOUT_MAX;
    // while ((!i2c_chk_evt(I2C_EVENT_MASTER_BYTE_RECEIVED)) && (timeout--))
    //     ;
    // if (timeout == -1)
    // {
    //     printf("i2c error: waiting for rx complete is timeout\r\n");
    //     return -1;
    // }

    // we're happy
    return 0;
}

int main()
{
    SystemInit();
    init_rcc();

    printf("initialize\r\n");

    init_i2c_master(I2C_SLAVE_ADDRESS);

    uint8_t buf[3 * 4];
    for (int i = 0; i < 3 * 4; i++)
    {
        if (i % 3 == 0)
        {
            buf[i] = 0xf0;
        }
        else
        {
            buf[i] = 0x00;
        }
    }

    printf("initialize done\r\n");

    Delay_Ms(100);

    printf("test i2c\r\n");
    i2c_send(0x74, buf, 3 * 4);

    i2c_receive(0x74, buf, 4);
    printf("received: 0x%02x\r\n", buf[0]);
    Delay_Ms(LOOP_MS);
    i2c_receive(0x74, buf, 4);
    printf("received: 0x%02x\r\n", buf[0]);
    Delay_Ms(LOOP_MS);
    i2c_receive(0x74, buf, 4);
    printf("received: 0x%02x\r\n", buf[0]);
    Delay_Ms(LOOP_MS);
    i2c_receive(0x74, buf, 4);
    printf("received: 0x%02x\r\n", buf[0]);
    Delay_Ms(LOOP_MS);
    i2c_receive(0x74, buf, 4);
    printf("received: 0x%02x\r\n", buf[0]);
    Delay_Ms(LOOP_MS);
    i2c_receive(0x74, buf, 4);
    printf("received: 0x%02x\r\n", buf[0]);
    Delay_Ms(LOOP_MS);
    i2c_receive(0x74, buf, 4);
    printf("received: 0x%02x\r\n", buf[0]);
    printf("test i2c done\r\n");

    while (1)
    {
        i2c_receive(0x74, buf, 1);
        printf("received: 0x%02x\r\n", buf[0]);
        Delay_Ms(LOOP_MS);
    }
}
