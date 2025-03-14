#include "debug.h"

#define LED_PIN GPIO_Pin_5

void init_gpio(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}


int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    
    SDI_Printf_Enable();

    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    printf("init\r\n");

    init_gpio();

    printf("start\r\n");

    while(1)
    {
        printf("loop\r\n");

        Delay_Ms(500);
        GPIO_WriteBit(GPIOB, LED_PIN, Bit_SET);
        Delay_Ms(500);
        GPIO_WriteBit(GPIOB, LED_PIN, Bit_RESET);
    }
}
