#include "debug.h"

/* Global define */
#define Size 6
#define RXAdderss 0x02
#define TxAdderss 0x02

/* Global Variable */
u8 TxData[Size] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
u8 RxData[5][Size];

/*********************************************************************
 * @fn      IIC_Init
 *
 * @brief   Initializes the IIC peripheral.
 *
 * @return  none
 */
void IIC_Init(u32 bound, u16 address)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    I2C_InitTypeDef I2C_InitTSturcture = {0};

    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitTSturcture.I2C_OwnAddress1 = 0x4e;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitTSturcture.I2C_ClockSpeed = 100000;
    I2C_Init(I2C1, &I2C_InitTSturcture);

    NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    I2C_ITConfig(I2C1, I2C_IT_ERR, ENABLE);

    I2C_ITConfig(I2C1, I2C_IT_BUF, ENABLE);

    // I2C1->CTLR1 |= I2C_CTLR1_PE;
    I2C_Cmd(I2C1, ENABLE);

    // I2C1->CTLR1 |= I2C_CTLR1_ACK;
    I2C_AcknowledgeConfig(I2C1, ENABLE);

    // I2C_CalculatePEC(I2C1, ENABLE);
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    IIC_Init(100000, 0x02);

    printf("start\r\n");

    while (1)
        ;
}
