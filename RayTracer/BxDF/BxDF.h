//
// Created by Tencent_GO on 2024/4/18.
//

#ifndef REAL_TIME_RAY_TRACER_BXDF_H
#define REAL_TIME_RAY_TRACER_BXDF_H

#include "Core/RealTimeRT.h"


enum BxDFFlags {
    Unset = 0x0,
    Reflection = 1 << 1,
    Transmission = 1 << 2,
    Diffuse = 1 << 3,
    Glossy = 1 << 4,
    Specular = 1 << 5,
    DiffuseReflection = Diffuse | Reflection,
    DiffuseTransmission = Diffuse | Transmission,
    SpecularReflection = Specular | Reflection,
    SpecularTransmission = Specular | Transmission,

    All = Reflection | Transmission | Diffuse | Glossy | Specular
};

inline bool isReflection(BxDFFlags flag) {
    return flag & BxDFFlags::Reflection;
}

inline bool isTransmission(BxDFFlags flag) {
    return flag & BxDFFlags::Transmission;
}

inline bool isDiffuse(BxDFFlags flag) {
    return flag & BxDFFlags::Diffuse;
}

inline bool isGlossy(BxDFFlags flag) {
    return flag & BxDFFlags::Glossy;
}

inline bool isSpecular(BxDFFlags flag) {
    return flag & BxDFFlags::Specular;
}


struct BxDFSample {
    glm::vec3 wi;
    glm::vec3 lightTransport{0.f};  // albedo * f * cos(theta)
    BxDFFlags flag;
};


class BxDF {
public:
    /*!
     * Sample a incident direction wi and eval f and pdf, pack the data into BxDFSample.
     * @param wo
     * @return
     */
    [[nodiscard]] virtual BxDFSample
    sampleF(const glm::vec3 &wo, const glm::vec3 &normal) const = 0;

    [[nodiscard]] virtual glm::vec3 evalLightTransport(const glm::vec3 &wi, const glm::vec3 &wo,
                                           const glm::vec3 &normal) const = 0;
};


#endif //REAL_TIME_RAY_TRACER_BXDF_H
