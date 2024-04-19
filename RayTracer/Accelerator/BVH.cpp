#include "BVH.h"
#include <iostream>
#include <algorithm>
#include <span>


BVH::BVH(const std::vector<std::shared_ptr<Primitive>> &primitives) {
    assert(primitives.size() != 0);

    std::vector<std::shared_ptr<Primitive>> primitives_copy = primitives;

    ordered_primitives.reserve(primitives.size());

    // TODO: use std::span
    root = recursiveBuild(primitives_copy, 0, (int) primitives_copy.size());
}

BVHNode *
BVH::recursiveBuild(std::vector<std::shared_ptr<Primitive>> &primitives, int start, int end) {
    BVHNode *currentNode = nullptr;

    if (start >= end)
        return currentNode;

    currentNode = new BVHNode();

    Bounds currentBounds;
    for (int primIndex = start; primIndex < end; primIndex++) {
        currentBounds.unionBounds(primitives[primIndex]->boundingBox());
    }

    int nPrimitives = end - start;

    if (nPrimitives <= BVH_LEAF_MAX_PRIMITIVES) {
        for (int primIndex = start; primIndex < end; primIndex++) {
            ordered_primitives.push_back(primitives[primIndex]);
        }

        currentNode->bbox = currentBounds;
        currentNode->first_primitive_index = start;
        currentNode->n_primitives = nPrimitives;
    } else {
        switch (splitMethod) {
            case BVHSplitMethod::EqualCount: {
                Bounds centroidBounds;
                for (int primIndex = start; primIndex < end; primIndex++) {
                    currentBounds.unionPoint(primitives[primIndex]->boundingBox().middle());
                }
                int axis = centroidBounds.maxDim();

                int mid = start + nPrimitives / 2;

                std::nth_element(&primitives[start], &primitives[mid], &primitives[end - 1] + 1,
                                 [axis](const std::shared_ptr<Primitive> &lhs,
                                        const std::shared_ptr<Primitive> &rhs) {
                                     return lhs->boundingBox().middle()[axis] <
                                            rhs->boundingBox().middle()[axis];
                                 });

                currentNode->left = recursiveBuild(primitives, start, mid);
                currentNode->right = recursiveBuild(primitives, mid, end);
                currentNode->bbox = currentBounds;
                currentNode->first_primitive_index = -1;
                currentNode->n_primitives = 0;
                break;
            }
        }
    }

    return currentNode;
}

bool BVH::intersect(const Ray &r, float tMax, Intersection &isect) const {
    return intersect(root, r, tMax, isect);
}

bool BVH::intersect(BVHNode *node, const Ray &r, float tMax, Intersection &isect) const {
    if (!node->bbox.intersect(r))
        return false;

    if (node->n_primitives != 0) {
        bool hit = false;
        for (int i = 0; i < node->n_primitives; i++) {
            if (ordered_primitives[node->first_primitive_index + i]->intersect(r, tMax, isect)) {
                hit = true;
                tMax = isect.t;
            }
        }
        return hit;
    } else {
        bool hit_left = intersect(node->left, r, tMax, isect);
        bool hit_right = intersect(node->right, r, hit_left ? isect.t : tMax, isect);

        return hit_left || hit_right;
    }
}
