#ifndef _USART_HAL_H_
#define _USART_HAL_H_

#include "hal_common.h"
#include "gpio.h"
#include "stdlib.h"

#ifndef UART_DEBUG
#define UART_DEBUG USART2
#endif

static inline int uart_read_ready(USART_TypeDef *uart) {
    return uart->ISR & USART_ISR_RXNE;
}

static inline uint8_t uart_read_byte(USART_TypeDef *uart) {
    return (uint8_t) (uart->RDR & 255);
}

static inline void uart_write_byte(USART_TypeDef *uart, uint8_t byte) {
    uart->TDR = byte;
    while ((uart->ISR & USART_ISR_TXE) == 0) spin(1);
}

static inline void uart_write_buf(USART_TypeDef *uart, char *buf, size_t len) {
    while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}

uint16_t uart_init(USART_TypeDef *uart, unsigned long baud);

#endif