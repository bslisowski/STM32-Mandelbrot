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

// SEND TWO BYTES AT A TIME

void move_left_cb(void) {
    move(MOVE_LEFT);
    mandlebrot_zoom(&db, cfg.width, cfg.height);
}

void move_right_cb(void) {
    move(MOVE_RIGHT);
    mandlebrot_zoom(&db, cfg.width, cfg.height);
}

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
    // uint16_t buff[WIDTH * HEIGHT];
    // db.buffer = (uint16_t *)buff;
    
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

    //uint16_t colors[] = { WHITE, RED, GREEN, BLUE, BLACK };
    for (int i = WIDTH*HEIGHT-1; i >= 0; i--) {
            db.buffer[i] = BLACK;
    }
    set_background(&db);
    db.x = 0;
    db.y = 0;
    //delay_ms(5000);
    // uint32_t t0 = s_ticks;

    float julias[11][2] = {
        {-0.8f,0.156f},
        {-0.4f,0.6f},
        {-0.7269f,0.1889f},
        {-0.123f,0.745f},
        {-0.75f,0.001f},
        {-0.391f,-0.587f},
        {-1.0f,0.1f},
        {0.360284f,0.100376f},
        {-0.52f,0.57f},
        {0.295f, 0.55f},
        {-0.624f,0.435f}
    };
    (void)julias;
    

    mandlebrot_zoom(&db, cfg.width, cfg.height);
    // uint16_t btn_state = _gpio_read(D2);
    // uint16_t btn_state_last = btn_state;
    // gpio_set_mode(D2, GPIO_MODE_INPUT);
    // gpio_set_pull(D2, GPIO_PULLUP);

    // IN IN LEFT IN
    // int j = 11;
    for (;;) {
        update_btn(&left_btn);
        update_btn(&right_btn);
        // btn_state = _gpio_read(D2);
        // if (btn_state != btn_state_last) {
        //     if (btn_state) {
        //         move(MOVE_LEFT);
        //         mandlebrot_zoom(&db, cfg.width, cfg.height);
        //     }
        //     btn_state_last = btn_state;
        // }
    }

    // db.y = 108;
    // draw_number(&db, RED, 0);
    // _i2c_init(PIN('B', 6), PIN('B', 7));
    // init_tof();
    
    // Dev_t 					dev = 0x52;
	// uint8_t 				status, isReady;
	// VL53L4CD_ResultsData_t 		results;
    // (void)status;
    // for (;;) {
        
    //     VL53L4CD_StartRanging(dev);
    //     isReady = 0;
    //     do {
    //         status = VL53L4CD_CheckForDataReady(dev, &isReady);
    //         delay_ms(5);
    //     } while (!isReady);

    //     VL53L4CD_ClearInterrupt(dev);
    //     VL53L4CD_GetResult(dev, &results);
    //     // printf("Status = %6u, Distance = %6u, Signal = %6u, Status = %d\r\n",
    //     //     results.range_status,
    //     //     results.distance_mm,
    //     //     results.signal_per_spad_kcps, status);
    //     VL53L4CD_StopRanging(dev);
        
    //     draw_number(&db, (uint16_t)((RED >> 8) | (RED << 8)), (uint32_t)results.distance_mm);
    //     delay_ms(250);
    // }
    return 0;
}