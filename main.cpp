#include "Rmath.h"
#include "Color.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"

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
    const int samples_per_pixel = 100;
    //WorldObject
    Hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
    //Camera
    camera cam;
    //Render
    std::cout<< "P3\n" << width << ' ' << height << "\n255\n";
    for (int j = height; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            color pixel_color(0,0,0);
            //抗锯齿方法，类似msaa，不过是采用在一个像素点随机位置采样给定次
            for(int s = 0; s < samples_per_pixel; ++s){
                auto u = (i + random_double()) / (width - 1);
                auto v = (j + random_double()) / (height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
}