cmake_minimum_required(VERSION 3.25)

project(real_time_ray_tracer)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(Walnut)

include_directories(RayTracer)

add_executable(real_time_ray_tracer
        RayTracer/WalnutApp.cpp
        RayTracer/Renderer.cpp
        RayTracer/Renderer.h
        RayTracer/Ray.h
        RayTracer/RealTimeRT.h
        RayTracer/Camera.cpp
        RayTracer/Camera.h
        RayTracer/Scene.cpp
        RayTracer/Scene.h
        RayTracer/Shapes/Primitive.h
        RayTracer/Intersection.h
        RayTracer/Shapes/Sphere.h
        RayTracer/Quad.h
        RayTracer/Materials/Material.h
        RayTracer/BVH.cpp
        RayTracer/BVH.h
        RayTracer/Bounds.cpp
        RayTracer/Bounds.h
        RayTracer/BxDF/BxDF.h
        RayTracer/Materials/Diffuse.h
        RayTracer/BxDF/DiffuseBxDF.h
        RayTracer/BxDF/SampleUtils.h
        RayTracer/Materials/DiffuseEmissive.h
        RayTracer/BxDF/SpecularBxDF.h
        RayTracer/Materials/Specular.h
)

target_link_libraries(
        real_time_ray_tracer
        PUBLIC walnut
)
