#include "hal/gpio.h"
#include "hal/usart.h"
#include "hal/hal_common.h"
#include "hal/pwm.h"
#include "hal/nucleo_f303k8_pins.h"
#include "hal/spi.h"
#include "hal/st7789v.h"
#include "hal/i2c.h"
#include "VL53L4CD_api.h"
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#define RED     0b1111100000000000 // 0x07E0u 
#define GREEN   0b0000011111100000 // 0x001Fu
#define BLUE    0b0000000000011111 // 0xF800u
#define BLACK   0x0000
#define WHITE   0xFFFF

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

int init_tof(void)
{

	/*********************************/
	/*   VL53L4CD ranging variables  */
	/********************* ************/
    printf("INIT TOF\r\n");
	Dev_t 					dev = 0x52;
	uint8_t 				status;
	status = VL53L4CD_SensorInit(dev);
	if(status)
	{
		printf("VL53L4CD ULD Loading failed\r\n");
		return status;
	}

	printf("VL53L4CD ULD ready !\r\n");

    // status = VL53L4CD_SetRangeTiming(dev, 200, 0);
	// if(status)
	// {
	// 	printf("VL53L4CD_SetRangeTiming failed with status %u\n", status);
	// 	return status;
	// }

	return status;
}




int main(void) {
    uart_init(UART_DEBUG, 115200);
    uint16_t pwm = PIN('B', 0);
    init_pwm(pwm, 100, 90);
    set_duty_cycle(pwm, 80);

    gpio_set_mode(PIN('A', 5), GPIO_MODE_INPUT);
    gpio_set_mode(PIN('A', 6), GPIO_MODE_INPUT);
    init_spi(MISO, MOSI, SCK);

    printf("Init disp\r\n");
    init_st7789v(&cfg);
    struct display_buffer db = {
        (uint16_t *)buff,
        0,
        0,
        24,
        24
    };

    for (int i = 4607; i >= 0; i--) {
        db.buffer[i] = 0xFFFF;
    }
    set_background(&db);
    db.y = 108;
    draw_number(&db, RED, 0);
    _i2c_init(PIN('B', 6), PIN('B', 7));
    draw_number(&db, RED, 1);
    // delay_ms(1);
    draw_number(&db, RED, 2);
    init_tof();
    printf("TOF\r\n");
    draw_number(&db, RED, 3);
    delay_ms(3000);
    Dev_t 					dev = 0x52;
	uint8_t 				status, isReady;
	VL53L4CD_ResultsData_t 		results;

    for (;;) {
        status = VL53L4CD_StartRanging(dev);
        do {
            status = VL53L4CD_CheckForDataReady(dev, &isReady);
            delay_ms(5);
        } while (!isReady);

        VL53L4CD_ClearInterrupt(dev);
        VL53L4CD_GetResult(dev, &results);
        printf("Status = %6u, Distance = %6u, Signal = %6u\r\n",
            results.range_status,
            results.distance_mm,
            results.signal_per_spad_kcps);
        status = VL53L4CD_StopRanging(dev);
        printf("End of ULD demo\r\nStatus = %d\r\n", status);
        draw_number(&db, RED, (uint32_t)results.distance_mm);
        delay_ms(1000);
    }
    return 0;
}