#ifndef _UTILS_H_
#define _UTILS_H_

#define ABS(x) (((x) < 0) ? (x) * -1.0f : (x)) 

static inline float map(float d, float fmin, float fmax, float tmin, float tmax) {
    float p = (d + ((fmax - fmin) / 2)) / (fmax - fmin);
    return tmin + (p * (tmax - tmin));
}




#endif