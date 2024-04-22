//
// Created by Tencent_GO on 2024/4/19.
//

#ifndef REAL_TIME_RAY_TRACER_GLOSSYBXDF_H
#define REAL_TIME_RAY_TRACER_GLOSSYBXDF_H

#include "BxDF.h"
#include "SampleUtils.h"

#define MICROFACET_SAMPLE_COSINE_HEMISPHERE 1
#if MICROFACET_SAMPLE_COSINE_HEMISPHERE
#define MICROFACET_SAMPLE_IMPORTANCE 0
#else
#define MICROFACET_SAMPLE_IMPORTANCE 1
#endif


class GlossyBxDF : public BxDF {
public:
    explicit GlossyBxDF(const glm::vec3 &albedo) : m_albedo(albedo) {}

    GlossyBxDF(const glm::vec3 &albedo, float roughness)
            : m_albedo(albedo), m_roughness(roughness), m_alpha(std::sqrt(m_roughness)) {
        if (m_alpha < .3f) {
            m_alpha = std::clamp(2 * m_alpha, .1f, .3f);
        }
    }

    [[nodiscard]] BxDFSample
    sampleF(const glm::vec3 &wo, const glm::vec3 &normal) const override {
        BxDFSample sample;
        sample.flag = BxDFFlags::Glossy;

#if MICROFACET_SAMPLE_COSINE_HEMISPHERE
        sample.wi = SampleUtils::sampleCosineHemisphere(normal);
#endif

#if MICROFACET_SAMPLE_IMPORTANCE
        // BUG: problem with sampling wm
        glm::vec3 wm = SampleUtils::Microfacet::sampleMicrofacetNormal(wo, normal, m_alpha);
        if (glm::dot(wo, wm) < 0.f)
            return sample;

        sample.wi = reflect(wo, wm);
#endif
        sample.lightTransport = evalLightTransport(sample.wi, wo, normal);

        return sample;
    }

    [[nodiscard]] glm::vec3 evalLightTransport(const glm::vec3 &wi, const glm::vec3 &wo,
                                               const glm::vec3 &normal) const override {
#if MICROFACET_SAMPLE_COSINE_HEMISPHERE
        float pdf = inv_pi;
        glm::vec3 f = m_albedo * SampleUtils::Microfacet::BRDF(wi, wo, normal, m_alpha);
#endif

#if MICROFACET_SAMPLE_IMPORTANCE
        glm::vec3 wm = glm::normalize(wi + wo);
        float cosTheta = glm::dot(wi, normal);
        if (cosTheta < 0.f)
            return glm::vec3(0.f);
        float pdf = SampleUtils::Microfacet::PDF_wi(wo, wm, normal, m_alpha);
        glm::vec3 f =
                m_albedo * SampleUtils::Microfacet::BRDF(wi, wo, normal, m_alpha) * cosTheta;
        if (isNan(f))
            return glm::vec3(1.f);
#endif

        return f / pdf;
    }

private:
    glm::vec3 m_albedo{.5f};
    float m_roughness{1.f};
    float m_alpha{std::sqrt(m_roughness)};
};

#endif //REAL_TIME_RAY_TRACER_GLOSSYBXDF_H
