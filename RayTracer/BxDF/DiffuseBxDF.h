//
// Created by Tencent_GO on 2024/4/18.
//

#ifndef REAL_TIME_RAY_TRACER_DIFFUSEBXDF_H
#define REAL_TIME_RAY_TRACER_DIFFUSEBXDF_H

#include "BxDF.h"
#include "SampleUtils.h"


class DiffuseBxDF : public BxDF {
public:
    explicit DiffuseBxDF(const glm::vec3& albedo) : m_albedo(albedo) {}

    [[nodiscard]] BxDFSample sampleF(const glm::vec3 &wo, const glm::vec3 &normal) const override {
        BxDFSample sample;

        glm::vec3 wi = SampleUtils::sampleCosineHemisphere(normal);
        sample.wi = wi;
        sample.lightTransport = evalLightTransport(wi, wo, normal);
        sample.flag = BxDFFlags::Diffuse;

        return sample;
    }

    [[nodiscard]] glm::vec3 evalLightTransport(const glm::vec3 &wi, const glm::vec3 &wo,
                                 const glm::vec3 &normal) const override {
        float lightTransport = 1.f;

        return 1.f * m_albedo;
    }

private:
    glm::vec3 m_albedo{.5f};
};

#endif //REAL_TIME_RAY_TRACER_DIFFUSEBXDF_H
