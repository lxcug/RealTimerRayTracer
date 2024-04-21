//
// Created by HUSTLX on 2024/4/21.
//

#ifndef REAL_TIME_RAY_TRACER_GLOSSY_H
#define REAL_TIME_RAY_TRACER_GLOSSY_H

#include "Material.h"
#include "BxDF/GlossyBxDF.h"


class GlossyMaterial : public Material {
public:
    explicit GlossyMaterial(const glm::vec3 &albedo, float roughness = 1.f): m_BxDF(albedo, roughness) {}

    [[nodiscard]] BxDFSample scatter(const Ray &ray, const Intersection &isect) const override {
        BxDFSample sample = m_BxDF.sampleF(-ray.direction, isect.normal);

        return sample;
    }

private:
    GlossyBxDF m_BxDF;
    float m_roughness{1.f};
};


#endif //REAL_TIME_RAY_TRACER_GLOSSY_H
