#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Vec3.h"

class sphere : public Hittable{
private:
    point3 center;
    double radius;
public:
    sphere(){};
    sphere(point3 cen, double r): center(cen), radius(r){};

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const{
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto h_b = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;

    auto _g = h_b * h_b - a * c;//一元二次方程求解的数量的公式
    if(_g < 0) return false;
    auto sqrted = std::sqrt(_g);

    //找到在tmin和tmax之间的可行光线
    auto root = (-h_b - sqrted) / a;//求根公式
    if(root < t_min || root < t_max){
        root = (-h_b + sqrted) / a;
        if(root < t_min || root < t_max) return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);//焦点的位置
    vec3 normal = (rec.p - center) / radius;
    rec.set_face_normal(r, normal);

    return true;
}
#endif