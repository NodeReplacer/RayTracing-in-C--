# Summary

Exploring raytracing (pathtracing actually) in C++. The language doesn't matter so much as the 3D math does, which I found more important.

# USAGE 

The C++ program outputs the image to cout as text. Furthermore it is in .ppm format.

Therefore, the output must be piped to a .ppm file. For example: ./RayTracing.exe > image.ppm

PPM viewers can be downloaded or even used online. 

If you don't want to download a new program here is a link to an online PPM viewer: https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html

# Performance

The performance gets poorer the higher the cam.samples_per_pixel and cam.max_depth values go. Samples_per_pixel is done for anti-aliasing and depth refers to how many bounces the rays can perform before we just tell it to stop.

Currently the values are very high and that's because I was trying to render an impressive looking scene to show off, but it took about 7 hours because the program does not take up all of your CPU cycles generating the scene. And it shouldn't.
