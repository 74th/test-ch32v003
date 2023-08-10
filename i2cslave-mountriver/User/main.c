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
void IIC_Init2()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    I2C_InitTypeDef I2C_InitTSturcture = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    // I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_2;
    // I2C_InitTSturcture.I2C_OwnAddress1 = 0x4e;
    // I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    // I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    // I2C_InitTSturcture.I2C_ClockSpeed = 200000;
    // I2C_Init(I2C1, &I2C_InitTSturcture);

    I2C1->CTLR1 |= I2C_CTLR1_SWRST;
    I2C1->CTLR1 &= ~I2C_CTLR1_SWRST;

    I2C1->CTLR1 = 0x0000;
    I2C1->CTLR2 = 0x0718;
    I2C1->CKCFGR = 0x8010;
    I2C1->OADDR1 = 0x0012;

    printf("I2C1->CTLR1: 0x%04x\r\n", I2C1->CTLR1);
    printf("I2C1->CTLR2: 0x%04x\r\n", I2C1->CTLR2);
    printf("I2C1->CKCFGR: 0x%04x\r\n", I2C1->CKCFGR);
    printf("I2C1->DADDR1: 0x%04x\r\n", I2C1->OADDR1);

    NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    I2C_ITConfig(I2C1, I2C_IT_ERR, ENABLE);

    I2C_ITConfig(I2C1, I2C_IT_BUF, ENABLE);

    I2C1->CTLR1 |= I2C_CTLR1_PE;
    // I2C_Cmd(I2C1, ENABLE);

    I2C1->CTLR1 |= I2C_CTLR1_ACK;
    // I2C_AcknowledgeConfig(I2C1, ENABLE);

    // I2C_CalculatePEC(I2C1, ENABLE);
}

void IIC_Init()
{
    // Enable GPIOC and I2C
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOC;
    RCC->APB1PCENR |= RCC_APB1Periph_I2C1;

    // PC1 is SDA, 10MHz Output, alt func, open-drain
    GPIOC->CFGLR &= ~(0xf << (4 * 1));
    GPIOC->CFGLR |= (GPIO_Speed_10MHz | 12) << (4 * 1);

    // PC2 is SCL, 10MHz Output, alt func, open-drain
    GPIOC->CFGLR &= ~(0xf << (4 * 2));
    GPIOC->CFGLR |= (GPIO_Speed_10MHz | 12) << (4 * 2);

    // Reset I2C1 to init all regs
    RCC->APB1PRSTR |= RCC_APB1Periph_I2C1;
    RCC->APB1PRSTR &= ~RCC_APB1Periph_I2C1;

    I2C1->CTLR1 |= I2C_CTLR1_SWRST;
    I2C1->CTLR1 &= ~I2C_CTLR1_SWRST;

#define APB_CLOCK (((24000000)) * (2))

    // Set module clock frequency
    uint32_t prerate = 2000000; // I2C Logic clock rate, must be higher than the bus clock rate
    I2C1->CTLR2 |= (APB_CLOCK / prerate) & I2C_CTLR2_FREQ;

    // Enable interrupts
    I2C1->CTLR2 |= I2C_CTLR2_ITBUFEN;
    I2C1->CTLR2 |= I2C_CTLR2_ITEVTEN; // Event interrupt
    I2C1->CTLR2 |= I2C_CTLR2_ITERREN; // Error interrupt

    NVIC_EnableIRQ(I2C1_EV_IRQn); // Event interrupt
    NVIC_SetPriority(I2C1_EV_IRQn, 2 << 4);
    NVIC_EnableIRQ(I2C1_ER_IRQn); // Error interrupt

    // Set clock configuration
    uint32_t clockrate = 1000000;                                                    // I2C Bus clock rate, must be lower than the logic clock rate
    I2C1->CKCFGR = ((APB_CLOCK / (3 * clockrate)) & I2C_CKCFGR_CCR) | I2C_CKCFGR_FS; // Fast mode 33% duty cycle
    // I2C1->CKCFGR = ((APB_CLOCK/(25*clockrate))&I2C_CKCFGR_CCR) | I2C_CKCFGR_DUTY | I2C_CKCFGR_FS; // Fast mode 36% duty cycle
    // I2C1->CKCFGR = (APB_CLOCK/(2*clockrate))&I2C_CKCFGR_CCR; // Standard mode good to 100kHz

    // Set I2C address
    I2C1->OADDR1 = 0x12;

    printf("I2C1->CTLR1: 0x%04x\r\n", I2C1->CTLR1);
    printf("I2C1->CTLR2: 0x%04x\r\n", I2C1->CTLR2);
    printf("I2C1->CKCFGR: 0x%04x\r\n", I2C1->CKCFGR);
    printf("I2C1->DADDR1: 0x%04x\r\n", I2C1->OADDR1);

    // Enable I2C
    I2C1->CTLR1 |= I2C_CTLR1_PE;

    // Acknowledge the first address match event when it happens
    I2C1->CTLR1 |= I2C_CTLR1_ACK;
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
    printf("using wch-sdk\r\n");
    printf("initialize\r\n");
    IIC_Init();
    printf("initialize done\r\n");

    printf("start\r\n");

    while (1)
        ;
}
