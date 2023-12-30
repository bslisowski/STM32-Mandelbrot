#include "hal_common.h"
#include "st7789v.h"
#include "fractals.h"
#include "utils.h"
#include <inttypes.h>
#include <math.h>

#define ZOOM_LIMIT 1048576

static const float _square = 3.0f;
static float _cx = -0.5f;
static float _cy = 0.0f;
static uint32_t _zoom = 1;

#define XMIN (_cx - ((_square/(float)_zoom)/2))
#define XMAX (_cx + ((_square/(float)_zoom)/2))
#define YMIN (_cy - ((_square/(float)_zoom)/2))
#define YMAX (_cy + ((_square/(float)_zoom)/2))

static inline float get_unit() {
    (void)_alpha_arr;
    float _sq = _square / (float)_zoom;
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

// val = 0 to 20 => 2^val
void zoom(uint8_t dir, uint8_t val) {
    switch(dir) {
        case ZOOM_IN:
            if (_zoom != ZOOM_LIMIT) _zoom = _zoom << 1;
            break;
        case ZOOM_OUT:
            if (_zoom != 1) _zoom = _zoom >> 1;
            break;
        case ZOOM_VAL:
            _zoom = (1ul << val);
            break;
        default:
            break;
    }
}



void mandlebrot(struct display_buffer *b, uint16_t w, uint16_t h) {
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


// float julias[11][2] = {
//         {-0.8f,0.156f},
//         {-0.4f,0.6f},
//         {-0.7269f,0.1889f},
//         {-0.123f,0.745f},
//         {-0.75f,0.001f},
//         {-0.391f,-0.587f},
//         {-1.0f,0.1f},
//         {0.360284f,0.100376f},
//         {-0.52f,0.57f},
//         {0.295f, 0.55f},
//         {-0.624f,0.435f}
//     };

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
