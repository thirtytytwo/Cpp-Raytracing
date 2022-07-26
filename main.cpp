#include "Rmath.h"
#include "Color.h"
#include "HittableList.h"
#include "Sphere.h"

#include <iostream>

color ray_color(const ray &r, const Hittable &world){
    hit_record rec;
    if(world.hit(r,0,infinity,rec)){
        return 0.5 * (rec.normal + color(1,1,1));
    }
    vec3 unit_dir = unit_vector(r.direction());
    auto t = 0.5 * (unit_dir.y() + 1.0);
    return (1.0 - t) * color(1,1,1) + t *color(0.5, 0.7, 1.0);
}

int main(int argc, const char * argv[]){
    //Image
    const double aspect_ratio = 16.0 / 9.0;
    const int width = 400;
    const int height = static_cast<int>(width / aspect_ratio);
    //WorldObject
    Hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
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
            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }
}