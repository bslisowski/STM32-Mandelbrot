#include "hal/gpio.h"
#include "hal/usart.h"
#include "hal/hal_common.h"
#include "hal/pwm.h"
#include "hal/nucleo_f303k8_pins.h"
#include "hal/spi.h"
#include "hal/st7789v.h"
#include "hal/i2c.h"
#include "hal/font.h"
#include "VL53L4CD_api.h"
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <math.h>

struct st7789v_config cfg = {
    SPI1,
    A3,
    D10,
    D6,
    240,
    240,
    0,
    0,
    0x19,
    0x35,
    0x12,
    0x20,
    0x08,
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

int init_tof(void)
{
    _gpio_write(D9, 1);
    delay_ms(1);
	Dev_t 					dev = 0x52;
	uint8_t 				status;
	status = VL53L4CD_SensorInit(dev);
	if(status)
	{
		printf("VL53L4CD ULD Loading failed\r\n");
		return status;
	}
	return status;
}

const uint16_t w = 24;
const uint16_t h = 24;

#define ABS(x) (((x) < 0) ? (x) * -1.0f : (x)) 

static inline float map(float d, float fmin, float fmax, float tmin, float tmax) {
    float p = (d + ((fmax - fmin) / 2)) / (fmax - fmin);
    return tmin + (p * (tmax - tmin));
}


/*
    {
   -2.0f, 
   0.47f,
   -1.12f, 
   1.12f
  },
  {
    -0.82358f, 
    -0.66248f,          
    0.072733f,
    0.180133f,
  },
  {
    -0.726582279f, 
    -0.571178189f,
    0.394255111f, 
    0.510516066f
  },
  {
    -1.581986144f, 
    -1.17295355f   
    -0.143187067f, 
   0.159609956f
  },


*/


void mandlebrot(struct display_buffer *b) {
    // -2, 0.47
    // -1.12, 1.12
    printf("************* START *************\r\n");
    int iter_max = 1000;
    float x_off = cfg.width / 2;
    float y_off = cfg.height / 2;
    uint16_t x = 0;
    uint16_t y = 0;
    while (y + b->height <= cfg.height) {
        x = 0;
        b->y = y;
        while (x + b->width <= cfg.width) {
            b->x = x;
            uint16_t x_pos = x;
            uint16_t y_pos = y;
            for (int i = 0; i < b->width * b->height; i++) {
                float x0 = map((float)x_pos - x_off, -1.0f * x_off, x_off, -2.0f, 0.47f);
                float y0 = map((float)y_pos - y_off , -1.0f * y_off, y_off, -1.12f, 1.12f);
                float x1 = 0.0f;
                float y1 = 0.0f;
                int iter = 0;
                while (x1*x1 < 4.0f && iter <= iter_max) {
                    float x_temp = x1*x1 - y1*y1 + x0;
                    y1 = 2*x1*y1 + y0;
                    x1 = x_temp;
                    ++iter;
                }
                b->buffer[i] = color_pixel(iter, iter_max);
                x_pos++;
                if (x_pos >= b->x + b->width) { 
                    x_pos = b->x; 
                    y_pos++;
                }
            }
            draw(b);
            x += b->width;
        }
        y += b->height;
    }

}

void mandlebrot_opt(struct display_buffer *b) {
    // -2, 0.47
    // -1.12, 1.12
    printf("************* START *************\r\n");
    int iter_max = 750;
    float x_off = cfg.width / 2;
    float y_off = cfg.height / 2;
    uint16_t x = 0;
    uint16_t y = 0;
    while (y + b->height <= cfg.height) {
        x = 0;
        b->y = y;
        while (x + b->width <= cfg.width) {
            b->x = x;
            uint16_t x_pos = x;
            uint16_t y_pos = y;
            for (int i = 0; i < b->width * b->height; i++) {
                float x0 = map((float)x_pos - x_off, -1.0f * x_off, x_off, -2.0f, 0.47f);
                float y0 = map((float)y_pos - y_off , -1.0f * y_off, y_off, -1.12f, 1.12f);
                float x1 = 0.0f;
                float y1 = 0.0f;
                float x2 = 0.0f;
                float y2 = 0.0f;
                int iter = 0;
                while (x2 + y2 <= 4.0f && iter < iter_max) {
                    y1 = 2 * x1 * y1 + y0;
                    x1 = x2 - y2 + x0;
                    x2 = x1 * x1;
                    y2 = y1 * y1;
                    iter++; 
                }
                b->buffer[i] = color_pixel(iter, iter_max);
                x_pos++;
                if (x_pos >= b->x + b->width) { 
                    x_pos = b->x; 
                    y_pos++;
                }
            }
            draw(b);
            x += b->width;
        }
        y += b->height;
    }

}

// SEND TWO BYTES AT A TIME

int main(void) {
    uart_init(UART_DEBUG, 115200);
    uint16_t pwm = PIN('B', 0);
    init_pwm(pwm, 100, 90);
    set_duty_cycle(pwm, 10);
    //delay_ms(5000);
    //printf("hello\r\n");
    gpio_set_mode(D9, GPIO_MODE_OUTPUT);
    _gpio_write(D9, 0);

    init_spi(MISO, MOSI, SCK);

    
    init_st7789v(&cfg);
    uint16_t buff[w*h];
    struct display_buffer db = {
        (uint16_t *)buff,
        0,
        0,
        w,
        h
    };
    
    uint16_t colors[] = { WHITE, RED, GREEN, BLUE, BLACK };
    for (int i = w*h-1; i >= 0; i--) {
            db.buffer[i] = colors[1];
    }
    set_background(&db);
    db.x = 0;
    db.y = 0;

    uint32_t t0 = s_ticks;
    // mandlebrot(&db);
    // t0 = s_ticks - t0;
    // delay_ms(1000);
    // printf("%ld\r\n", t0);
    // for (int i = w*h-1; i >= 0; i--) {
    //         db.buffer[i] = colors[2];
    // }
    // set_background(&db);
    // t0 = s_ticks;
    mandlebrot_opt(&db);
    t0 = s_ticks - t0;
    delay_ms(1000);
    printf("%ld\r\n", t0);
    for (;;) spin(1);

    db.y = 108;
    draw_number(&db, RED, 0);
    _i2c_init(PIN('B', 6), PIN('B', 7));
    init_tof();
    
    Dev_t 					dev = 0x52;
	uint8_t 				status, isReady;
	VL53L4CD_ResultsData_t 		results;
    (void)status;
    for (;;) {
        
        VL53L4CD_StartRanging(dev);
        isReady = 0;
        do {
            status = VL53L4CD_CheckForDataReady(dev, &isReady);
            delay_ms(5);
        } while (!isReady);

        VL53L4CD_ClearInterrupt(dev);
        VL53L4CD_GetResult(dev, &results);
        // printf("Status = %6u, Distance = %6u, Signal = %6u, Status = %d\r\n",
        //     results.range_status,
        //     results.distance_mm,
        //     results.signal_per_spad_kcps, status);
        VL53L4CD_StopRanging(dev);
        
        draw_number(&db, (uint16_t)((RED >> 8) | (RED << 8)), (uint32_t)results.distance_mm);
        delay_ms(250);
    }
    return 0;
}