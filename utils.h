#ifndef _UTILS_H_
#define _UTILS_H_
#include <math.h>

#define ABS(x) (((x) < 0) ? (x) * -1.0f : (x)) 

static inline float map(float d, float fmin, float fmax, float tmin, float tmax) {
    float p = (d + ((fmax - fmin) / 2)) / (fmax - fmin);
    return tmin + (p * (tmax - tmin));
}

static inline float quadratic(float c) {
  // -b +/- sqrt(b^2 - 4ac) / 2a
  float res1 = ABS((1.0f + sqrtf(1.0f - 4.0f*c)) / 2.0f);
  float res2 = ABS((1.0f - sqrtf(1.0f - 4.0f*c)) / 2.0f);
  float ret = res1 > res2 ? res1 : res2;
  
  return ret + 0.1f*ret;
}


static inline float get_radius(float c) {
  return c > 2.0f ? c : 2.0f;
}




#endif