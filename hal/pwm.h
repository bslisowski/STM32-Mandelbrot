#ifndef _PWM_HAL_H_
#define _PWM_HAL_H_
#include "hal_common.h"
#include "gpio.h"
#include <stdio.h>


#define PWM_FREQ(pwmfreq, timfreq) ((uint32_t)((timfreq / 2 / pwmfreq) - 1))
#define PWM_DC(pwmfreq, percent) ((uint32_t)(((float)pwmfreq * ((float)percent / 100.f))))

enum {PWM_MODE_1 = 6, PWM_MODE_2 };

static inline void _set_duty_cycle(TIM_TypeDef *tim, uint8_t dc) {
    uint32_t f = tim->ARR;
    uint16_t d = (uint16_t)PWM_DC(f, dc);
    tim->CCR1 = d;
}

static inline void enable_pwm(TIM_TypeDef *tim) {
    tim->CR1 |= TIM_CR1_CEN;
}
 
static inline void _init_pwm(uint16_t pin, uint32_t freq, uint16_t dc) {

    gpio_config config = {
        pin, GPIO_MODE_AF, GPIO_OUTPUT_PP, GPIO_SPEED_HIGH, 0, AF2
    };
    gpio_init(&config);
    
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    TIM3->ARR = freq;
    TIM3->CCR3 = dc;
    TIM3->PSC = 2;
    TIM3->CCMR2 |= TIM_CCMR2_OC3PE;
    TIM3->CCMR2 |= ((uint32_t)PWM_MODE_1 << TIM_CCMR2_OC3M_Pos);
    TIM3->CCMR2 &= ~(3U << TIM_CCMR2_CC3S_Pos);
    TIM3->CCER |= TIM_CCER_CC3E;
  //  TIM3->CCER &= ~TIM_CCER_CC3NE;
    // TIM3->CR2 |= TIM_CR2_CCPC; capture compare preloaded
    TIM3->EGR |= TIM_EGR_CC3G | TIM_EGR_UG;
    // TIM3->BDTR |= TIM_BDTR_MOE; main output enable
    TIM3->CR1 |= TIM_CR1_ARPE;
    TIM3->CR1 |= TIM_CR1_CEN;
}


int init_pwm(uint16_t pin, uint32_t freq, uint8_t dc);
int set_duty_cycle(uint16_t pin, uint8_t dc);

#endif
