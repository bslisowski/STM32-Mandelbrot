#ifndef _HAL_COMMON_H_
#define _HAL_COMMON_H_

#include <inttypes.h>
#include <stdbool.h>
#include "stm32f303x8.h"

#define BIT(x) (1U << (x))

enum { APB1_PRE = 0, APB2_PRE = 4, AHB_PRE = 8 };
enum { PLL_HSI = 4, PLL_M = 10 };

#define SYS_FREQUENCY ((PLL_HSI * PLL_M) * 1000000)         // 40MHz
#define HLCK_FREQUENCY (AHB_PRE > 7 ? (SYS_FREQUENCY / (2 << (AHB_PRE - 8))) : SYS_FREQUENCY)    // 20
#define APB1_FREQUENCY (HLCK_FREQUENCY / (APB1_PRE > 3 ? BIT((APB1_PRE - 3)) : 1))         // 20
#define APB2_FREQUENCY (HLCK_FREQUENCY / (APB2_PRE > 3 ? BIT((APB2_PRE - 3)) : 1))         // 10
#define FLASH_LATENCY (HLCK_FREQUENCY > 48000000 ? 2 : HLCK_FREQUENCY > 24000000 ? 1 : 0)

#define SYS_CORE_FREQUENCY (HLCK_FREQUENCY / 8)

static inline void spin(volatile uint32_t count) {
    while (count--) asm("nop");
}

extern volatile uint32_t s_ticks;

static inline void delay_ms(uint32_t ms) {
    uint32_t t = s_ticks + ms;
    while (s_ticks < t) (void) 0;
}

static inline bool timer_expired(uint32_t *t, uint32_t prd, uint32_t now) {
    if (now + prd < *t) *t = 0;
    if (*t == 0) *t = now + prd;
    if (*t > now) return false;
    *t = (now - *t) > prd ? now + prd : *t + prd;
    return true; 
}

#endif