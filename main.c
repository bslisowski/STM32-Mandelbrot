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

int example1(void)
{

	/*********************************/
	/*   VL53L4CD ranging variables  */
	/*********************************/

	Dev_t 					dev;
	uint8_t 				status, loop, isReady;
	uint16_t 				sensor_id = 0;
	VL53L4CD_ResultsData_t 		results;		/* results data from VL53L4CD */


	/*********************************/
	/*      Customer platform        */
	/*********************************/

	/* Default VL53L4CD I2C address */
	dev = 0x52;

	/* (Optional) Change I2C address */
	// status = VL53L4CD_SetI2CAddress(dev, 0x20);
	// dev = 0x20;


	/*********************************/
	/*   Power on sensor and init    */
	/*********************************/

	/* (Optional) Check if there is a VL53L4CD sensor connected */
	status = VL53L4CD_GetSensorId(dev, &sensor_id);
	if(status || (sensor_id != 0xEBAA))
	{
		printf("VL53L4CD not detected at requested address: %d\r\n", sensor_id);
		return status;
	}

	/* (Mandatory) Init VL53L4CD sensor */
	status = VL53L4CD_SensorInit(dev);
	if(status)
	{
		printf("VL53L4CD ULD Loading failed\r\n");
		return status;
	}

	printf("VL53L4CD ULD ready !\r\n");

	/*********************************/
	/*         Ranging loop          */
	/*********************************/

	status = VL53L4CD_StartRanging(dev);

	loop = 0;
	while(loop < 200)
	{
		/* Use polling function to know when a new measurement is ready.
		 * Another way can be to wait for HW interrupt raised on PIN 7
		 * (GPIO 1) when a new measurement is ready */
 
		status = VL53L4CD_CheckForDataReady(dev, &isReady);

		if(isReady)
		{
			/* (Mandatory) Clear HW interrupt to restart measurements */
			VL53L4CD_ClearInterrupt(dev);

			/* Read measured distance. RangeStatus = 0 means valid data */
			VL53L4CD_GetResult(dev, &results);
			printf("Status = %6u, Distance = %6u, Signal = %6u\r\n",
				 results.range_status,
				 results.distance_mm,
				 results.signal_per_spad_kcps);
			loop++;
		}

		/* Wait a few ms to avoid too high polling (function in platform
		 * file, not in API) */
		WaitMs(dev, 5);
	}

	status = VL53L4CD_StopRanging(dev);
	printf("End of ULD demo\r\n");
	return status;
}




int main(void) {

    uart_init(UART_DEBUG, 115200);
    // uint16_t pwm = PIN('A', 6);
    // init_pwm(pwm, 100, 90);
    // set_duty_cycle(pwm, 80);

    //init_spi(MISO_ALT, MOSI_ALT, SCK_ALT);

    // printf("Init disp\r\n");
    // init_st7789v(&cfg);
    // struct display_buffer db = {
    //     (uint16_t *)buff,
    //     0,
    //     0,
    //     24,
    //     24
    // };
    // uint8_t col_index = 0;
    // (void)col_index;
    // for (int i = 4607; i >= 0; i--) {
    //     buff[i] = WHITE;
    // }
    // set_background(&db);

    // db.x = (240 / 2) - 12; 
    // db.y = (240 / 2) - 12; 
    delay_ms(10);
    // draw_number(&db, RED, 1234890);
    // printf("disp ready\r\n");
    _i2c_init(PIN('B', 6), PIN('B', 7));

    example1();
    for (;;) {
        spin(1);
    }
    return 0;
}