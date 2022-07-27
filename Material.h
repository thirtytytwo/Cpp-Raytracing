#ifndef MATERIAL_H
#define MATERIAL_H

#include "Rmath.h"
#include "Hittable.h"

class Material{
public:
    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)const =0;
};

//diffuse漫反射材质
class Lambertian : public Material{
private:
    color albedo;
public:
    Lambertian(const color &a) : albedo(a){}
    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)const override{
        auto scatter_dir = rec.normal + random_unit_vector();
        if(scatter_dir.nera_zero()){
            scatter_dir = rec.normal;
        }
        scattered = ray(rec.p, scatter_dir);
        attenuation = 0.8 * albedo;//漫反射有吸收
        return true;
    }
};

//镜面反射材质
class Metal : public Material{
private:
    color albedo;
public:
    Metal(const color &a): albedo(a){}

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override{
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;//镜面反射没有吸收
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};
#endif