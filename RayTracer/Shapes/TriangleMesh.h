//
// Created by Tencent_GO on 2024/4/23.
//

#ifndef REAL_TIME_RAY_TRACER_TRIANGLEMESH_H
#define REAL_TIME_RAY_TRACER_TRIANGLEMESH_H

#include "Core/RealTimeRT.h"
#include "Primitive.h"
#include "Accelerator/Bounds.h"
#include <vector>


class TriangleMesh : public Primitive {
public:
    TriangleMesh(const std::vector<glm::point3> &vertices, const std::vector<int> &indices,
                 const std::vector<glm::vec3> &normals, const std::vector<glm::vec3> &tangents,
                 const std::vector<glm::vec2> &uvs);

    [[nodiscard]] Bounds boundingBox() const override {
        return m_boundingBox;
    }

    bool intersect(const Ray &ray, float tMax, Intersection &isect) const override;

    void setMaterial(const std::shared_ptr<Material> &mat) override {
        m_mat = mat;
    }


private:
    // Geometry
    std::vector<glm::point3> m_vertices;
    std::vector<glm::vec2> m_uvs;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec3> m_tangents;
    std::vector<int> m_indices;
    Bounds m_boundingBox;
    // Material
    std::shared_ptr<Material> m_mat;
};


#endif //REAL_TIME_RAY_TRACER_TRIANGLEMESH_H
