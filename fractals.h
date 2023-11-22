#ifndef _FRACTALS_H_
#define _FRACTALS_H_
#include "hal/font.h"

void mandlebrot(struct display_buffer *b, uint16_t w, uint16_t h);
void mandlebrot_opt(struct display_buffer *b, uint16_t w, uint16_t h);
void julia(struct display_buffer *b, uint16_t w, uint16_t h, float cx, float cy);



static rgb_t gradient[] = {
    (rgb_t){0, 7, 10},
    (rgb_t){32, 107, 203},
    (rgb_t){237, 255, 255},
    (rgb_t){255, 170, 0},
    (rgb_t){0, 2, 0},
};

// Position = 0.0     Color = (  0,   7, 100)
// Position = 0.16    Color = ( 32, 107, 203)
// Position = 0.42    Color = (237, 255, 255)
// Position = 0.6425  Color = (255, 170,   0)
// Position = 0.8575  Color = (  0,   2,   0)
static inline uint16_t color_pixel(int i, int imax) {
    (void)_digit_arr;
    float p = (float)i / (float)imax;
    rgb_t c;
    rgb_t *c1, *c2;
    float lerp_val = 0.0f;
    if (p < 0.16f) {
        c1 = &gradient[0];
        c2 = &gradient[1];
        lerp_val = (p + (0.16f - 0.0f)/2) / (0.16f - 0.0f);
    }
    else if (p < 0.42f) {
        c1 = &gradient[1];
        c2 = &gradient[2];
        lerp_val = (p + (0.42f - 0.16f)/2) / (0.42f - 0.16f);
    }
    else if (p < 0.6425f) {
        c1 = &gradient[2];
        c2 = &gradient[3];
        lerp_val = (p + (0.6425f- 0.42f)/2) / (0.6425f - 0.42f);
    }
    else if (p < 0.8575f) {
        c1 = &gradient[3];
        c2 = &gradient[4];
        lerp_val = (p + (0.8575f - 0.6425f)/2) / (0.8575f - 0.6425f);
    }
    else {
        c1 = &gradient[4];
        c2 = &gradient[0];
        lerp_val = (p + (1.0f - 0.8575f)/2) / (1.0f - 0.8575f);
    }

    lerp_rbg(&c, c1, c2, lerp_val);

    return RGB565(c.r, c.g, c.b);
}


#endif