#ifndef HITTABLE_H
#define HITTABLE_H

#include "Rmath.h"

class Material;

struct hit_record {
    point3 p;
    vec3 normal;
    double t;//时间t
    bool reverse_normal = false;//因为我们需要法线方向和光线方向相对，才能算出准确的反射，所以在需要的时候我们要反转法线
    shared_ptr<Material> mat_ptr;
    inline void set_face_normal(const ray &r, const vec3 &_normal){
        reverse_normal = dot(r.direction(), _normal) < 0;
        normal = reverse_normal ? _normal : -_normal;
    }
};

class Hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif
