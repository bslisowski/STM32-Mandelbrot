#include "hal/gpio.h"
#include "hal/hal_common.h"
#include "hal/nucleo_f303k8_pins.h"
#include <stdio.h>
#include <stdbool.h>



int main(void) {
    uint16_t led = D13;
    gpio_config led_config = { 
        led,  
        GPIO_MODE_OUTPUT, 
        GPIO_OUTPUT_PP,
        GPIO_SPEED_LOW,
        GPIO_PULL_NONE,
        AF14
    };
    gpio_init(&led_config);

    uint8_t onoff = 0;

    for (;;) {
        onoff = !onoff;
        gpio_write(led, onoff);
        delay_ms(1000);
    }
    return 0;
}