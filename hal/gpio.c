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
    gpio_set_mode(config->pin, config->mode);
    gpio_set_output_type(config->pin, config->output_type);
    gpio_set_speed(config->pin, config->output_speed);
    gpio_set_output_type(config->pin, config->output_type);
    if (config->mode == GPIO_MODE_AF) {
        gpio_set_af(config->pin, config->af);
    }
    return 0;
}

int gpio_write(uint16_t pin, bool val) {
    if (!IS_GPIO_ALL_INSTANCE(GPIO(PINBANK(pin)))) {
        return -1;
    }
    _gpio_write(pin, val);

    return 0;
}

int gpio_read(uint16_t pin) {
    if (!IS_GPIO_ALL_INSTANCE(GPIO(PINBANK(pin)))) {
        return -1;
    }
    _gpio_read(pin);

    return 0;
}