//
// Created by Tencent_GO on 2024/4/19.
//

#ifndef REAL_TIME_RAY_TRACER_GLOSSYBXDF_H
#define REAL_TIME_RAY_TRACER_GLOSSYBXDF_H

#include "BxDF.h"
#include "SampleUtils.h"


class GlossyBxDF : public BxDF {
public:
    explicit GlossyBxDF(const glm::vec3 &albedo) : m_albedo(albedo) {}

    GlossyBxDF(const glm::vec3 &albedo, float roughness)
            : m_albedo(albedo), m_roughness(roughness) {}

    [[nodiscard]] BxDFSample
    sampleF(const glm::vec3 &wo, const glm::vec3 &normal) const override {
        BxDFSample sample;
        glm::vec3 wm = SampleUtils::Microfacet::sampleMicrofacet(wo, normal, m_roughness);
        sample.wi = glm::reflect(-wo, wm);
        sample.lightTransport = evalLightTransport(wo, wm, normal);
        // Note: Add cosine term to light transport
        sample.lightTransport *= glm::dot(sample.wi, normal);
        sample.flag = BxDFFlags::Glossy;

        return sample;
    }

    [[nodiscard]] glm::vec3 evalLightTransport(const glm::vec3 &wo, const glm::vec3 &wm,
                                               const glm::vec3 &normal) const override {
        float pdf = SampleUtils::Microfacet::PDF(wo, wm, normal, m_roughness);
        glm::vec3 f = m_albedo;  // TODO: cal Microfacet BRDF;

        return f / pdf;
    }

private:
    glm::vec3 m_albedo{.5f};
    float m_roughness{1.f};
};

#endif //REAL_TIME_RAY_TRACER_GLOSSYBXDF_H
