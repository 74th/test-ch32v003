#include "debug.h"

#define LED_PIN GPIO_Pin_1

void init_gpio(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();

    Delay_Ms(5000);

    // SDI_Printf_Enable();
    USART_Printf_Init(115200);


    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf("ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    printf("init\r\n");

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOC | RCC_PB2Periph_AFIO, ENABLE);
    RCC_PB1PeriphClockCmd(RCC_PB1Periph_PWR, ENABLE);
    RCC_LSICmd(ENABLE);

    init_gpio();

    EXTI_InitTypeDef EXTI_InitStructure = {0};

    EXTI_InitStructure.EXTI_Line = EXTI_Line9;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);


    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
        ;

    // LSI 128kHz
    // Prescaler 10240 -> 128kHz / 10240 = 12.5Hz = 80ms
    // window value 31
    // 80ms * 31 = 2000ms = 2s
    PWR_AWU_SetPrescaler(PWR_AWU_Prescaler_10240);
    PWR_AWU_SetWindowValue(25);
    PWR_AutoWakeUpCmd(ENABLE);

    printf("start\r\n");

    int count = 0;
    uint8_t ledState = 0;


    while(1)
    {
        printf("loop\r\n");

        printf("go to standby: %d\r\n\r\n", count);

        PWR_EnterSTANDBYMode(PWR_STANDBYEntry_WFE);

        USART_Printf_Init(115200);

        printf("\r\nAuto wake up: %d \r\n", count);

        GPIO_WriteBit(GPIOC, GPIO_Pin_1, ledState);
        ledState ^= 1;

        count++;
    }
}
