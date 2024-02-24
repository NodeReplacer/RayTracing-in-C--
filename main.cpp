#include "common_constants.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

#include <iostream>
#include <vector>
#include <future>

int process(int value) {
    // Perform some computation or task
    // For simplicity, just square the input value
    std::this_thread::sleep_for(std::chrono::milliseconds(1000  ));
    return value * value;
}

int main() {

    // World

    hittable_list world; //aka the scene we are rendering.

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    //I'm still not sure about OOP and all that encapsulation, inheritance, etc.
    //Because now if someone wants to know what my code is about they have to chase my definitions around
    //and try and guess where some of them are.
    //How fortunate for us that they are simply named after themselves so camera is in camera.h but that's not always
    //the case (rarely the case) in most programs.

    //So I just had an experience that illustrated my problem. I had to go modify my write_color function and now I had to
    //go searching through all my files to remember where it was. Instead of just going here and searching this one file.
    //Good thing my IDE lets me search between files. Just ignore the fact that I would not need that function right now
    //if I never did this encapsulate inheritance thing to begin with.

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 20;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.position = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;
    
    //render - No async used. The for loop colors lines one by one.
    //render2 - Async (and therefore multiple threads) used. No limit to CPU usage. You WILL max out all your cores if use this method. Though it is fast.
    //render3 - To Be Created. Divide the jobs into a limited number of batches. Give each their own thread. Won't be as fast as render2 but will give the CPU
    //          some rest.
    
    cam.render2(world); //render2 is asynchronously multi-threaded. It will max out your CPU on all cores as it did mine.
                        //I was planning on dividing the image into jobs and creating a limited number of threads to handle
                        //it, but haven't gotten around to it yet. It would be easy though.
    
}
