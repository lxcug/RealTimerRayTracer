cmake_minimum_required(VERSION 3.25)

project(real_time_ray_tracer)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(Walnut)

include_directories(RayTracer)

add_executable(real_time_ray_tracer
        RayTracer/WalnutApp.cpp
        RayTracer/Core/Renderer.cpp
        RayTracer/Core/Renderer.h
        RayTracer/Core/Ray.h
        RayTracer/Core/RealTimeRT.h
        RayTracer/Core/Camera.cpp
        RayTracer/Core/Camera.h
        RayTracer/Core/Scene.cpp
        RayTracer/Core/Scene.h
        RayTracer/Shapes/Primitive.h
        RayTracer/Core/Intersection.h
        RayTracer/Shapes/Sphere.h
        RayTracer/Shapes/Quad.h
        RayTracer/Materials/Material.h
        RayTracer/Accelerator/BVH.cpp
        RayTracer/Accelerator/BVH.h
        RayTracer/Accelerator/Bounds.cpp
        RayTracer/Accelerator/Bounds.h
        RayTracer/BxDF/BxDF.h
        RayTracer/Materials/Diffuse.h
        RayTracer/BxDF/DiffuseBxDF.h
        RayTracer/BxDF/SampleUtils.h
        RayTracer/Materials/DiffuseEmissive.h
        RayTracer/BxDF/SpecularBxDF.h
        RayTracer/Materials/Specular.h
        RayTracer/Samples/CreateScene.h
        RayTracer/BxDF/GlossyBxDF.h
        RayTracer/Materials/Glossy.h
)

target_link_libraries(
        real_time_ray_tracer
        PUBLIC walnut
)
