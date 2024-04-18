// @author: lx
// @time: 2024/1/11
// @brief: 

#ifndef REAL_TIME_RAY_TRACER_MATERIAL_H
#define REAL_TIME_RAY_TRACER_MATERIAL_H

#include "RealTimeRT.h"
#include "Ray.h"
#include "Intersection.h"
#include "BxDF/BxDF.h"


class Material {
public:
    virtual ~Material() = default;

    [[nodiscard]] virtual BxDFSample scatter(const Ray &ray, const Intersection &isect) const = 0;

    [[nodiscard]] virtual glm::vec3 emit(float u, float v, const glm::point3& p) const {
        return {0.f, 0.f, 0.f};
    }
};


//class Material {
//public:
//    virtual ~Material() = default;
//
//    virtual bool
//    scatter(const Ray &r, const Intersection &isect, Ray &scattered_ray, glm::vec3 &fr,
//            float &pdf) const = 0;
//
//    [[nodiscard]] virtual glm::vec3 emit(float u, float v, const glm::point3 &p) const {
//        return {0.f, 0.f, 0.f};
//    }
//};


//class Lambertian : public Material {
//public:
//    explicit Lambertian(const glm::vec3 &_color) : albedo(_color) {}
//
//    bool
//    scatter(const Ray &r, const Intersection &isect, Ray &scattered_ray, glm::vec3 &fr,
//            float &pdf) const override {
//        glm::vec3 scatter_dir = cos_weighted_hemisphere_sampling(isect.normal);
////        glm::vec3 scatter_dir = isect.normal + random_in_unit_sphere();
//        scattered_ray = Ray(isect.p, scatter_dir);
////        float cos = glm::dot(scattered_ray.direction, isect.normal);
//        fr = albedo;
//        pdf = 1.f;
//
//        return true;
//    }
//
//private:
//    glm::vec3 albedo{.5f, .5f, .5f};
//};
//
//class Emissive : public Material {
//public:
//    explicit Emissive(const glm::vec3 &_color) : albedo(_color) {}
//
//    bool
//    scatter(const Ray &r, const Intersection &isect, Ray &scattered_ray, glm::vec3 &fr,
//            float &pdf) const override {
//        glm::vec3 scatter_dir = cos_weighted_hemisphere_sampling(isect.normal);
////        glm::vec3 scatter_dir = isect.normal + random_in_unit_sphere();
//        scattered_ray = Ray(isect.p, scatter_dir);
////        float cos = glm::dot(scattered_ray.direction, isect.normal);
//        fr = albedo;
//        pdf = 1.f;
//
//        return true;
//    }
//
//    glm::vec3 emit(float u, float v, const glm::point3 &p) const override {
//        return glm::vec3(1.f);
//    }
//
//private:
//    glm::vec3 albedo{.5f, .5f, .5f};
//};
//
//
//class Metal : public Material {
//public:
//    explicit Metal(const glm::vec3 &a, float _fuzz = 0.f) : albedo(a),
//                                                            fuzz(_fuzz < 1. ? _fuzz : 1.) {}
//
//    bool
//    scatter(const Ray &r, const Intersection &isect, Ray &scattered_ray, glm::vec3 &fr,
//            float &pdf) const override {
//        glm::vec3 reflect_dir =
//                glm::reflect(r.direction, isect.normal) + fuzz * random_in_unit_sphere();
//        scattered_ray = Ray(isect.p, reflect_dir);
//        float cos = std::max(glm::dot(scattered_ray.direction, isect.normal), 0.f);
//        fr = albedo * cos;
//        pdf = 1.f;
//
//        return glm::dot(scattered_ray.direction, isect.normal) > 0.f;
//    }
//
//private:
//    glm::vec3 albedo;
//    float fuzz;
//};
//
//
//class PBRMaterial : public Material {
//public:
//    PBRMaterial(const glm::vec3 &_albedo, float _roughness, float _metallic)
//            : albedo(_albedo), roughness(_roughness), metallic(_metallic) {}
//
//    PBRMaterial(float _roughness, float _metallic) : roughness(_roughness), metallic(_metallic) {}
//
//    bool
//    scatter(const Ray &r, const Intersection &isect, Ray &scattered_ray, glm::vec3 &fr,
//            float &pdf) const override {
//        glm::vec3 scatter_dir = -D_GGX(roughness);
//        scattered_ray = Ray(isect.p, scatter_dir);
//        fr = albedo;
//        pdf = 1.f;
//
//        return true;
//    }
//
//private:
//    float roughness{1.f};
//    float metallic{0.f};
//    glm::vec3 albedo{.5f, .5f, .5f};
//
//    [[nodiscard]] static glm::vec3 D_GGX(float roughness) {
//        float a = roughness * roughness;
//        float xi1 = random_float(), xi2 = random_float();
//        float theta = std::atan(std::sqrt(-a * a * std::log(1 - xi1)));
//        float phi = 2 * pi * xi2;
//
//        return sphericalToCoords(theta, phi);
//    }
//
//    [[nodiscard]] static glm::vec3 sphericalToCoords(float theta, float phi) {
//        float sinTheta = std::sin(theta);
//        return {
//                sinTheta * std::cos(phi),
//                sinTheta * std::sin(phi),
//                std::cos(theta)
//        };
//    }
//};
//
//
//class Glossy : public Material {
//public:
//    bool scatter(const Ray &r, const Intersection &isect, Ray &scattered_ray, glm::vec3 &fr,
//                 float &pdf) const override {
//
//    }
//
//private:
//
//};




#endif //REAL_TIME_RAY_TRACER_MATERIAL_H
