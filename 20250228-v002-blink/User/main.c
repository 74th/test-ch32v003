#include "debug.h"


#define LED_PIN1 GPIO_Pin_1

void init_gpio(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOC, ENABLE);
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    printf("init\r\n");

    init_gpio();

    printf("start\r\n");


    // int count = 0;
    // uint8_t ledState = 0;

    while(1)
    {

        // printf("loop %d\r\n", count);

        // USART_Printf_Init(115200);

        GPIO_WriteBit(GPIOC, GPIO_Pin_4, 0);
        GPIO_WriteBit(GPIOC, GPIO_Pin_2, 1);
        GPIO_WriteBit(GPIOC, GPIO_Pin_1, 0);
        GPIO_WriteBit(GPIOA, GPIO_Pin_2, 0);
        Delay_Ms(300);
        GPIO_WriteBit(GPIOC, GPIO_Pin_4, 0);
        GPIO_WriteBit(GPIOC, GPIO_Pin_2, 0);
        GPIO_WriteBit(GPIOC, GPIO_Pin_1, 0);
        GPIO_WriteBit(GPIOA, GPIO_Pin_2, 1);
        Delay_Ms(300);
        GPIO_WriteBit(GPIOC, GPIO_Pin_4, 0);
        GPIO_WriteBit(GPIOC, GPIO_Pin_2, 0);
        GPIO_WriteBit(GPIOC, GPIO_Pin_1, 1);
        GPIO_WriteBit(GPIOA, GPIO_Pin_2, 0);
        Delay_Ms(300);
        GPIO_WriteBit(GPIOC, GPIO_Pin_4, 1);
        GPIO_WriteBit(GPIOC, GPIO_Pin_2, 0);
        GPIO_WriteBit(GPIOC, GPIO_Pin_1, 0);
        GPIO_WriteBit(GPIOA, GPIO_Pin_2, 0);
        Delay_Ms(300);
    }
}
