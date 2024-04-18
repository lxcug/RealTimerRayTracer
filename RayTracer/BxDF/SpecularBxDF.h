//
// Created by Tencent_GO on 2024/4/18.
//

#ifndef REAL_TIME_RAY_TRACER_SPECULARBXDF_H
#define REAL_TIME_RAY_TRACER_SPECULARBXDF_H

#include "BxDF.h"
#include "SampleUtils.h"


class SpecularBxDF : public BxDF {
public:
    [[nodiscard]] BxDFSample sampleF(const glm::vec3 &wo, const glm::vec3 &normal) const override {
        BxDFSample sample;

        glm::vec3 wi = glm::reflect(-wo, normal);
        sample.wi = wi;
        sample.lightTransport = evalLightTransport(wi, wo, normal);
        sample.flag = BxDFFlags::Specular;

        return sample;
    }

    // Note: the incident direction wi is drawn from Dirac Delta Distribution.
    [[nodiscard]] glm::vec3 evalLightTransport(const glm::vec3 &wi, const glm::vec3 &wo,
                                 const glm::vec3 &normal) const override {
        float lightTransport = 1.f;

        return {lightTransport, lightTransport, lightTransport};
    }

//    [[nodiscard]] glm::vec3 f(const glm::vec3 &wi, const glm::vec3 &wo, const glm::vec3& normal) const override {
//        float cosTheta = std::max(0.f, glm::dot(wi, normal));
//
//        return {1.f / cosTheta, 1.f / cosTheta, 1.f / cosTheta};
//    }
//
//    [[nodiscard]] float
//    pdf(const glm::vec3 &wi, const glm::vec3 &wo, const glm::vec3 &normal) const override {
//        // TODO: Add wi and reflect direction dot condition
//        return 1.f;
//    }
};


#endif //REAL_TIME_RAY_TRACER_SPECULARBXDF_H
