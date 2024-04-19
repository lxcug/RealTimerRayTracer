// @author: lx
// @time: 2024/1/13
// @brief: 

#ifndef REAL_TIME_RAY_TRACER_BVH_H
#define REAL_TIME_RAY_TRACER_BVH_H

#include "Core/RealTimeRT.h"
#include "Accelerator/Bounds.h"
#include "Shapes/Primitive.h"

#define BVH_LEAF_MAX_PRIMITIVES 4


struct BVHNode {
    BVHNode() = default;

    Bounds bbox;
    BVHNode *left = nullptr, *right = nullptr;
    uint32_t n_primitives = 0;
    int first_primitive_index = -1;

    ~BVHNode() {
        delete left;
        delete right;
    }
};


enum class BVHSplitMethod {
    EqualCount = 0x0,
    Middle,
    SurfaceAreaHeuristic,
    SAH = SurfaceAreaHeuristic,
};


class BVH {
public:
    explicit BVH(const std::vector<std::shared_ptr<Primitive>> &primitives);

    ~BVH() {
        delete root;
    }

    bool intersect(const Ray &r, float tMax, Intersection &isect) const;

private:
    BVHNode *
    recursiveBuild(std::vector<std::shared_ptr<Primitive>> &primitives, int start, int end);

    bool intersect(BVHNode *node, const Ray &r, float tMax, Intersection &isect) const;

private:
    BVHNode *root{nullptr};
    std::vector<std::shared_ptr<Primitive>> ordered_primitives;
    BVHSplitMethod splitMethod{BVHSplitMethod::EqualCount};
};


#endif //REAL_TIME_RAY_TRACER_BVH_H
