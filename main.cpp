#include "Vec3.h"
#include "Color.h"
#include "Ray.h"

#include <iostream>

double hit_sphere(const point3& center, double radius, const ray& r){
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2 * dot(r.direction(), oc);
    auto c = dot(oc,oc) - radius * radius;

    auto _g = b * b - 4 * a * c;//一元二次方程求解的数量的公式
    if(_g < 0){
        return -1.0;
    }
    else{
        return (-b - sqrt(_g) ) / (2.0*a);
    }
}

color ray_color(const ray& r){
    auto t = hit_sphere(point3(0,0, -1), 0.5, r);
    if(t > 0){
        vec3 n = unit_vector(r.at(t) - point3(0,0,-1));//球心和焦点相减得到未归一化的法向量，因为是球体所以可以用这个方法
        return 0.5 * color(n.x() + 1, n.y() + 1, n.z() + 1);
    }
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0,1.0,1.0) + t * color(0.5, 0.7, 1.0);
}


int main(int argc, const char * argv[]){
    //Image
    const double aspect_ratio = 16.0 / 9.0;
    const int width = 400;
    const int height = static_cast<int>(width / aspect_ratio);
    //Camera
    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0;//摄像机和视口的距离，znear(可能是)

    point3 origin = point3(0,0,0);
    vec3 horizontal = vec3(viewport_width,0,0);
    vec3 vertical = vec3(0,viewport_height,0);
    //视口左下角坐标
    point3 bottom_left_corner = origin - horizontal/2 - vertical/2 - vec3(0,0,focal_length);

    //Render
    std::cout<< "P3\n" << width << ' ' << height << "\n255\n";
    for (int j = height; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            //获取一个像素大小
            double u = double(i) / (width - 1);
            double v = double(j) / (height - 1);
            ray r = ray(origin,bottom_left_corner + u * horizontal + v * vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
}