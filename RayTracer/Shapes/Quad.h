//
// Created by Tencent_GO on 2024/4/23.
//

#ifndef REAL_TIME_RAY_TRACER_QUAD_H
#define REAL_TIME_RAY_TRACER_QUAD_H

#include "Core/RealTimeRT.h"
#include "Primitive.h"
#include "Accelerator/Bounds.h"
#include <iostream>


class Quad : public Primitive {
public:
    explicit Quad(const glm::point3 &q, const glm::vec3 &u = glm::vec3(1.f, 0.f, 0.f),
                  const glm::vec3 &v = glm::vec3(0.f, 1.f, 0.f));

    Quad(const glm::point3 &q, const glm::vec3 &u, const glm::vec3 &v,
         const std::shared_ptr<Material> &mat);

    [[nodiscard]] Bounds boundingBox() const override {
        glm::point3 r = m_qWorld + m_uWorld;
        glm::point3 s = r + m_vWorld;
        glm::point3 t = m_qWorld + m_vWorld;
        auto bounds_diag1 = Bounds(m_qWorld, s);
        auto bounds_diag2 = Bounds(r, t);
        return {bounds_diag1, bounds_diag2};
    }

    bool intersect(const Ray &ray, float tMax, Intersection &isect) const override;

    void setMaterial(const std::shared_ptr<Material> &mat) override {
        m_mat = mat;
    }

    void setTransform(const glm::vec3 &translation, const glm::vec3 &scale = glm::vec3(1.f),
                      const glm::quat &rotation = glm::identity<glm::quat>()) override {
        m_transform = glm::translate(glm::identity<glm::mat4>(), translation);
        m_transform = m_transform * glm::mat4_cast(rotation);
        m_transform = glm::scale(m_transform, scale);
        recalculateParams();
    }

    void recalculateParams();

private:
    // Geometry
    glm::point3 m_q, m_qWorld;
    glm::vec3 m_u, m_v, m_uWorld, m_vWorld;
    glm::vec3 normal;
    Bounds m_boundingBox;
    // Material
    std::shared_ptr<Material> m_mat;
    // For Ray-Quad Intersection
    glm::vec3 w;
    float D;
};

#endif //REAL_TIME_RAY_TRACER_QUAD_H
