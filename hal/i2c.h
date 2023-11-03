#ifndef _I2C_HAL_H_
#define _I2C_HAL_H_

#include "hal_common.h"
#include "gpio.h"

static inline void _i2c_init(uint16_t scl, uint16_t sda) {
    gpio_config config = {
        scl, GPIO_MODE_AF, GPIO_OUTPUT_OD, GPIO_SPEED_HIGH, 0, AF4
    };
    _gpio_init(&config);
    config.pin = sda;
    _gpio_init(&config);

    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    
}


#endif