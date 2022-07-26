#ifndef COLOR_H
#define COLOR_H

#include "Rmath.h"
#include <iostream>

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    // Write the translated [0,255] value of each color component.
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    auto scale = 1.0/ samples_per_pixel;

//这里对颜色信号进行伽马反校正，因为输出图片中，图片浏览器会自动进行伽马矫正，会把我们输出的颜色信息转换电信号再转回来的时候更暗
//这里采用伽马值为2，因为可以直接用平方函数就行，2.2还需要自定义平方函数
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);
    
    out << static_cast<int>(256 * clamp(r,0.0,0.999)) << ' '
        << static_cast<int>(256 * clamp(g,0.0,0.999)) << ' '
        << static_cast<int>(256 * clamp(b,0.0,0.999)) << '\n';
}

#endif