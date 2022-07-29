#include "Rmath.h"
#include "Color.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"

#include <iostream>

color ray_color(const ray &r, const Hittable &world, int depth){
    hit_record rec;
    if(depth <= 0){
        return color(0,0,0);
    }
    if(world.hit(r,0.001,infinity,rec)){
        ray scattered;
        color attenuation;
        if(rec.mat_ptr->scatter(r, rec, attenuation, scattered)) return attenuation * ray_color(scattered, world, depth - 1);
        return color(0,0,0);
    }
    vec3 unit_dir = unit_vector(r.direction());
    auto t = 0.5 * (unit_dir.y() + 1.0);
    return (1.0 - t) * color(1,1,1) + t *color(0.5, 0.7, 1.0);
}

Hittable_list random_scene() {
    Hittable_list world;

    auto ground_material = make_shared<Lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main(int argc, const char * argv[]){
    //Image
    const double aspect_ratio = 16.0 / 9.0;
    const int width = 800;
    const int height = static_cast<int>(width / aspect_ratio);
    const int samples_per_pixel = 100;//规定一个像素中随机生成ray的次数
    const int max_depth = 50;//规定光漫反射极限次数
    //WorldObject
    Hittable_list world = random_scene();
    //Camera
    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist2focus = 10;
    auto aperture = 0.1;
    camera cam(lookfrom, lookat, vup ,20.0, aspect_ratio,aperture, dist2focus);
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
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
}