// @author: lx
// @time: 2024/1/11
// @brief: 

#ifndef REAL_TIME_RAY_TRACER_INTERSECTION_H
#define REAL_TIME_RAY_TRACER_INTERSECTION_H

#include "Core/RealTimeRT.h"
#include <memory>


class Material;

struct Intersection {
    Intersection() = default;

    glm::point3 p;
    glm::vec3 normal;
    float t;
    std::shared_ptr<Material> mat;
    float u, v;
    bool backFace{false};
};


#endif //REAL_TIME_RAY_TRACER_INTERSECTION_H
