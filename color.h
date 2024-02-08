#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>
#include <sstream>

using color = vec3;

//In linear space the image is too dark because it is assumed that the image will be 'gamma corrected'
//'Gamma corrected' means that there will be some transform applied to the 0 to 1 values before being stored as a byte.
//The 0 to 1 values are the brightness values. Truthfully, the 0 to 1 value is the reflectiveness of each ray.
//so 0.5 is 50% reflective.
inline double linear_to_gamma(double linear_component)
{
    return sqrt(linear_component);
}
void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    /*
    // Write the translated [0,255] value of each color component. RGB.
    // This method causes aliasing to occur. So we'll instead average out the aliasing.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
    */
    
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();
    
    // Divide the color by the number of samples. Anti-aliasing.
    auto scale = 1.0 / samples_per_pixel; //I've been using auto as much as I can to get used to it but I'm getting
    //really suspicious of it at this point.
    r *= scale;
    g *= scale;
    b *= scale;

    // Apply the linear to gamma transform.
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);
    
    // Write the translated [0,255] value of each color component.
    static const interval intensity(0.000, 0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}

std::string async_write_color(color pixel_color, int samples_per_pixel) {
    
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples. Anti-aliasing.
    auto scale = 1.0 / samples_per_pixel; //I've been using auto as much as I can to get used to it but I'm getting
    //really suspicious of it at this point.
    r *= scale;
    g *= scale;
    b *= scale;

    // Apply the linear to gamma transform.
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Write the translated [0,255] value of each color component.
    static const interval intensity(0.000, 0.999);
    
    std::string result = std::to_string(static_cast<int>(256 * intensity.clamp(r))) + ' '
                         + std::to_string(static_cast<int>(256 * intensity.clamp(g))) + ' '
                         + std::to_string(static_cast<int>(256 * intensity.clamp(b))) + '\n';
    return result;
}

#endif