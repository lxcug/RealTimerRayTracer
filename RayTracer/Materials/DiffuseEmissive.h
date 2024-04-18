//
// Created by Tencent_GO on 2024/4/18.
//

#ifndef REAL_TIME_RAY_TRACER_DIFFUSEEMISSIVE_H
#define REAL_TIME_RAY_TRACER_DIFFUSEEMISSIVE_H

#include "RealTimeRT.h"
#include "BxDF/DiffuseBxDF.h"
#include "Intersection.h"
#include "Ray.h"


#include "Material.h"
#include "BxDF/DiffuseBxDF.h"


class DiffuseEmissiveMaterial : public Material {
public:
    explicit DiffuseEmissiveMaterial(const glm::vec3 &albedo) : m_albedo(albedo) {}

    DiffuseEmissiveMaterial(const glm::vec3 &albedo, const glm::vec3 &emissive)
            : m_albedo(albedo), m_emissive(emissive) {}

    [[nodiscard]] BxDFSample scatter(const Ray &ray, const Intersection &isect) const override {
        BxDFSample sample = m_BxDF.sampleF(-ray.direction, isect.normal);
        sample.lightTransport *= m_albedo;

        return sample;
    }

    [[nodiscard]] glm::vec3 emit(float u, float v, const glm::point3 &p) const override {
        return {1.f, 1.f, 1.f};
    }


private:
    DiffuseBxDF m_BxDF;
    glm::vec3 m_albedo{.5f};
    glm::vec3 m_emissive{1.f};
};


#endif //REAL_TIME_RAY_TRACER_DIFFUSEEMISSIVE_H
