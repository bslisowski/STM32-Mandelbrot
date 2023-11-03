#include "hal/gpio.h"
#include "hal/usart.h"
#include "hal/hal_common.h"
#include "hal/pwm.h"
#include "hal/nucleo_f303k8_pins.h"
#include "hal/spi.h"
#include "hal/st7789v.h"
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

// struct st7789v_config {
//     SPI_TypeDef spi;
//     uint16_t cs, dc, rst;
//     uint16_t width, height;
//     uint8_t x_offset, y_offset;
//     uint8_t vcom, gctrl, vrhs, vdvs, mdac, gamma, colmod, lcm;
//     uint8_t porch[5]; 
//     uint8_t cmd2en[4];
//     uint8_t pwctrl[2];
//     uint8_t pvgam[14];
//     uint8_t nvgam[14];
//     uint8_t ram[2];
//     uint8_t rgb[3]; 
// };

    // uint8_t porch[] = { 0x0c, 0x0c, 0x00, 0x33, 0x33 }; 
    // uint8_t cmd2en[4] = { 0x5a, 0x69, 0x02, 0x01};
    // uint8_t pwctrl[2] = { 0xa4, 0xa1};
    // uint8_t pvgam[14] = {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23};
    // uint8_t nvgam[14] ={0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23};
    // uint8_t ram[2] = {0x00, 0xF0};
    // uint8_t rgb[3] = { 0xCD, 0x08, 0x14};

#define RED     0b1111100000000000 // 0x07E0u 
#define GREEN   0b0000011111100000 // 0x001Fu
#define BLUE    0b0000000000011111 // 0xF800u
#define BLACK   0x0000
#define WHITE   0xFFFF

struct st7789v_config cfg = {
    SPI1,
    A3,
    D3,
    D6,
    240,
    240,
    0,
    0,
    0x19,
    0x35,
    0x12,
    0x20,
    0x00,
    0x01,
    0x55,
    0x2C,
    { 0x0c, 0x0c, 0x00, 0x33, 0x33 },
    { 0x5a, 0x69, 0x02, 0x01},
	{ 0xa4, 0xa1},
    {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23},
    {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23},
    {0x00, 0xF0},
    { 0xCD, 0x08, 0x14},
};

uint16_t buff[4608];
uint16_t col[] = {WHITE, RED, GREEN, BLUE, BLACK};

int main(void) {

    uart_init(UART_DEBUG, 115200);
    uint16_t pwm = PIN('B', 6);
    init_pwm(pwm, 100, 90);
    set_duty_cycle(pwm, 80);

    init_spi(MISO_ALT, MOSI_ALT, SCK_ALT);
    // int32_t duty = 0;
    // int8_t updown = 1;
    //uint32_t timer = 0, period = 1000;

    // (void) duty, (void) updown, (void) timer, (void) period;
    //delay_ms(10);
    printf("Init disp\r\n");
    init_st7789v(&cfg);
    struct display_buffer db = {
        (uint16_t *)buff,
        0,
        0,
        24,
        24
    };
    uint8_t col_index = 0;
    (void)col_index;
    for (int i = 4607; i >= 0; i--) {
        buff[i] = WHITE;
    }
    set_background(&db);

    db.x = (240 / 2) - 12; 
    db.y = (240 / 2) - 12; 
    
    // db.x += 24;
    draw_number(&db, RED, 1234567890);
    printf("disp ready\r\n");
    //int num_count = 10;
    //int num_index = 0;
    for (;;) {
        spin(1);
        // draw_digit(&db, RED, (uint8_t)num_index);
        // num_index = (num_index + 1) % num_count;
        // delay_ms(3000);
        // if (timer_expired(&timer, period, s_ticks)) {
        //     // duty += updown;
        //     // if (duty <= 0) {
        //     //     updown = 1;
        //     // }
        //     // else if (duty >= 100) {
        //     //     updown = -1;
        //     // }
        //     // set_duty_cycle(pwm, (uint8_t)duty);
        //     // spi_write_buf((uint16_t *)"Hello", 5);
        // }
        
        // set_background(&db);
        // col_index = (uint8_t)((col_index + 1) % 5);
        // delay_ms(3000);
        // fill(col[col_index]);
        // col_index = (uint8_t)((col_index + 1) % 5);
        // delay_ms(3000);
    }
    return 0;
}