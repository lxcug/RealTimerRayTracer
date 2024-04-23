//
// Created by Tencent_GO on 2024/4/23.
//

#include "Sphere.h"

#include <utility>


Sphere::Sphere(float _radius, std::shared_ptr<Material> _mat)
        : m_radius(_radius), m_mat(std::move(_mat)) {}


Sphere::Sphere(const glm::point3 &_center, float _radius, std::shared_ptr<Material> _mat)
        : m_center(_center), m_radius(_radius), m_mat(std::move(_mat)) {}

bool Sphere::intersect(const Ray &ray, float tMax, Intersection &isect) const {
    using namespace glm;

    // To world space
    glm::vec3 center = glm::vec3((m_transform * glm::vec4(m_center, 1.f)));

    vec3 o = ray.origin - center;
    vec3 d = ray.direction;
    float a = 1.f;
    float b = 2 * dot(o, d);
    float c = o.x * o.x + o.y * o.y + o.z * o.z - m_radius * m_radius;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0.f)
        return false;
    auto sqrted = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    float root = (-b - sqrted) / (2 * a);

    if (root <= RayPrimitiveIntersectMinTime || root >= tMax) {
        root = (-b + sqrted) / (2 * a);
        if (root <= RayPrimitiveIntersectMinTime || root >= tMax)
            return false;
    }

    isect.t = root;
    isect.p = ray.at(root);
    isect.mat = m_mat;
    vec3 temp = (isect.p - center) / m_radius;
    // Note: Ignore hit backface
    if (dot(temp, ray.direction) >= 0.f) {
        isect.backFace = true;
        return false;
    }
    isect.normal = temp;

    // TODO: UV Coordinates
    isect.u = 0.f;
    isect.v = 1.f;

    return true;
}
