//
// Created by Tencent_GO on 2024/4/23.
//

#include "Quad.h"

Quad::Quad(const glm::point3 &q, const glm::vec3 &u, const glm::vec3 &v)
        : m_q(q), m_u(u), m_v(v) {
    recalculateParams();
}

Quad::Quad(const glm::point3 &q, const glm::vec3 &u, const glm::vec3 &v,
           const std::shared_ptr<Material> &mat) : m_q(q), m_u(u), m_v(v), m_mat(mat) {
    recalculateParams();
}

bool Quad::intersect(const Ray &ray, float tMax, Intersection &isect) const {
    float denom = glm::dot(normal, ray.direction);
    if (denom > 0.f) {
        isect.backFace = true;
    }

    if (std::abs(denom) < 1e-8)
        return false;

    float t = (D - glm::dot(normal, ray.origin)) / denom;
    if (t < RayPrimitiveIntersectMinTime || t > tMax)
        return false;

    glm::point3 isect_point = ray.at(t);
    glm::vec3 planar_hitpt_vector = isect_point - m_qWorld;
    float alpha = glm::dot(w, glm::cross(planar_hitpt_vector, m_vWorld));
    float beta = glm::dot(w, glm::cross(m_uWorld, planar_hitpt_vector));
    if (alpha < 0.f || alpha > 1.f || beta < 0.f || beta > 1.f)
        return false;

    isect.t = t;
    isect.p = isect_point;
    isect.mat = m_mat;
    isect.normal = normal;

    isect.u = alpha;
    isect.v = beta;

    return true;
}

void Quad::recalculateParams() {
    m_uWorld = m_transform * glm::vec4(m_u, 0.f);
    m_vWorld = m_transform * glm::vec4(m_v, 0.f);
    auto n = glm::cross(m_uWorld, m_vWorld);
    m_qWorld = glm::vec3(m_transform * glm::vec4(m_q, 1.f));
    normal = glm::normalize(n);
    D = glm::dot(normal, m_qWorld);
    w = n / glm::dot(n, n);
}
