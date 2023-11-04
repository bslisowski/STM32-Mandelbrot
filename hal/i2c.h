#ifndef _I2C_HAL_H_
#define _I2C_HAL_H_

#include "hal_common.h"
#include "gpio.h"


static inline void i2c_read(uint8_t addr, uint8_t *data, uint8_t size) {
    I2C1->CR2 |= (addr << I2C_CR2_SADD_Pos);
    I2C1->CR2 |= I2C_CR2_RD_WRN;
    I2C1->CR2 &= ~(0xFFu << I2C_CR2_NBYTES_Pos);
    I2C1->CR2 |= (size << I2C_CR2_NBYTES_Pos);
    I2C1->CR2 |= I2C_CR2_START;
    while (size--) {
        while ((I2C1->ISR & I2C_ISR_RXNE) == 0) spin(1);
        *data++ = (uint8_t)I2C1->RXDR;
    }
    while((I2C1->ISR & I2C_ISR_TC) == 0) spin(1);
}
        
static inline void i2c_write(uint8_t addr, uint8_t *data, uint8_t size) {
    I2C1->CR2 |= (addr << I2C_CR2_SADD_Pos);
    I2C1->CR2 &= ~I2C_CR2_RD_WRN;
    I2C1->CR2 &= ~(0xFFu << I2C_CR2_NBYTES_Pos);
    I2C1->CR2 |= (size << I2C_CR2_NBYTES_Pos);
    // AUTOEND ?
    I2C1->CR2 |= I2C_CR2_START;
    while(size--) {
        while ((I2C1->ISR & I2C_ISR_TXIS) == 0) spin(1);
        I2C1->TXDR = *data++;
    }
    while((I2C1->ISR & I2C_ISR_TC) == 0) spin(1);
}



static inline void _i2c_init(uint16_t scl, uint16_t sda) {
    gpio_config config = {
        scl, GPIO_MODE_AF, GPIO_OUTPUT_OD, GPIO_SPEED_HIGH, 0, AF4
    };
    _gpio_init(&config);
    config.pin = sda;
    _gpio_init(&config);

    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    
    // I2C_TIMINGR -> SCL       master clock config
        // t_SCL = t_SYNC1 + t_SYNC2 + ( ( (SCLH+1) + (SCLL+1) ) * (PRESC+1) * t_I2CCLK
        // 100kHz  
            // PRESC = 1
            // SCLL = 0x13
            // SCLH = 0xF
            // SDADEL = 0x02
            // SCLDEL = 0x04
    I2C1->TIMINGR |= (1U << I2C_TIMINGR_PRESC_Pos);
    I2C1->TIMINGR |= (0x13 << I2C_TIMINGR_SCLL_Pos);
    I2C1->TIMINGR |= (0x0F << I2C_TIMINGR_SCLH_Pos);
    I2C1->TIMINGR |= (0x02 << I2C_TIMINGR_SDADEL_Pos);
    I2C1->TIMINGR |= (0x04 << I2C_TIMINGR_SCLDEL_Pos);
    I2C1->CR1 |= I2C_CR1_PE;
}


#endif