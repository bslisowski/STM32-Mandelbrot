#ifndef _SPI_HAL_H_
#define _SPI_HAL_H_

#include "hal_common.h"
#include "gpio.h"
#include <stdlib.h>


#define MOSI        PIN('B', 5)  
#define MISO        PIN('B', 4)
#define SCK         PIN('B', 3)
#define MOSI_ALT    PIN('A', 7)  
#define MISO_ALT    PIN('A', 6)
#define SCK_ALT     PIN('A', 5)

#define IS_MOSI_PIN(p) (p == MOSI || p == MOSI_ALT)
#define IS_MISO_PIN(p) (p == MISO || p == MISO_ALT)
#define IS_SCK_PIN(p) (p == SCK || p == SCK_ALT)

// void SPI1_IRQHandler(void);

static inline void _init_spi(uint16_t miso, uint16_t mosi, uint16_t sck) {
    
    gpio_config mo = {
        mosi, GPIO_MODE_AF, GPIO_OUTPUT_PP, GPIO_SPEED_HIGH, 0, AF5
    };
    _gpio_init(&mo);
    (void)miso;
    gpio_config mi = {
        miso, GPIO_MODE_AF, 0, GPIO_SPEED_HIGH, 0, AF5
    };
    _gpio_init(&mi); 
    gpio_config sc = {
        sck, GPIO_MODE_AF, GPIO_OUTPUT_PP, GPIO_SPEED_HIGH, 0, AF5
    };
    _gpio_init(&sc);

    // NVIC->ISER[1] |= (1U << 3);

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN_Msk;

    SPI1->CR1 |= SPI_CR1_MSTR_Msk | SPI_CR1_SSM_Msk | SPI_CR1_SSI_Msk;
    SPI1->CR2 &= ~SPI_CR2_DS;
    SPI1->CR2 |= (7U << SPI_CR2_DS_Pos);
    SPI1->CR1 |= SPI_CR1_SPE;
}

static inline void spi_enable(void) {
    SPI1->CR1 |= SPI_CR1_SPE;
}

static inline void spi_disable(void) {
    SPI1->CR1 &= ~SPI_CR1_SPE;
}

static inline void cs_enable(uint16_t pin) {
    _gpio_write(pin, 0);
}

static inline void cs_disable(uint16_t pin) {
    _gpio_write(pin, 1);
}


static inline void spi_write_byte(uint8_t byte) {
    while ((SPI1->SR & SPI_SR_TXE_Msk) == 0) spin(1);
    *((volatile uint8_t *) &(SPI1->DR)) = byte;
}

static inline void spi_write_buf(uint8_t *buf, size_t len, uint16_t cs) {
    (void) cs;
    //cs_enable(cs);
    while (len-- > 0) spi_write_byte(*buf++);
    //cs_disable(cs);
    while ((SPI1->SR & SPI_SR_TXE_Msk) == 0) spin(1);
    while ((SPI1->SR & SPI_SR_BSY_Msk) == 0) spin(1);
    uint32_t temp = SPI1->DR;
    temp = SPI1->SR;
    (void)temp;
}

// spi interrupt 35


int init_spi(uint16_t miso, uint16_t mosi, uint16_t sck);

#endif