#include "hal/gpio.h"
#include "hal/usart.h"
#include "hal/hal_common.h"
#include "hal/pwm.h"
#include "hal/nucleo_f303k8_pins.h"
#include "hal/spi.h"
#include "hal/st7789v.h"
#include "hal/i2c.h"
#include "hal/font.h"
#include "fractals.h"
#include "VL53L4CD_api.h"
#include "hal/button.h"
#include "utils.h"
#include <stdio.h>
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
    { 0xD0, 0x04, 0x0D, 0x11, 0x13,
      0x2B, 0x3F, 0x54, 0x4C, 0x18,
      0x0D, 0x0B, 0x1F, 0x23
    },
    { 0xD0, 0x04, 0x0C, 0x11, 0x13,
      0x2C, 0x3F, 0x44, 0x51, 0x2F,
      0x1F, 0x1F, 0x20, 0x23
    },
    { 0x00, 0xF0 },
    { 0xCD, 0x08, 0x14 },
};

int init_tof(void)
{
    _gpio_write(A0, 0);
    delay_ms(1);
    _gpio_write(A0, 1);
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

#define WIDTH 24
#define HEIGHT 24


uint16_t buff[WIDTH * HEIGHT];

struct display_buffer db = {
    (uint16_t *)buff,
    0,
    0,
    WIDTH,
    HEIGHT
};

uint8_t zoom_mode = 0;
uint8_t zoom_val = 0;
// SEND TWO BYTES AT A TIME

void move_left_cb(void) {
    move(MOVE_LEFT);
    mandlebrot_zoom(&db, cfg.width, cfg.height);
}


void move_right_cb(void) {
    move(MOVE_RIGHT);
    mandlebrot_zoom(&db, cfg.width, cfg.height);
}

void move_up_cb(void) {
    move(MOVE_UP);
    mandlebrot_zoom(&db, cfg.width, cfg.height);
}

void move_down_cb(void) {
    move(MOVE_DOWN);
    mandlebrot_zoom(&db, cfg.width, cfg.height);
}

void zoom_cb(void) {
    if (zoom_mode) {
        zoom(ZOOM_VAL, zoom_val);
        mandlebrot_zoom(&db, cfg.width, cfg.height);
    }
    else {
        for (int i = WIDTH*HEIGHT-1; i >= 0; i--) {
            db.buffer[i] = WHITE;
        }
        set_background(&db);
        db.x = 108;
        db.y = 108;
        draw_number(&db, BLACK, 0);
    }
    zoom_mode = !zoom_mode;
}

char str[50];

int main(void) {
    (void)_alpha_arr;
    uart_init(UART_DEBUG, 115200);
    
    uint16_t pwm = PIN('B', 0);
    init_pwm(pwm, 100, 90);
    set_duty_cycle(pwm, 10);
    init_spi(MISO, MOSI, SCK);
    init_st7789v(&cfg);

    for (int i = WIDTH*HEIGHT-1; i >= 0; i--) {
            db.buffer[i] = WHITE;
    }
    set_background(&db);
    db.x = 0;
    db.y = 0;

    
    _i2c_init(PIN('B', 6), PIN('B', 7));
    init_tof();
    
    button left_btn = {
        D2,
        ACTIVE_LOW,
        1,
        0,
        0,
        move_left_cb
    };
    init_btn(&left_btn);

    button right_btn = {
        D7,
        ACTIVE_LOW,
        1,
        0,
        0,
        move_right_cb
    };
    init_btn(&right_btn);

    button up_btn = {
        D8,
        ACTIVE_LOW,
        1,
        0,
        0,
        move_up_cb
    };
    init_btn(&up_btn);

    button down_btn = {
        D9,
        ACTIVE_LOW,
        1,
        0,
        0,
        move_down_cb
    };
    init_btn(&down_btn);
    
    button zoom_btn = {
        A1,
        ACTIVE_LOW,
        1,
        0,
        0,
        zoom_cb
    };
    init_btn(&zoom_btn);
    
    for (int i = WIDTH*HEIGHT-1; i >= 0; i--) {
            db.buffer[i] = BLACK;
    }
    set_background(&db);
    db.x = 0;
    db.y = 0;
    
    mandlebrot_zoom(&db, cfg.width, cfg.height);

    Dev_t 					dev = 0x52;
	uint8_t 				status, isReady;
	VL53L4CD_ResultsData_t 		results;
    (void)status;

    for (;;) {
        update_btn(&zoom_btn);
        if (zoom_mode) {
            uint8_t index = 10;
            uint32_t v = 0;
            while (index-- && zoom_mode) {
                VL53L4CD_StartRanging(dev);
                isReady = 0;
                do {
                    status = VL53L4CD_CheckForDataReady(dev, &isReady);
                    delay_ms(5);
                    update_btn(&zoom_btn);
                } while (!isReady);

                VL53L4CD_ClearInterrupt(dev);
                VL53L4CD_GetResult(dev, &results);
                VL53L4CD_StopRanging(dev);
                v += (uint32_t)results.distance_mm;
                delay_ms(1);
                update_btn(&zoom_btn);
            }
            if (!zoom_mode) continue;
            v /= 10;
            if (v < 15) v = 15;
            if (v > 300) v = 300;
            printf("%ld\r\n", v);
            float temp = map((float)v, 15.0f, 300.0f, 20.0f, 0.0f);
            zoom_val = (uint8_t)temp;  
            draw_number(&db, BLACK, (uint32_t)zoom_val);
        }
        else {
            update_btn(&left_btn);
            update_btn(&right_btn);
            update_btn(&up_btn);
            update_btn(&down_btn);
        }
    }
    
    return 0;
}
