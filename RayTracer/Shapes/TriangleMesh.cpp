//
// Created by Tencent_GO on 2024/4/23.
//

#include "TriangleMesh.h"


TriangleMesh::TriangleMesh(const std::vector<glm::point3> &vertices,
                           const std::vector<int> &indices, const std::vector<glm::vec3> &normals,
                           const std::vector<glm::vec3> &tangents,
                           const std::vector<glm::vec2> &uvs) {

}

bool TriangleMesh::intersect(const Ray &ray, float tMax, Intersection &isect) const {

    return false;
}
