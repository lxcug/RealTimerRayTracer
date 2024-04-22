//
// Created by Tencent_GO on 2024/4/18.
//

#ifndef REAL_TIME_RAY_TRACER_SPECULARBXDF_H
#define REAL_TIME_RAY_TRACER_SPECULARBXDF_H

#include "BxDF.h"
#include "SampleUtils.h"


class SpecularBxDF : public BxDF {
public:
    explicit SpecularBxDF(const glm::vec3 &albedo) : m_albedo(albedo) {}

    [[nodiscard]] BxDFSample sampleF(const glm::vec3 &wo, const glm::vec3 &normal) const override {
        BxDFSample sample;
        sample.flag = BxDFFlags::Specular;

        if (glm::dot(wo, normal) < 0.f)
            return sample;

        glm::vec3 wi = reflect(wo, normal);
        sample.wi = wi;
        sample.lightTransport = evalLightTransport(wi, wo, normal);

        return sample;
    }

    // Note: the incident direction wi is drawn from Dirac Delta Distribution.
    [[nodiscard]] glm::vec3 evalLightTransport(const glm::vec3 &wi, const glm::vec3 &wo,
                                               const glm::vec3 &normal) const override {
        float lightTransport = 1.f;

        return {lightTransport, lightTransport, lightTransport};
    }

private:
    glm::vec3 m_albedo{.5f};
};


#endif //REAL_TIME_RAY_TRACER_SPECULARBXDF_H
