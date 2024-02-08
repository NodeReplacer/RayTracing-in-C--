#ifndef CAMERA_H
#define CAMERA_H

#include "common_constants.h"

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "parallel.h"

#include <iostream>
#include <thread>
#include <vector>
#include <future>

#define PARALLEL_FOR_BEGIN(nb_elements) parallel_for(nb_elements, [&](int start, int end){ for(int i = start; i < end; ++i)
#define PARALLEL_FOR_END()})

int progress;

class camera {
public:
    // Image
    /*
    //To think that image started out like this, huh? But to make a pathtracer (this is not a real raytracer) we need to make the scene
    //3D. Which means cameras and an aspect ratio instead of hard coding the number of pixels an image is.
    int image_width = 256;
    int image_height = 256;
    */
    
    //And here it is, our new image dimensions.
    //I do like the fact that organizing them like this makes it immediately apparent this is a list of features.
    //I can use this for formatting later.
    double aspect_ratio      = 1.0; //Default. Should be overwritten.
    int    image_width       = 100; //Default. Should be overwritten. 
    int    samples_per_pixel = 10;  //Count of random samples for each pixel
    int    max_depth         = 10;  //Maximum number of ray bounces in the scene. Why? Because ray_color is recursive. It stops recursing when it fails to 
                                    //hit anything. That's long enough to blow the stack if it happens too long. So we return no light contribution
                                    //At maximum depth.
    
    double vfov = 90;  // Vertical view angle (field of view)
    point3 position = point3(0,0,-1);  // Point camera is looking from
    point3 lookat   = point3(0,0,0);   // Point camera is looking at
    vec3   vup      = vec3(0,1,0);     // Camera-relative "up" direction

    double defocus_angle = 0;  // Variation angle of rays through each pixel
    double focus_dist = 10;    // Distance from camera position point to distance where focus is perfect.
    
    //But if they aren't overwritten then the program won't explode.
    
    std::string renderLine(int image_width, int samples_per_pixel, const hittable& world, int j) {
        std::string coloredLine;
        
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0,0,0); //Base pixel color of 'no values'.
            for (int sample = 0; sample < samples_per_pixel; ++sample) {
                //Default sample size is set in int main() for now. But there is a default value for samples_per_pixel
                ray r = get_ray(i, j);
                pixel_color += ray_color(r, max_depth, world);
            }
            coloredLine.append(async_write_color(pixel_color, samples_per_pixel));
        }
        //Probably should sink this. The clog happens asynchronously so it won't really help track the progress to the user
        //but the user probably needs this just to know that something is happening behind the hood.
        std::clog << "\rScanlines remaining: " << --progress << ' ' << std::flush;
        return coloredLine;
    }
    
    void render2(const hittable& world) {
        initialize();
        
        std::vector<std::future<std::string>> futures;
        progress = image_height; //Progress indicator for async to display progress to the user. 
        // Render
        //PPM image header - This header is ID. Tells programs what kind of file it is (ppm in our case).
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        
        for (int j = 0; j < image_height; ++j) {
            //Progress marker
            //std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            futures.push_back(std::async(std::launch::async, &camera::renderLine, this, image_width, samples_per_pixel, std::cref(world), j));
            //renderLine(image_width,samples_per_pixel,world,j);
            //Do it line by line and make sure each line returns in the correct order.
            //futures.push_back(std::async(std::launch::async, &camera::renderLine, image_width, samples_per_pixel, world, j));
            /*
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0,0,0); //Base pixel color of 'no values'.
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    //Default sample size is set in int main() for now. But there is a default value for samples_per_pixel
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_color, samples_per_pixel);
            }
            */
        }
        for (auto& jobList : futures) {
            std::string result = jobList.get();
            //std::clog << "Job List\n";
            std::cout << result;
        }
        std::clog << "\rDone. Used render2                 \n"; 
    }
    
    void render(const hittable& world) {
        initialize();
        
        // Render
        //PPM image header - This header is ID. Tells programs what kind of file it is (ppm in our case).
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        
        for (int j = 0; j < image_height; ++j) {
            //Progress marker
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            
            for (int i = 0; i < image_width; ++i) {
                /*
                // Our very beginning picture colorer. Before we migrated all of the necessary parts to vec3.h and color.h.
                // Without them we write to the file correctly but don't store anything in the program except for the file
                // So if we wanted to do anything else to the picture we wouldn't be able to without hard coding it right here.
                auto r = double(i) / (image_width-1);
                auto g = double(j) / (image_height-1);
                auto b = 0;
    
                int ir = static_cast<int>(255.999 * r);
                int ig = static_cast<int>(255.999 * g);
                int ib = static_cast<int>(255.999 * b);
    
                std::cout << ir << ' ' << ig << ' ' << ib << '\n';
                */
                color pixel_color(0,0,0); //Base pixel color of 'no values'.
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    //Default sample size is set in int main() for now. But there is a default value for samples_per_pixel
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_color, samples_per_pixel);
            }
        }
        std::clog << "\rDone. Used render1                 \n";
    }

