#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "common_constants.h"

class material;

class hit_record {
public:
    point3 p;
    vec3 normal;
    shared_ptr<material> mat;
    double t;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual ~hittable() = default;
    
    //Instead of loading in two doubles to find out what two points define our ray we use the (now created) interval.h
    //class 'interval' to do so.
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif