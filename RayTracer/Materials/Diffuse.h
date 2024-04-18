//
// Created by Tencent_GO on 2024/4/18.
//

#ifndef REAL_TIME_RAY_TRACER_DIFFUSE_H
#define REAL_TIME_RAY_TRACER_DIFFUSE_H

#include "Material.h"
#include "BxDF/DiffuseBxDF.h"


class DiffuseMaterial : public Material {
public:
    explicit DiffuseMaterial(const glm::vec3 albedo) : m_albedo(albedo) {}

    [[nodiscard]] BxDFSample scatter(const Ray &ray, const Intersection &isect) const override {
        BxDFSample sample = m_BxDF.sampleF(-ray.direction, isect.normal);
        sample.lightTransport *= m_albedo;

        return sample;
    }

private:
    DiffuseBxDF m_BxDF;
    glm::vec3 m_albedo{.5f};
};


#endif //REAL_TIME_RAY_TRACER_DIFFUSE_H
