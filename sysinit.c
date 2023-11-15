#include "hal/hal_common.h"
#include <inttypes.h>

uint32_t SystemCoreClock = HLCK_FREQUENCY;

// 0000 0000 0010 0000 0010 0000 1000 0000

void SystemInit(void) {
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));
    FLASH->ACR |= FLASH_LATENCY | FLASH_ACR_PRFTBE;
    RCC->CR &= ~RCC_CR_PLLON;
    RCC->CFGR &= ~RCC_CFGR_PLLMUL;
    RCC->CFGR |= ((PLL_M - 2) << RCC_CFGR_PLLMUL_Pos);
    RCC->CFGR &= ~RCC_CFGR_PLLSRC;
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0 ) spin(1);
    RCC->CFGR |= (AHB_PRE << RCC_CFGR_HPRE_Pos);
    RCC->CFGR |= (APB2_PRE << RCC_CFGR_PPRE2_Pos);
    RCC->CFGR |= (APB1_PRE << RCC_CFGR_PPRE1_Pos);
    RCC->CFGR |= 2U << RCC_CFGR_SW_Pos;
    while ((RCC->CFGR & 12) == 0) spin(1);

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    SysTick_Config(SystemCoreClock / 1000);
}

volatile uint32_t s_ticks;

void SysTick_Handler(void) {
    s_ticks++;
}
