#ifndef _PWM_HAL_H_
#define _PWM_HAL_H_
#include "hal_common.h"
#include "gpio.h"
#include <stdio.h>

#define PWM_FREQ(pwmfreq, timfreq) ((uint16_t)((timfreq / pwmfreq) - 1))
#define PWM_DC(pwmfreq, percent) ((uint16_t)(((float)pwmfreq * ((float)percent / 100.f))))

enum {PWM_MODE_1 = 6, PWM_MODE_2 };

// 
static inline void init_pwm(uint16_t pin, uint32_t freq, uint8_t dc_perc) {

    uint16_t f = PWM_FREQ(freq, APB2_FREQUENCY);
    uint16_t dc = PWM_DC(f, dc_perc);
    printf("PWM_FREQ = %d\r\n", f);
    printf("PWM_DC = %d\r\n", dc);
    gpio_config config = {
        pin, GPIO_MODE_AF, GPIO_OUTPUT_PP, GPIO_SPEED_HIGH, 0, AF1
    };
    gpio_init(&config);
    
    RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;

    TIM16->ARR = f;
    TIM16->CCR1 = 0;
    TIM16->CCMR1 |= TIM_CCMR1_OC1PE;
    TIM16->CCMR1 |= ((uint32_t)PWM_MODE_2 << TIM_CCMR1_OC1M_Pos);
    TIM16->CCMR1 &= ~(3U << TIM_CCMR1_CC1S_Pos);
    TIM16->CCER |= TIM_CCER_CC1NE;
    TIM16->CR2 |= TIM_CR2_CCPC;
    TIM16->EGR |= TIM_EGR_CC1G | TIM_EGR_UG;
    TIM16->BDTR |= TIM_BDTR_MOE;
    TIM16->CR1 |= TIM_CR1_ARPE;
    TIM16->CR1 |= TIM_CR1_CEN;


}
#endif
