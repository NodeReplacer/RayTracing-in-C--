//
// Created by user on 1/25/2024.
//

#ifndef COMMON_CONSTANTS_H
#define COMMON_CONSTANTS_H

#include <random>
#include <cmath>
#include <limits>
#include <memory>

std::random_device rd;
std::minstd_rand mt(rd());
std::uniform_real_distribution<double> dist(0, 1.0);

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}
inline double random_double() {
    // Returns a random real in [0,1).
    return dist(mt);
    
    //return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

// Common Headers

#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif //COMMON_CONSTANTS_H