private:
    int    image_height;   // Rendered image height
    point3 center;         // Camera center
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below
    vec3   u, v, w;        // Camera frame basis vectors
    vec3   defocus_disk_u;  // Defocus disk horizontal radius
    vec3   defocus_disk_v;  // Defocus disk vertical radius
    
    void initialize() {
        
        // Calculate the image height, and ensure that it's at least 1.
        image_height = static_cast<int>(image_width / aspect_ratio); //Image height is not a real value and it is the smaller of the two values
        image_height = (image_height < 1) ? 1 : image_height; //Images don't have -1 sizes in either width or height, but width is larger than height in
        //our aspect ratio (16:9) so we know that height will always hit 1 (or go below 1) before width does
        //if we start shrinking the image.
        
        center = position;
        
        // Camera

        //Now that we're going into 3D, the image can be bigger than the camera view so we need a camera.
        //Find the Field of View.
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = unit_vector(position - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        
        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge
        //viewport_height increases.

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the size of our defocus disk. (Depth of field)
        auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray(int i, int j) const {
        //Get a randomly sampled camera ray for the pixel at location i,j.
        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        
        //In case you forget, pixel00_loc is the pixel in the top left corner. I'd name the variable pixel(0,0) if
        //I could.
        
        //The heck do we need this for?
        //We need it because we are using rays.
        //So the ray projects from the camera
        //to a pixel.
        //pixel_center is actually pixel LOCATION.
        //(Of our target I mean. In world space)
        auto pixel_sample = pixel_center + pixel_sample_square(); //Now we sample the square around the pixel for anti-aliasing.

        //Calculate the ray direction from the camera center to the pixel center
        //auto ray_origin = center; //With this, the focus is perfect. Our defocusing lens is size 0.
        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }
    
    vec3 pixel_sample_square() const {
        // Returns a random point in the square surrounding a pixel at the origin.
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }
    
    point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {
        hit_record rec;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return color(0,0,0);

        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);
            return color(0,0,0);
        }

        //Make whatever ray we were given a unit vector (that means make it length 1 but still pointing in where it is supposed to be pointing.
        vec3 unit_direction = unit_vector(r.direction());

        auto a = 0.5*(unit_direction.y() + 1.0);
        //Below is linear interpolation which is blendedValue = (1−a)*startValue+a*endValue
        //What's 'a'? It determines where we are in the interpolation. If 'a' is 1 we are at our endValue, if it is 0 then we are at our startValue
        //That is: The equation will return the startValue or endValue depending where we are.
        //If 'a' is between 0 and 1 then we are returning a value between our start and end value.
        //So therefore 'a' can be anything because it is determined by us and what we want. Right now I want 'a' to change as we go down the image.
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }
};

#endif