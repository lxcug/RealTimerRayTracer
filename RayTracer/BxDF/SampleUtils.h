//
// Created by Tencent_GO on 2024/4/18.
//

#ifndef REAL_TIME_RAY_TRACER_SAMPLEUTILS_H
#define REAL_TIME_RAY_TRACER_SAMPLEUTILS_H

#include "RealTimeRT.h"


namespace SampleUtils {

    glm::vec3 sampleCosineHemisphere(const glm::vec3 &normal) {
        float xi1 = random_float(), xi2 = random_float();

        float theta = std::acos(sqrt(1.f - xi1));
        float phi = 2.f * pi * xi2;
        float sin_theta = std::sin(theta);
        glm::vec3 local_dir(
                sin_theta * cos(phi),
                sin_theta * sin(phi),
                cos(theta)
        );

        if (glm::dot(local_dir, normal) < 0.f)
            return -local_dir;
        else
            return local_dir;
//        return buildCoord(normal) * local_dir;
    }


}


#endif //REAL_TIME_RAY_TRACER_SAMPLEUTILS_H
