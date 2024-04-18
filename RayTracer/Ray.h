// @author: lx
// @time: 2024/1/11
// @brief: 

#ifndef REAL_TIME_RAY_TRACER_RAY_H
#define REAL_TIME_RAY_TRACER_RAY_H

#include "RealTimeRT.h"


class Ray {
public:
    Ray() = default;

    Ray(const glm::point3 &_p) : origin(_p) {}

    Ray(const glm::point3 &_origin, const glm::vec3 &direction, float time = 0.)
            : origin(_origin), direction(glm::normalize(direction)), time(time) {}

    glm::point3 at(float t) const {
        return origin + t * direction;
    }

//private:
    glm::point3 origin;
    glm::vec3 direction;
    float time;
};


#endif //REAL_TIME_RAY_TRACER_RAY_H
