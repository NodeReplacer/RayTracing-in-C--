cmake_minimum_required(VERSION 3.25)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(RayTracing main.cpp vec3.h color.h ray.h hittable.h sphere.h hittable_list.h interval.h camera.h material.h parallel.h)
