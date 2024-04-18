//
// Created by Tencent_GO on 2024/4/18.
//

#ifndef REAL_TIME_RAY_TRACER_DIFFUSEBXDF_H
#define REAL_TIME_RAY_TRACER_DIFFUSEBXDF_H

#include "BxDF.h"
#include "SampleUtils.h"


class DiffuseBxDF : public BxDF {
public:
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

        return {lightTransport, lightTransport, lightTransport};
    }

//    [[nodiscard]] glm::vec3
//    f(const glm::vec3 &wi, const glm::vec3 &wo, const glm::vec3 &normal) const override {
//        return glm::vec3(inv_pi);
//    }
//
//    [[nodiscard]] float
//    pdf(const glm::vec3 &wi, const glm::vec3 &wo, const glm::vec3 &normal) const override {
//        return glm::dot(wi, normal) * inv_pi;
//    }
};

#endif //REAL_TIME_RAY_TRACER_DIFFUSEBXDF_H
