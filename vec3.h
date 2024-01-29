#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

//3D vector for all rendering purposes.

using std::sqrt;

// Definition of a 3D vector class
class vec3 {
public:
    // Components of the vector
    double e[3];

    // Constructors
    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    // Accessors for individual components
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    // Unary negation operator
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

    // Subscript operator for access to components
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    // Compound addition operator
    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    // Compound multiplication by scalar operator
    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    // Compound division by scalar operator
    vec3& operator/=(double t) {
        return *this *= 1 / t;
    }

    // Calculate the length of the vector
    double length() const {
        return sqrt(length_squared());
    }

    // Calculate the squared length of the vector (avoiding square root)
    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
    
    //Just to check in case we wind up with essentially zero. Dividing by 0 will kill us so we do need to make sure
    //we can see that coming.
    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }
    
    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max) {
        return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }
    
};

// point3 and vec3 are the same thing. But helpful for clarity in writing.
using point3 = vec3;

// Vector Utility Functions

// Output stream operator for vec3
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// Addition operator for vec3
inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// Subtraction operator for vec3
inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// Component-wise multiplication operator for vec3
inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// Scalar multiplication operators for vec3
inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

// Scalar division operator for vec3
inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

// Dot product of two vectors
inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

// Cross product of two vectors
inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// Returns the unit vector of a vector
inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

//Find a random vector that only gets results on the surface of a hemisphere.
//Pick a random point in the unit cube, where x, y, and z all range from −1 to +1, and reject this point if it is outside the unit sphere.
//Why do we need this? 
// Well. A random point inside a sphere and the point at the center of the sphere, when connected to make a line,
//will point out and away from the center of the sphere.
// We aren't looking for a point inside a 'physical' sphere that will be rendered, we are looking for a random point for the ray to
//bounce away from the point that it made contact with any surface. 
// 
// Yes the random point right now may point into the object the ray is supposed to bounce off of.
//But we can just compare it to the surface normal to prevent that.
inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1,1);
        if (p.length_squared() < 1)
            return p;
    }
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

//What's all this? Disk? What disk?. It's for camera depth of field.
//A camera has a big lens and that defocuses everything, however with the right lens the
//light rays will hit the lens and be bent directly into a single point in the part of the camera that senses the light rays.
//Those light rays will be in focus. But there are a lot of rays that will not hit that single point. Those will be out of focus.
//So the 'disk' is the defocus disk. The image has perfect clarity when the disk is of size 0. As it gets bigger depth of field changes.
inline vec3 random_in_unit_disk() {
    //Otherwise this function is the same as random_in_unit_sphere but for two dimensions.
    while (true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

//Hemisphere? what do you mean hemisphere?
//This is to make sure the ray bounces to a direction away from the sphere and not inside the sphere.
//We randomly determine bounce direction so we need to make sure for that.

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    //It works by dot product. A dot product can be seen as projection of one line onto the other.
    //So if the lines don't go in remotely the same direction the dot product will be negative.
    //(You may think it would be number not found but there's more to it than that.)
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

//Reflection for gauging, uhhh, reflections.
inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif  // VEC3_H
