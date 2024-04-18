// @author: lx
// @time: 2024/1/11
// @brief: 

#ifndef REAL_TIME_RAY_TRACER_PRIMITIVE_H
#define REAL_TIME_RAY_TRACER_PRIMITIVE_H

#include "RealTimeRT.h"
#include "Ray.h"
#include "Intersection.h"


class Bounds;

class Primitive {
public:
    virtual bool intersect(const Ray &ray, float tMax, Intersection &isect) const = 0;

    [[nodiscard]] virtual bool intersectP(const Ray& r, float tMax) const { return false; }

    [[nodiscard]] virtual const Bounds &boundingBox() const = 0;
};


#endif //REAL_TIME_RAY_TRACER_PRIMITIVE_H
