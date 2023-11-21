#include "hal/hal_common.h"
#include "hal/st7789v.h"
#include "fractals.h"
#include "utils.h"
#include "hal/font.h"
#include <inttypes.h>

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


void mandlebrot(struct display_buffer *b, uint16_t w, uint16_t h) {
    // -2, 0.47
    // -1.12, 1.12
    printf("************* START *************\r\n");
    int iter_max = 1000;
    float x_off = w / 2;
    float y_off = h / 2;
    uint16_t x = 0;
    uint16_t y = 0;
    while (y + b->height <= h) {
        x = 0;
        b->y = y;
        while (x + b->width <= w) {
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

void mandlebrot_opt(struct display_buffer *b, uint16_t w, uint16_t h) {
    // -2, 0.47
    // -1.12, 1.12
    printf("************* START *************\r\n");
    int iter_max = 750;
    float x_off = w / 2;
    float y_off = h / 2;
    uint16_t x = 0;
    uint16_t y = 0;
    while (y + b->height <= h) {
        x = 0;
        b->y = y;
        while (x + b->width <= w) {
            b->x = x;
            uint16_t x_pos = x;
            uint16_t y_pos = y;
    //         -0.82358f, 
    // -0.66248f,          
    // 0.072733f,
    // 0.180133f,
            for (int i = 0; i < b->width * b->height; i++) {
                float x0 = map((float)x_pos - x_off, -1.0f * x_off, x_off, -2.0f, 0.42f);
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
                // int new_iter = 0;
                // float log_zn = logf(x2 + y2) / 2.0f;
                // new_iter = (int)( logf(log_zn / logf(2.0f)) / logf(2.0f) );
                // b->buffer[i] = color_pixel(iter + 1 - new_iter, iter_max);
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