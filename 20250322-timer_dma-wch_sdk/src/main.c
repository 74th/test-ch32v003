#include <ch32v00x.h>
#include <debug.h>

#define GPIOA_LED_PIN GPIO_Pin_1
#define GPIOC_LED_PIN GPIO_Pin_2
#define GPIOD_LED_PIN GPIO_Pin_4

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void Delay_Init(void);
void Delay_Ms(uint32_t n);
void DMA1_Channel2_IRQHandler(void) __attribute__((interrupt)) __attribute__((section(".srodata")));
void DMA1_Channel2_IRQHandler(void)
{
    DMA1->INTFCR = DMA1_IT_GL2;
}

uint32_t gpioc_dma_buf[16] = {0};
uint32_t gpiod_dma_buf[16] = {0};
uint32_t gpioa_dma_buf[16] = {0};

void init(void)
{
    printf("init\r\n");

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure = {0};
    DMA_InitTypeDef DMA_InitStructure = {0};

    // GPIOにクロック供給
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM, ENABLE);

    // GPIO出力
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

    GPIO_InitStructure.GPIO_Pin = GPIOA_LED_PIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIOC_LED_PIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIOD_LED_PIN;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    // TIM1_CH1 DMA1_Channel2
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&GPIOA->BSHR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)gpioa_dma_buf;
    DMA_InitStructure.DMA_BufferSize = sizeof(gpioa_dma_buf) / sizeof(gpioa_dma_buf[0]);
    DMA_DeInit(DMA1_Channel2);
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel2, ENABLE);

    // TIM1_CH2 DMA1_Channel3
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&GPIOC->BSHR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)gpioc_dma_buf;
    DMA_InitStructure.DMA_BufferSize = sizeof(gpioc_dma_buf) / sizeof(gpioc_dma_buf[0]);
    DMA_DeInit(DMA1_Channel3);
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel3, ENABLE);

    // TIM1_CH3 DMA1_Channel6
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&GPIOD->BSHR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)gpiod_dma_buf;
    DMA_InitStructure.DMA_BufferSize = sizeof(gpiod_dma_buf) / sizeof(gpiod_dma_buf[0]);
    DMA_DeInit(DMA1_Channel6);
    DMA_Init(DMA1_Channel6, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel6, ENABLE);

    // 48MHzで動作するため、48MHz / 480 = 100kHz (10us) でカウント
    TIM_TimeBaseInitStructure.TIM_Prescaler = 480;
    // 10us ごとにトリガ
    TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    // PWMではないが、PWMモードで出力設定する
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_Pulse = 6;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    TIM_GenerateEvent(TIM1, TIM_EventSource_Trigger);

    // TIM1_CH1 DMA1_Channel2
    TIM_DMACmd(TIM1, TIM_DMA_CC1, ENABLE);
    // TIM1_CH2 DMA1_Channel3
    TIM_DMACmd(TIM1, TIM_DMA_CC2, ENABLE);
    // TIM1_CH3 DMA1_Channel6
    TIM_DMACmd(TIM1, TIM_DMA_CC3, ENABLE);
    TIM_DMACmd(TIM1, TIM_DMA_Update, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();

    // SDI printf の有効化
    // ビルドフラグ SDI_PRINT=1 が必要
    // wlink sdi-print enable を実行し、USBシリアルをリスンする
    // SDI_Printf_Enable();

    // USART1 printf の有効化
    USART_Printf_Init(115200);

    for (int i = 0; i < 16; i++)
    {
        if (i & 2)
        {
            gpioa_dma_buf[i] = GPIOA_LED_PIN;
            gpioc_dma_buf[i] = GPIOC_LED_PIN;
            gpiod_dma_buf[i] = GPIOD_LED_PIN;
        }
        else
        {
            gpioa_dma_buf[i] = GPIOA_LED_PIN << 16;
            gpioc_dma_buf[i] = GPIOC_LED_PIN << 16;
            gpiod_dma_buf[i] = GPIOD_LED_PIN << 16;
        }
    }

    init();
    Delay_Ms(100);

    printf("RCC->AHBPCENR, 0x%lx\r\n", RCC->AHBPCENR);
    printf("RCC->APB2PCENR, 0x%lx\r\n", RCC->APB2PCENR);
    printf("GPIOC->CFGLR, 0x%lx\r\n", GPIOC->CFGLR);
    printf("DMA1_Channel2->CFGR, 0x%lx\r\n", DMA1_Channel2->CFGR);
    printf("DMA1_Channel2->CNTR, 0x%lx\r\n", DMA1_Channel2->CNTR);
    printf("DMA1_Channel2->MADDR, 0x%lx\r\n", DMA1_Channel2->MADDR);
    printf("DMA1_Channel2->PADDR, 0x%lx\r\n", DMA1_Channel2->PADDR);
    printf("TIM1->CTLR1, 0x%x\r\n", TIM1->CTLR1);
    printf("TIM1->CTLR2, 0x%x\r\n", TIM1->CTLR2);
    printf("TIM1->SMCFGR, 0x%x\r\n", TIM1->SMCFGR);
    printf("TIM1->DMAINTENR, 0x%x\r\n", TIM1->DMAINTENR);
    printf("TIM1->INTFR, 0x%x\r\n", TIM1->INTFR);
    printf("TIM1->SWEVGR, 0x%x\r\n", TIM1->SWEVGR);
    printf("TIM1->CHCTLR1, 0x%x\r\n", TIM1->CHCTLR1);
    printf("TIM1->CHCTLR2, 0x%x\r\n", TIM1->CHCTLR2);
    printf("TIM1->CCER, 0x%x\r\n", TIM1->CCER);
    printf("TIM1->PSC, 0x%x\r\n", TIM1->PSC);
    printf("TIM1->ATRLR, 0x%x\r\n", TIM1->ATRLR);
    printf("TIM1->RPTCR, 0x%x\r\n", TIM1->RPTCR);
    printf("TIM1->CH1CVR, 0x%lx\r\n", TIM1->CH1CVR);
    printf("TIM1->CH2CVR, 0x%lx\r\n", TIM1->CH2CVR);
    printf("TIM1->CH3CVR, 0x%lx\r\n", TIM1->CH3CVR);
    printf("TIM1->CH4CVR, 0x%lx\r\n", TIM1->CH4CVR);
    printf("TIM1->BDTR, 0x%x\r\n", TIM1->BDTR);
    printf("TIM1->DMACFGR, 0x%x\r\n", TIM1->DMACFGR);
    printf("TIM1->DMAADR, 0x%x\r\n", TIM1->DMAADR);

    printf("start\r\n");

    uint32_t count = 0;

    while (1)
    {
        printf("Step %d %d\r\n", TIM1->CNT, DMA1_Channel2->CNTR);

        // GPIOC->BSHR = GPIOC_LED_PIN;
        // Delay_Ms(100);
        // GPIOC->BSHR = GPIOC_LED_PIN << 16;
        Delay_Ms(1000);
    }
}

void NMI_Handler(void) {}
void HardFault_Handler(void)
{
    while (1)
    {
    }
}