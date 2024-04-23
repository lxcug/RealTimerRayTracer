// @author: lx
// @time: 2024/1/11
// @brief: 

#ifndef REAL_TIME_RAY_TRACER_SPHERE_H
#define REAL_TIME_RAY_TRACER_SPHERE_H

#include "shapes/Primitive.h"
#include "Materials/Material.h"
#include <iostream>
#include "Accelerator/Bounds.h"


class Sphere : public Primitive {
public:
    Sphere(float _radius, std::shared_ptr<Material> _mat);

    Sphere(const glm::point3 &_center, float _radius, std::shared_ptr<Material> _mat);

    [[nodiscard]] Bounds boundingBox() const override {
        glm::point3 center = glm::vec3(m_transform * glm::vec4(m_center, 1.f));
        auto bounds = Bounds(center - m_radius, center + m_radius);

        return bounds;
    }

    bool intersect(const Ray &ray, float tMax, Intersection &isect) const override;

    void setMaterial(const std::shared_ptr<Material> &mat) override {
        m_mat = mat;
    }

private:
    // Geometry
    glm::point3 m_center{0.f};
    float m_radius;
    // Material
    std::shared_ptr<Material> m_mat;
};


#endif //REAL_TIME_RAY_TRACER_SPHERE_H
