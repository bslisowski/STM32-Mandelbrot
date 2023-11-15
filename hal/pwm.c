#include "pwm.h"
#include <stdio.h>
#include "nucleo_f303k8_pins.h"

int init_pwm(uint16_t pin, uint32_t freq, uint8_t dc) {

    // if (pin != TIM16_CH1N){
    //     printf("ERR: Invalid pin\r\n");
    //     return -1;
    // }
    uint32_t f = PWM_FREQ(freq, APB1_FREQUENCY);
    if (f >> 16) {
        printf("ERR: Invalid frequency\r\n");
        return -1;
    }
    if (dc > 100) {
        printf("ERR: Invalid duty cycle\r\n");
        return -1;
    }
    uint16_t d = (uint16_t)PWM_DC(f, d);
    _init_pwm(pin, freq, dc);
    return 0;
}

int set_duty_cycle(uint16_t pin, uint8_t dc) {
    // if (pin != TIM16_CH1N){
    //     printf("ERR: Invalid pin\r\n");
    //     return -1;
    // }
    (void)pin;
    if (dc > 100) {
        printf("ERR: Invalid duty cycle\r\n");
        return -1;
    }
    _set_duty_cycle(TIM16, dc);
    return 0;
}