#include "hal/gpio.h"
#include "hal/usart.h"
#include "hal/hal_common.h"
#include "hal/pwm.h"
#include "hal/nucleo_f303k8_pins.h"
#include "hal/spi.h"
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>


int main(void) {

    uart_init(UART_DEBUG, 115200);
    uint16_t pwm = PIN('B', 6);
    init_pwm(pwm, 100, 90);
    init_spi(MISO_ALT, MOSI_ALT, SCK_ALT);
    int32_t duty = 0;
    int8_t updown = 1;
    uint32_t timer = 0, timer1 = 0, period = 10, period1 = 1000;
    for (;;) {
        if (timer_expired(&timer, period, s_ticks)) {
            duty += updown;
            if (duty <= 0) {
                updown = 1;
            }
            else if (duty >= 100) {
                updown = -1;
            }
            set_duty_cycle(pwm, (uint8_t)duty);
        }
        if (timer_expired(&timer1, period1, s_ticks)) {
            spi_write_buf((uint16_t *)"Hello World\n", 12/2);
        }
    }
    return 0;
}