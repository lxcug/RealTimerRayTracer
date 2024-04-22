//
// Created by Tencent_GO on 2024/4/22.
//

#include "Core/RealTimeRT.h"
#include "BxDF/BxDF.h"
#include "BxDF/GlossyBxDF.h"
#include <iostream>


void testSampling() {
    glm::vec3 normal{0.f, 0.f, 1.f};

    glm::vec3 wo{-.5f, 0.f, .5f};
    wo = glm::normalize(wo);
    std::cout << "wo: ";
    printVec3(wo);
    float roughness = .1f;
    std::cout << roughness << std::endl;

    random_vec2();
    random_vec2();

    glm::vec3 wm = SampleUtils::Microfacet::sampleMicrofacetNormal(wo, normal, roughness);
    std::cout << "Sampled wm ";
    printVec3(wm);
    glm::vec3 wi = reflect(wo, wm);
    std::cout << "Sampled wi ";
    printVec3(wi);

    float brdf = SampleUtils::Microfacet::BRDF(wi, wo, normal, roughness);
    std::cout << brdf << std::endl;

    float pdf = SampleUtils::Microfacet::PDF_wi(wo, wm, normal, roughness);
    std::cout << pdf << std::endl;

    std::cout << brdf / pdf << std::endl;
}


int main() {
    testSampling();

    return 0;
}
