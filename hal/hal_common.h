#ifndef _HAL_COMMON_H_
#define _HAL_COMMON_H_

#include <inttypes.h>
#include <stdbool.h>
#include "stm32f303x8.h"

#define BIT(x) (1U << (x))

enum { APB1_PRE = 0, APB2_PRE = 5, AHB_PRE = 9 };
enum { PLL_HSI = 4, PLL_M = 8 };

#define SYS_FREQUENCY ((PLL_HSI * PLL_M) * 1000000)
#define HLCK_FREQUENCY (AHB_PRE > 7 ? (SYS_FREQUENCY / (2 << (AHB_PRE - 8))) : AHB_PRE)    
#define APB1_FREQUENCY (HLCK_FREQUENCY / (APB1_PRE > 3 ? BIT((APB1_PRE - 3)) : 1))
#define APB2_FREQUENCY (HLCK_FREQUENCY / (APB2_PRE > 3 ? BIT((APB2_PRE - 3)) : 1))    
#define FLASH_LATENCY (HLCK_FREQUENCY > 48000000 ? 2 : HLCK_FREQUENCY > 24000000 ? 1 : 0)

static inline void spin(volatile uint32_t count) {
    while (count--) asm("nop");
}

extern volatile uint32_t s_ticks;

static inline void delay_ms(uint32_t ms) {
    uint32_t t = s_ticks + ms;
    while (s_ticks < t) (void) 0;
}

#endif