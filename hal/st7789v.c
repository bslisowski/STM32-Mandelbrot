#include "st7789v.h"
#include <stdio.h>
#include "font.h"
#include <math.h>

int draw_digit(struct display_buffer *b, uint16_t color, uint8_t digit) {
    if (digit > 9) {
        return -1;
    }

    const uint8_t *d = _digit_arr[digit];
    for (int i = 0; i < 72; i++) {
        for (int j = 0; j < 8; j++) {
            b->buffer[i*8 + j] = d[i] & (1 << (7 - j) ) ? color : 0xFFFF;
        }
    }

    _draw(b);
    return 0;
}


int draw_number(struct display_buffer *b, uint16_t color, uint32_t num) {
    uint32_t temp = num;
    uint8_t num_digits = 1;
    while ((temp /= 10)) num_digits++;
    b->x = (uint16_t)((display->width / 2) + (num_digits * b->width / 2));
    while (num_digits--) {
        b->x -= 24;
        draw_digit(b, color, (uint8_t)(num%10));
        num /= 10;
    }
    return 0;
}

int draw(struct display_buffer *b) {
    _draw(b);
    return 0;
}


int set_background(struct display_buffer *b) {
    uint16_t x = 0;
    uint16_t y = 0;
    while (y + b->height <= display->width) {
        x = 0;
        b->y = y;
        while (x + b->width <= display->height) {
            b->x = x;
            _draw(b);
            x += b->width;
        }
        y += b->height;
    }

    return 0;
}

void fill(uint16_t color) {
    uint16_t addr[2];
    addr[0] = 0;
    addr[1] = (uint16_t)239;
    uint8_t buf[4];
    buf[0] = (uint8_t)(addr[0] >> 8);
    buf[1] = (uint8_t)(addr[0] & 0xFF);
    buf[2] = (uint8_t)(addr[1] >> 8);
    buf[3] = (uint8_t)(addr[1] & 0xFF);
    _write_st7789v(ST7789V_CASET, buf, 4);
    _write_st7789v(ST7789V_RASET, buf, 4);
    cs_enable(display->cs);
    spin(1);
    _gpio_write(display->dc, 0);
    spi_write_byte(ST7789V_RAMWR);
    while ((SPI1->SR & SPI_SR_TXE_Msk) == 0) spin(1);
    while ((SPI1->SR & SPI_SR_BSY_Msk) == 0) spin(1);
    uint32_t temp = SPI1->DR;
    temp = SPI1->SR;
    (void)temp;
    spin(1);
    _gpio_write(display->dc, 1);
    for (int i = 57600; i >= 0; i--) {
        spi_write_byte((uint8_t)color);
        spi_write_byte((uint8_t)(color >> 8));
    }
    while ((SPI1->SR & SPI_SR_TXE_Msk) == 0) spin(1);
    while ((SPI1->SR & SPI_SR_BSY_Msk) == 0) spin(1);
    temp = SPI1->DR;
    temp = SPI1->SR;
    (void)temp;
    spin(2);
    cs_disable(display->cs);
    // printf("HELLO\r\n");
}


void display_on() {
    _write_st7789v(ST7789V_DISPON, NULL, 0);
}

void display_off() {
    _write_st7789v(ST7789V_DISPOFF, NULL, 0);
}

void reset_st7789v() {
    if (display->rst) {
        _gpio_write(display->rst, 0);
        delay_ms(1);
        _gpio_write(display->rst, 1);
    } 
    else {
        _write_st7789v(ST7789V_SWRESET, NULL, 0);
        delay_ms(5);
    }
}

int init_st7789v(struct st7789v_config *config) {
    display = config;
    gpio_set_mode(display->cs, GPIO_MODE_OUTPUT);
    gpio_set_speed(display->cs, GPIO_SPEED_HIGH);
    _gpio_write(display->cs, 1);
    gpio_set_mode(display->dc, GPIO_MODE_OUTPUT);
    gpio_set_speed(display->dc, GPIO_SPEED_HIGH);
    _gpio_write(display->dc, 1);
    gpio_set_mode(display->rst, GPIO_MODE_OUTPUT);
    _gpio_write(display->rst, 1);

    reset_st7789v();
    display_off();
    // set lcd margins
    _write_st7789v(ST7789V_CMD2EN, (uint8_t *)display->cmd2en, sizeof(display->cmd2en));
    _write_st7789v(ST7789V_PORCTRL, (uint8_t *)display->porch, sizeof(display->porch));
    uint8_t temp = 0;
    _write_st7789v(ST7789V_DGMEM, &temp, 1);
    temp = 0x0F;
    _write_st7789v(ST7789V_FRCTRL2, &temp, 1);
    _write_st7789v(ST7789V_GCTRL, &display->gctrl, 1);
    _write_st7789v(ST7789V_VCOMS, &display->vcom, 1);
    // ??  
        // VDVVRHEN
        // VRH
        // VDS
    _write_st7789v(ST7789V_PWCTRL1, (uint8_t *)display->pwctrl, sizeof(display->pwctrl));
    _write_st7789v(ST7789V_MADCTL, &display->mdac, 1);
    _write_st7789v(ST7789V_COLMOD, &display->colmod, 1);
    _write_st7789v(ST7789V_LCMCTRL, &display->lcm, 1);
    _write_st7789v(ST7789V_GAMSET, &display->gamma, 1);
    _write_st7789v(ST7789V_INVON, NULL, 0);
    _write_st7789v(ST7789V_PVGAMCTRL, (uint8_t *)display->pvgam, sizeof(display->pvgam));
    _write_st7789v(ST7789V_NVGAMCTRL, (uint8_t *)display->nvgam, sizeof(display->nvgam));
    _write_st7789v(ST7789V_RAMCTRL, (uint8_t *)display->ram, sizeof(display->ram));
    _write_st7789v(ST7789V_RGBCTRL, (uint8_t *)display->rgb, sizeof(display->rgb));
    _write_st7789v(ST7789V_SLPOUT, NULL, 0);
    display_on();
    return 0;
}