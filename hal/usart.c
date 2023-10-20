#include "usart.h"

bool uart_init(USART_TypeDef *uart, unsigned long baud) {
    uint16_t rx = 0, tx = 0;
    uint32_t freq = 0;
     
    if (uart == USART1) {
        freq = APB2_FREQUENCY, RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
        rx = PIN('A', 10), tx = PIN('A', 9);
    }
    else if (uart == USART2) {
        freq = APB1_FREQUENCY, RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
        rx = PIN('A', 15), tx = PIN('A', 2);
    }
    else if (uart == USART3) {
        freq = APB1_FREQUENCY, RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
        rx = PIN('A', 15), tx = PIN('A', 2);
    }
    else {
        return false;
    }

    gpio_config pin_config = { 
        rx,  
        GPIO_MODE_AF, 
        0,
        0,
        GPIO_PULL_NONE,
        AF7
    };
    _gpio_init(&pin_config);
    pin_config =(gpio_config){ 
        tx,  
        GPIO_MODE_AF, 
        GPIO_OUTPUT_PP,
        GPIO_SPEED_MEDIUM,
        GPIO_PULL_NONE,
        AF7
    };
    _gpio_init(&pin_config);
    uart->CR1 = 0;
    uart->BRR = freq / baud;
    uart->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;
    return true;
}