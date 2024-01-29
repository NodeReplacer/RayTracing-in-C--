#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
    //Reckless reading about math was used to find this. It creates a sphere at point3& center with a radius of radius.
    sphere(point3 _center, double _radius, shared_ptr<material> _material)
            : center(_center), radius(_radius), mat(_material) {}

    //We used to have two doubles to find out what two points define the ray. We use the (now created) interval.h
    //class 'interval' to do so now. The points used to be called max and min and likewise ray_t has a max and min value.
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = r.origin() - center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = half_b*half_b - a*c; //A final transformed representation of our math to determine if the ray "r"
                                                        //intersects with the sphere given above.
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);
        
        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a;
            if (!ray_t.surrounds(root)) //surrounds checks if the point it acts on is between ray_t.max and ray_t.min.
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

private:
    point3 center;
    double radius;
    shared_ptr<material> mat;
};

#endif