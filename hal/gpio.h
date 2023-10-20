#ifndef _GPIO_HAL_H_
#define _GPIO_HAL_H_

#include "hal_common.h"

#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

#define GPIO(bank) ((GPIO_TypeDef *) (GPIOA_BASE + 0x400U * (bank)))

struct gc {
    uint16_t pin;
    uint8_t mode, output_type, output_speed, pull, af;
};
typedef struct gc gpio_config;

enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };
enum { GPIO_OUTPUT_PP, GPIO_OUTPUT_OD };
enum { GPIO_SPEED_LOW, GPIO_SPEED_MEDIUM, GPIO_SPEED_HIGH, GPIO_SPEED_INSANE };
enum { GPIO_PULL_NONE, GPIO_PULLUP, GPIO_PULLDOWN };
enum { AF0, AF1, AF2, AF3, AF4, AF5, AF6, AF7, AF8, AF9, AF10, AF12, AF13, AF14, AF15 };

#define IS_GPIO_MODE(m) ((m < 0 || m > 3) ? 0 : 1)
#define IS_GPIO_OUTPUT_TYPE(o) ((o < 0 || o > 1) ? 0 : 1)
#define IS_GPIO_SPEED(s) ((s < 0 || s > 3) ? 0 : 1)
#define IS_GPIO_PULL(p) ((p < 0 || p > 2) ? 0 : 1)
#define IS_GPIO_AF(a) ((a < 0 || a > 14) ? 0 : 1)

static inline void gpio_set_output_type(uint16_t pin, uint8_t output) {
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    if (output) {
        gpio->OTYPER |= BIT(PINNO(pin));
    }
    else {
        gpio->OTYPER &= ~BIT(PINNO(pin));
    }
}

static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
  GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
  int n = PINNO(pin);
  RCC->AHBENR |= (BIT(PINBANK(pin)) << RCC_AHBENR_GPIOAEN_Pos);
  gpio->MODER &= ~(3U << (n * 2));
  gpio->MODER |= (mode & 3U) << (n * 2);
}

static inline void gpio_set_speed(uint16_t pin, uint8_t speed) {
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    int n = PINNO(pin);
    gpio->OSPEEDR &= ~(3U << (n * 2));
    gpio->OSPEEDR |= (speed & 3U) << (n * 2);
}

static inline void gpio_set_af(uint16_t pin, uint8_t af_num) {
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    int n = PINNO(pin);
    gpio->AFR[n >> 3] &= ~(15UL << ((n & 7) * 4));
    gpio->AFR[n >> 3] |= ((uint32_t) af_num) << ((n & 7) * 4);
}

static inline void _gpio_write(uint16_t pin, bool val) {
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    gpio->BSRR = (1U << PINNO(pin)) << (val ? 0 : 16);
}

static inline bool _gpio_read(uint16_t pin) {
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    return 1 & (gpio->IDR >> PINNO(pin));
}

static inline void _gpio_init(gpio_config *config) {
    gpio_set_mode(config->pin, config->mode);
    gpio_set_output_type(config->pin, config->output_type);
    gpio_set_speed(config->pin, config->output_speed);
    gpio_set_output_type(config->pin, config->output_type);
    if (config->mode == GPIO_MODE_AF) {
        gpio_set_af(config->pin, config->af);
    }
}

int gpio_init(gpio_config *config);
int gpio_write(uint16_t pin, bool val);
int gpio_read(uint16_t pin);

#endif