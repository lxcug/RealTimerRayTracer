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
        glm::vec3 wm = SampleUtils::Microfacet::sampleMicrofacetNormal(wo, normal, m_roughness);
        wm = glm::vec3(0.f, 0.f, 1.f);
        sample.wi = glm::reflect(-wo, wm);
        sample.lightTransport = evalLightTransport(wo, wm, normal);
        sample.flag = BxDFFlags::Glossy;

        return sample;
    }

    [[nodiscard]] glm::vec3 evalLightTransport(const glm::vec3 &wi, const glm::vec3 &wo,
                                               const glm::vec3 &normal) const override {
        glm::vec3 wm = glm::normalize(wi + wo);

        float pdf = SampleUtils::Microfacet::PDF_wi(wo, wm, normal, m_roughness);
        float cosTheta = std::max(0.f, glm::dot(wi, normal));
        glm::vec3 f =
                m_albedo * SampleUtils::Microfacet::BRDF(wi, wo, normal, m_roughness) * cosTheta;

        return f / pdf;
    }

private:
    glm::vec3 m_albedo{.5f};
    float m_roughness{1.f};
};

#endif //REAL_TIME_RAY_TRACER_GLOSSYBXDF_H
