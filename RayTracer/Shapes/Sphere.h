// @author: lx
// @time: 2024/1/11
// @brief: 

#ifndef REAL_TIME_RAY_TRACER_SPHERE_H
#define REAL_TIME_RAY_TRACER_SPHERE_H

#include "shapes/Primitive.h"
#include "Materials/Material.h"
#include <iostream>
#include "Bounds.h"


class Sphere : public Primitive {
public:
    Sphere(const glm::point3 &_center, float _radius, std::shared_ptr<Material> _mat)
            : m_center(_center), m_radius(_radius), m_mat(std::move(_mat)) {
        m_bounding_box = Bounds(m_center - m_radius, m_center + m_radius);
    }

    [[nodiscard]] const Bounds & boundingBox() const override {
        return m_bounding_box;
    }

    bool intersect(const Ray &ray, float tMax, Intersection &isect) const override {
        using namespace glm;

        vec3 o = ray.origin - m_center;
        vec3 d = ray.direction;
        float a = length(d);
        float b = 2 * dot(o, d);
        float c = o.x * o.x + o.y * o.y + o.z * o.z - m_radius * m_radius;

        float discriminant = b * b - 4 * a * c;
        if (discriminant < 0.f)
            return false;
        auto sqrted = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        float root = (-b - sqrted) / (2 * a);

        if (root <= 0 || root >= tMax) {
            root = (-b + sqrted) / (2 * a);
            if (root <= 0 || root >= tMax)
                return false;
        }

        isect.t = root;
        isect.p = ray.at(root);
        isect.mat = m_mat;
        vec3 temp = (isect.p - m_center) / m_radius;
        isect.normal = dot(temp, ray.direction) < 0 ? temp : -temp;

        // TODO: UV Coordinates
        isect.u = 0.f;
        isect.v = 1.f;

        return true;
    }

private:
    glm::point3 m_center;
    float m_radius;
    std::shared_ptr<Material> m_mat;
    Bounds m_bounding_box;
};


#endif //REAL_TIME_RAY_TRACER_SPHERE_H
