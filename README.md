# Summary

![image](https://github.com/NodeReplacer/RayTracing-in-C--/assets/80176553/4af260f0-2a74-40f7-a0aa-e1a52329a1f1)

Exploring raytracing (pathtracing actually) in C++. The language doesn't matter so much as the 3D math does, which I found more important.

# USAGE 

The C++ program outputs the image to cout as text. Furthermore it is in .ppm format.

Therefore, the output must be piped to a .ppm file. For example: ./RayTracing.exe > image.ppm

PPM viewers can be downloaded or even used online. 

If you don't want to download a new program here is a link to an online PPM viewer: https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html

Note: Windows powershell outputs text in UTF-16, which WILL cause some issue with most ppm files. The normal cmd.exe program outputs it just fine so that can be used instead. Also the website linked can handle the distinction with no issue but if you want to keep the testing all offline then make sure that the program is run in something that outputs to ASCII or UTF-8. 

# Performance

The performance gets poorer the higher the cam.samples_per_pixel and cam.max_depth values go. Samples_per_pixel is done for anti-aliasing and depth refers to how many bounces the rays can perform before we just tell it to stop.

Currently the values are very high and that's because I was trying to render an impressive looking scene to show off, but it took about 7 hours because the program does not take up all of your CPU cycles generating the scene. And it shouldn't.

# HOWEVER

With the creation of (currently named) render2. The program will take up all of your CPU rendering the image. It multithreads but it does a lot more than that now. As such, the program now only renders a tiny image at low sample size and depth.

Turn those numbers up at your own risk.
