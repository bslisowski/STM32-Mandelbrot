#ifndef _FRACTALS_H_
#define _FRACTALS_H_
#include "hal/font.h"

#define MOVE_UP		0
#define MOVE_DOWN	1
#define MOVE_LEFT	2
#define MOVE_RIGHT	3
#define ZOOM_OUT 	0
#define ZOOM_IN		1

void move(uint8_t dir);
void zoom(uint8_t dir);
void mandlebrot_zoom(struct display_buffer *b, uint16_t w, uint16_t h);
void mandlebrot(struct display_buffer *b, uint16_t w, uint16_t h);
void julia(struct display_buffer *b, uint16_t w, uint16_t h, float cx, float cy);

// 20

static rgb_t gradient[] = {
    (rgb_t){0, 7, 10},
    (rgb_t){32, 107, 203},
    (rgb_t){237, 255, 255},
    (rgb_t){255, 170, 0},
    (rgb_t){0, 2, 0},
};

static rgb_t gradient1[] = {
	(rgb_t){0, 7, 100},
	(rgb_t){2, 12, 105},
	(rgb_t){3, 17, 111},
	(rgb_t){4, 22, 116},
	(rgb_t){6, 27, 122},
	(rgb_t){7, 32, 128},
	(rgb_t){9, 37, 134},
	(rgb_t){10, 42, 140},
	(rgb_t){11, 47, 146},
	(rgb_t){13, 52, 152},
	(rgb_t){14, 58, 158},
	(rgb_t){15, 63, 164},
	(rgb_t){17, 68, 169},
	(rgb_t){18, 73, 175},
	(rgb_t){20, 78, 180},
	(rgb_t){22, 83, 185},
	(rgb_t){23, 88, 189},
	(rgb_t){25, 92, 193},
	(rgb_t){27, 97, 197},
	(rgb_t){30, 102, 200},
	(rgb_t){32, 107, 203},
	(rgb_t){37, 115, 207},
	(rgb_t){44, 124, 211},
	(rgb_t){52, 133, 215},
	(rgb_t){62, 142, 219},
	(rgb_t){73, 152, 223},
	(rgb_t){86, 162, 226},
	(rgb_t){98, 172, 230},
	(rgb_t){112, 181, 233},
	(rgb_t){126, 191, 236},
	(rgb_t){140, 200, 239},
	(rgb_t){154, 209, 242},
	(rgb_t){167, 218, 245},
	(rgb_t){180, 226, 247},
	(rgb_t){192, 233, 249},
	(rgb_t){203, 239, 251},
	(rgb_t){213, 245, 252},
	(rgb_t){222, 249, 253},
	(rgb_t){229, 252, 254},
	(rgb_t){234, 254, 255},
	(rgb_t){237, 255, 255},
	(rgb_t){239, 255, 253},
	(rgb_t){240, 254, 248},
	(rgb_t){242, 252, 240},
	(rgb_t){243, 250, 228},
	(rgb_t){244, 247, 215},
	(rgb_t){246, 244, 200},
	(rgb_t){247, 240, 183},
	(rgb_t){248, 236, 165},
	(rgb_t){249, 232, 147},
	(rgb_t){250, 227, 128},
	(rgb_t){251, 222, 108},
	(rgb_t){252, 216, 90},
	(rgb_t){252, 211, 72},
	(rgb_t){253, 205, 55},
	(rgb_t){254, 199, 40},
	(rgb_t){254, 193, 27},
	(rgb_t){255, 188, 15},
	(rgb_t){255, 182, 7},
	(rgb_t){255, 176, 2},
	(rgb_t){255, 170, 0},
	(rgb_t){253, 164, 0},
	(rgb_t){248, 156, 0},
	(rgb_t){240, 148, 0},
	(rgb_t){228, 138, 0},
	(rgb_t){215, 128, 0},
	(rgb_t){200, 117, 0},
	(rgb_t){183, 106, 0},
	(rgb_t){165, 95, 0},
	(rgb_t){147, 84, 0},
	(rgb_t){128, 72, 0},
	(rgb_t){108, 61, 0},
	(rgb_t){90, 51, 0},
	(rgb_t){72, 41, 0},
	(rgb_t){55, 31, 0},
	(rgb_t){40, 23, 0},
	(rgb_t){27, 16, 0},
	(rgb_t){15, 10, 0},
	(rgb_t){7, 6, 0},
	(rgb_t){2, 3, 0},
	(rgb_t){0, 2, 0},
	(rgb_t){0, 2, 0},
	(rgb_t){0, 2, 2},
	(rgb_t){0, 2, 4},
	(rgb_t){0, 2, 7},
	(rgb_t){0, 3, 11},
	(rgb_t){0, 3, 15},
	(rgb_t){0, 3, 20},
	(rgb_t){0, 3, 26},
	(rgb_t){0, 4, 31},
	(rgb_t){0, 4, 38},
	(rgb_t){0, 4, 44},
	(rgb_t){0, 5, 50},
	(rgb_t){0, 5, 57},
	(rgb_t){0, 5, 64},
	(rgb_t){0, 6, 70},
	(rgb_t){0, 6, 77},
	(rgb_t){0, 6, 83},
	(rgb_t){0, 6, 89},
	(rgb_t){0, 7, 95},
};

// Position = 0.0     Color = (  0,   7, 100)
// Position = 0.16    Color = ( 32, 107, 203)
// Position = 0.42    Color = (237, 255, 255)
// Position = 0.6425  Color = (255, 170,   0)
// Position = 0.8575  Color = (  0,   2,   0)

// 0.0000;0.0000
// 0.0160;3.0493
// 0.0320;5.8641
// 0.0480;8.5449
// 0.0640;11.1923
// 0.0800;13.9066
// 0.0960;16.7884
// 0.1120;19.9381
// 0.1280;23.4563
// 0.1440;27.4435
// 0.1600;32.0000
// 0.1860;43.8201
// 0.2120;62.2638
// 0.2380;85.5705
// 0.2640;111.9798
// 0.2900;139.7312
// 0.3160;167.0640
// 0.3420;192.2179
// 0.3680;213.4321
// 0.3940;228.9464
// 0.4200;237.0000
// 0.4422;240.0940
// 0.4645;242.9649
// 0.4868;245.5884
// 0.5090;247.9405
// 0.5313;249.9970
// 0.5535;251.7337
// 0.5757;253.1265
// 0.5980;254.1512
// 0.6202;254.7838
// 0.6425;255.0000
// 0.6640;247.8600
// 0.6855;228.4800
// 0.7070;199.9200
// 0.7285;165.2400
// 0.7500;127.5000
// 0.7715;89.7600
// 0.7930;55.0800
// 0.8145;26.5200
// 0.8360;7.1400
// 0.8575;0.0000
// 0.8718;0.0000
// 0.8860;0.0000
// 0.9002;0.0000
// 0.9145;0.0000
// 0.9287;0.0000
// 0.9430;0.0000
// 0.9573;0.0000
// 0.9715;0.0000
// 0.9858;0.0000
// 1.0000;0.0000

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


static inline uint16_t color_pixel_new(int i, int imax) {
    float p = (float)i / (float)imax;
    int index = (int)(100.0f * p);
    return RGB565(gradient1[index].r, gradient1[index].g, gradient1[index].b);
}


#endif