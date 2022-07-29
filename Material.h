#ifndef MATERIAL_H
#define MATERIAL_H

#include "Rmath.h"
struct hit_record;

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
    double fuzz;
public:
    Metal(const color &a, double f): albedo(a), fuzz(f < 1?f:1){}

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override{
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_unit_vector());
        attenuation = albedo;//镜面反射没有吸收
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};

//有折射的材质
class Dielectric : public Material{
private:
    double ir;
    static double reflectance(double cosine, double ref_idx){
        //10.4提到的
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
public:
    Dielectric(double index_of_refraction): ir(index_of_refraction){}

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override{
        attenuation = color(1.0,1.0,1.0);
        double refraction_ratio = rec.reverse_normal ? (1.0/ir) : ir;

        vec3 unit_direction = unit_vector(r_in.direction());

        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool need2reflect = refraction_ratio * sin_theta > 1.0;//当折射率和角度theta乘大于一时，就不应该是折射，而是反射了
        vec3 dir;
        if(need2reflect || reflectance(cos_theta, refraction_ratio) > random_double()){
            dir = reflect(unit_direction, rec.normal);
        }else dir = refract(unit_direction, rec.normal, refraction_ratio);
        
        scattered = ray(rec.p, dir);
        return true;
    }
};
#endif