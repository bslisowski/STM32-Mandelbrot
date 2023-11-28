#include "hal/hal_common.h"
#include "hal/st7789v.h"
#include "fractals.h"
#include "utils.h"
#include <inttypes.h>
#include <math.h>

static const float _square = 3.0f;
// static const float _x_lower = -2.0f;
// static const float _x_upper =  1.0f;
// static const float _y_lower = -1.5f;
// static const float _y_upper =  1.5f; 
static float _cx = -0.5f;
static float _cy = 0.0f;
static uint16_t _zoom = 1;

#define XMIN (_cx - ((_square/_zoom)/2))
#define XMAX (_cx + ((_square/_zoom)/2))
#define YMIN (_cy - ((_square/_zoom)/2))
#define YMAX (_cy + ((_square/_zoom)/2))

static inline float get_unit() {
    float _sq = _square / _zoom;
    return _sq / 24;
}

void move(uint8_t dir) {
    float unit = get_unit();
    switch (dir) {
        case MOVE_UP:
            _cy += unit;
            break;
        case MOVE_DOWN:
            _cy -= unit;
            break;
        case MOVE_LEFT:
            _cx -= unit;
            break;
        case MOVE_RIGHT:
            _cx += unit;
            break;  
        default:
            break;
    }
}

void zoom(uint8_t dir) {
    if (dir) {
        if (_zoom != 65535) _zoom = _zoom << 1;
    }
    else {
        if (_zoom != 1) _zoom = _zoom >> 1;
    }
}



void mandlebrot_zoom(struct display_buffer *b, uint16_t w, uint16_t h) {
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
            for (int i = 0; i < b->width * b->height; i++) {
                float x0 = map((float)x_pos - x_off, -1.0f * x_off, x_off, XMIN, XMAX);
                float y0 = map((float)y_pos - y_off , -1.0f * y_off, y_off, YMIN, YMAX);
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


// -1.34007f, -0.742322f
// -0.501123f, -0.058426f

void mandlebrot(struct display_buffer *b, uint16_t w, uint16_t h) {
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
                float x0 = map((float)x_pos - x_off, -1.0f * x_off, x_off, -2.0f, 1.0f);
                float y0 = map((float)y_pos - y_off , -1.0f * y_off, y_off, -1.5f, 1.5f);
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


void julia(struct display_buffer *b, uint16_t w, uint16_t h, float cx, float cy) {
    //static uint8_t prnt = 0;
    printf("cx = %f\tcy = %f\r\n", (double)cx, (double)cy);
    float x_off = w / 2;
    float y_off = h / 2;
    uint16_t x = 0;
    uint16_t y = 0;
    float r = get_radius(sqrtf(cx*cx + cy*cy));
    float w_scale = quadratic(-1.0f * sqrtf(cx*cx + cy*cy));
    float h_scale = w_scale;

    while (y + b->height <= h) {
        x = 0;
        b->y = y;
        while (x + b->width <= w) {
            b->x = x;
            uint16_t x_pos = x;
            uint16_t y_pos = y;
            printf("STARTING (%d, %d)\r\n", x_pos, y_pos);
            for (int i = 0; i < b->width * b->height; i++) {
                float x0 = map((float)x_pos - x_off, -1.0f * x_off, x_off, -w_scale, w_scale);
                float y0 = map((float)y_pos - y_off , -1.0f * y_off, y_off, -h_scale, h_scale);
                int iter = 0;
                int iter_max = 1000;

                while (x0*x0 + y0*y0 <= r*r && iter < iter_max) {
                    float x_temp = x0*x0 - y0*y0;
                    y0 = 2 * x0 * y0 + cy;
                    x0 = x_temp + cx;
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
            printf("Hello \r\n");
            x += b->width;
        }
        y += b->height;
    }
    //prnt++;
}
