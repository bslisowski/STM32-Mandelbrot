#include "st7789v.h"
#include <stdio.h>

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
            x++;
        }
        y++;
    }

    return 0;
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