// @author: lx
// @time: 2024/1/11
// @brief: 

#ifndef REAL_TIME_RAY_TRACER_QUAD_H
#define REAL_TIME_RAY_TRACER_QUAD_H

#include "shapes/Primitive.h"


class Quad : public Primitive {
public:

private:
    glm::point3 Q;
    glm::vec3 u, v;
};


#endif //REAL_TIME_RAY_TRACER_QUAD_H
