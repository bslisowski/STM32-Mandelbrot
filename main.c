#include "hal/gpio.h"
#include "hal/usart.h"
#include "hal/hal_common.h"
#include "hal/pwm.h"
#include "hal/nucleo_f303k8_pins.h"
#include <stdio.h>
#include <stdbool.h>



int main(void) {

    uart_init(UART_DEBUG, 115200);
    init_pwm(PIN('B', 6), 100, 90);

    int32_t duty = 0;
    int8_t updown = 1;
    uint32_t timer = 0, period = 1;
    for (;;) {
        if (timer_expired(&timer, period, s_ticks)) {
            duty += updown * 10;
            if (duty <= 0) {
                updown = 1;
                TIM16->CCR1 = 0;
            }
            else if (duty >= 19999) {
                updown = -1;
                TIM16->CCR1 = 19999;
            }
            else {
                TIM16->CCR1 = (uint16_t)duty;
            }
        }
    }
    return 0;
}