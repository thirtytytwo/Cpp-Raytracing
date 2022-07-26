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

inline double ramdom_double(){
    return rand()/ (RAND_MAX + 1.0);
}

inline double ramdom_double(int min, int max){
    return min + (max - min) * ramdom_double();
}

#include "Ray.h"
#include "Vec3.h"

#endif