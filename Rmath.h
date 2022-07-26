#ifndef RMATH_H
#define RMATH_H

#include<cmath>
#include<limits>
#include<memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535;

inline double degree2radians(double degree){
    return degree * pi / 180.0;
}

inline double random_double(){
    return rand()/ (RAND_MAX + 1.0);
}

inline double random_double(int min, int max){
    return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max){
    if(x < min) return min;
    if(x > max) return max;
    return x;
}

#include "Ray.h"
#include "Vec3.h"

#endif