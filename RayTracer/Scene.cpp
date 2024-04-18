#include "Scene.h"
#include "Materials/Diffuse.h"
#include "Materials/DiffuseEmissive.h"
#include "Materials/Specular.h"


Scene::Scene() {
    m_objects.push_back(
            std::make_shared<Sphere>(glm::vec3(0., -100., 0.), 99.,
                                     std::make_shared<DiffuseMaterial>(glm::vec3(.3f, .5f, .3f))));

    m_objects.push_back(
            std::make_shared<Sphere>(glm::vec3(0.f, 10.f,  0.f), 4.,
                                     std::make_shared<DiffuseEmissiveMaterial>(random_vec3())));
//    m_objects.push_back(
//            std::make_shared<Sphere>(glm::vec3(-3.f, 0.f,  3.f), 2.,
//                                     std::make_shared<DiffuseEmissiveMaterial>(random_vec3())));

    for (int x = -2; x <= 2; x++) {
        for (int z = -2; z <= 2; z++) {
            m_objects.push_back(
                    std::make_shared<Sphere>(glm::vec3(x, 2., z), .4f,
                                             std::make_shared<DiffuseMaterial>(random_vec3())));
        }
    }

//    m_objects.push_back(
//            std::make_shared<Sphere>(glm::vec3(0.f, 1.f, 3.f), 1.f,
//                                     std::make_shared<SpecularMaterial>(random_vec3()))
//            );
//    m_objects.push_back(
//            std::make_shared<Sphere>(glm::vec3(3.f, 1.f, 3.f), 1.f,
//                                     std::make_shared<SpecularMaterial>(random_vec3()))
//    );

    m_objects.push_back(
            std::make_shared<Sphere>(glm::vec3(0., 0., 5.), .5,
                                     std::make_shared<DiffuseMaterial>(random_vec3())));

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
