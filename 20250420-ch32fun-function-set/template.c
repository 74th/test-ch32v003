/* Template app on which you can build your own. */

#include "ch32fun.h"
#include <stdio.h>

#define USE_UINT_MULTIPLIER 0
#define USE_UINT_DEVISION 0
#define USE_FLOAT_MULTIPIER 0
#define USE_FLOAT_DIVISION 0
#define USE_DEBUG_PRINTF 0

int main()
{
    SystemInit();

    volatile uint32_t count = 0;

    while (1)
    {
#if USE_DEBUG_PRINTF
        printf("%d\r\n", count);
#endif
#if USE_UINT_MULTIPLIER
        volatile uint32_t num1 = count * 12345;
#if USE_DEBUG_PRINTF
        printf("%d\r\n", num1);
#endif
#else
        volatile uint32_t num1 = count;
#if USE_DEBUG_PRINTF
        printf("%d\r\n", num1);
#endif
#endif

#if USE_UINT_DEVISION
        volatile uint32_t num2 = count / 12345;
#if USE_DEBUG_PRINTF
        printf("%d\r\n", num2);
#endif
#else
        volatile uint32_t num2 = count;
#if USE_DEBUG_PRINTF
        printf("%d\r\n", num2);
#endif
#endif

#if USE_FLOAT_MULTIPIER
        volatile float num3 = count;
        num3 = num3 * 12.345f;
#if USE_DEBUG_PRINTF
        printf("%f\r\n", num3);
#endif
#else
        volatile uint32_t num3 = count;
#if USE_DEBUG_PRINTF
        printf("%d\r\n", num3);
#endif
#endif

#if USE_FLOAT_DIVISION
        volatile float num4 = count;
        num4 = num4 / 12.345f;
#if USE_DEBUG_PRINTF
        printf("%f\r\n", num4);
#endif
#else
        volatile uint32_t num4 = count;
#if USE_DEBUG_PRINTF
        printf("%d\r\n", num4);
#endif
#endif

        count++;
        Delay_Ms(100);
    }
}
