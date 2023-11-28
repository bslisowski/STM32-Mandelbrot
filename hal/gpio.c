#include "gpio.h"


int gpio_init(gpio_config *config) {
    if (!IS_GPIO_ALL_INSTANCE(GPIO(PINBANK(config->pin)))) {
        return -1;
    }
    if (!IS_GPIO_MODE(config->mode)) {
        return -1;
    }
    if (!IS_GPIO_OUTPUT_TYPE(config->output_type)) {
        return -1;
    }
    if (!IS_GPIO_SPEED(config->output_speed)) {
        return -1;
    }
    if (!IS_GPIO_PULL(config->pull)) {
        return -1;
    }
    if (!IS_GPIO_AF(config->af)) {
        return -1;
    }
    _gpio_init(config);
    
    return 0;
}

int gpio_write(uint16_t pin, uint16_t val) {
    if (!IS_GPIO_ALL_INSTANCE(GPIO(PINBANK(pin)))) {
        return 1;
    }
    _gpio_write(pin, val);

    return 0;
}

uint16_t gpio_read(uint16_t pin) {
    if (!IS_GPIO_ALL_INSTANCE(GPIO(PINBANK(pin)))) {
        return 0;
    }
    return _gpio_read(pin);
}