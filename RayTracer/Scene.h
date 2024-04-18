// @author: lx
// @time: 2024/1/11
// @brief: 

#ifndef REAL_TIME_RAY_TRACER_SCENE_H
#define REAL_TIME_RAY_TRACER_SCENE_H

#include "RealTimeRT.h"
#include "shapes/Sphere.h"
#include <vector>
#include "BVH.h"


class Scene {
public:
    Scene();

    [[nodiscard]] const std::vector<std::shared_ptr<Primitive>> &getObjects() const {
        return m_objects;
    }

    void addObject(const std::shared_ptr<Primitive> &object) {
        m_objects.push_back(object);
    }

    // TODO: BVH Accelerator
    bool intersect(const Ray &r, Intersection &isect) const;

    glm::vec3 &getBackgroundColor() {
        return m_background_color;
    }

    [[nodiscard]] const glm::vec3 &getBackgroundColor() const {
        return m_background_color;
    }

private:
    std::vector<std::shared_ptr<Primitive>> m_objects;
    glm::vec3 m_background_color{0.f};
    std::shared_ptr<BVH> m_bvh_tree;
};


#endif //REAL_TIME_RAY_TRACER_SCENE_H
