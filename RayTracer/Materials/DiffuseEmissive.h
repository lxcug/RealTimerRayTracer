//
// Created by Tencent_GO on 2024/4/18.
//

#ifndef REAL_TIME_RAY_TRACER_DIFFUSEEMISSIVE_H
#define REAL_TIME_RAY_TRACER_DIFFUSEEMISSIVE_H

#include "Core/RealTimeRT.h"
#include "BxDF/DiffuseBxDF.h"
#include "Core/Intersection.h"
#include "Core/Ray.h"


#include "Material.h"
#include "BxDF/DiffuseBxDF.h"


class DiffuseEmissiveMaterial : public Material {
public:
    explicit DiffuseEmissiveMaterial(const glm::vec3 &albedo, float intensity = 1.f)
            : m_BxDF(albedo), m_intensity(intensity) {}

    DiffuseEmissiveMaterial(const glm::vec3 &albedo, const glm::vec3 &emissive,
                            float intensity = 1.f)
            : m_BxDF(albedo), m_emissive(emissive), m_intensity(intensity) {}

    [[nodiscard]] BxDFSample scatter(const Ray &ray, const Intersection &isect) const override {
        BxDFSample sample = m_BxDF.sampleF(-ray.direction, isect.normal);

        return sample;
    }

    [[nodiscard]] glm::vec3 emit(float u, float v, const glm::point3 &p) const override {
        return m_emissive * m_intensity;
    }

private:
    DiffuseBxDF m_BxDF;
    glm::vec3 m_emissive{1.f};
    float m_intensity{1.f};
};


#endif //REAL_TIME_RAY_TRACER_DIFFUSEEMISSIVE_H
