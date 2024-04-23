#include "Core/Scene.h"
#include "Materials/Diffuse.h"
#include "Materials/DiffuseEmissive.h"
#include "Materials/Specular.h"
#include "Materials/Glossy.h"
#include "Shapes/Quad.h"
#include "Samples/CreateScene.h"


Scene::Scene() {
    createTestScene(*this);

    // Build BVH
    m_bvh_tree = std::make_shared<BVH>(m_objects);
}

bool Scene::intersect(const Ray &r, Intersection &isect) const {
    float tMax = float_infinity;
    return m_bvh_tree->intersect(r, tMax, isect);

    bool hit = false;
    float tMax1 = float_infinity;

    for (const auto &object: m_objects) {
        if (object->intersect(r, tMax1, isect)) {
            hit = true;
            tMax1 = isect.t;
        }
    }
    return hit;
}
