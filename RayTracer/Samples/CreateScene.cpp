//
// Created by Tencent_GO on 2024/4/23.
//

#include "CreateScene.h"
#include "Core/Scene.h"


void createTestScene(Scene &scene) {
    using namespace glm;

    std::shared_ptr<Sphere> ground = std::make_shared<Sphere>(vec3(0.f), 99.f,
                                                              std::make_shared<DiffuseMaterial>(
                                                                      random_vec3()));
    ground->setTransform(vec3(0.f, -100.f, 0.f));
    scene.m_objects.push_back(ground);

    std::shared_ptr<DiffuseEmissiveMaterial> emissiveMat =
            std::make_shared<DiffuseEmissiveMaterial>(random_vec3());

    std::shared_ptr<Sphere> emissiveSphere = std::make_shared<Sphere>(glm::vec3(0.f, 10.f, 0.f),
                                                                      4.f, emissiveMat);
//    scene.m_objects.push_back(emissiveSphere);

    std::shared_ptr<Sphere> diffuseSphere1 =
            std::make_shared<Sphere>(glm::vec3(0., -1., 4.), .5,
                                     std::make_shared<DiffuseMaterial>(glm::vec3(.3f, .5f, .3f)));
    scene.m_objects.push_back(diffuseSphere1);

    std::shared_ptr<Sphere> glossySphere =
            std::make_shared<Sphere>(.7, std::make_shared<GlossyMaterial>(random_vec3(), 0.f));
    glossySphere->setTransform(glm::vec3(-2.f, 0.f, 3.f), glm::vec3(2.f));
    scene.m_objects.push_back(glossySphere);

    scene.m_objects.push_back(
            std::make_shared<Sphere>(glm::vec3(0., -1., 5.3), .5,
                                     std::make_shared<DiffuseEmissiveMaterial>(random_vec3())));
    scene.m_objects.push_back(
            std::make_shared<Sphere>(glm::vec3(2.f, 1.f, 2.f), 1.f,
                                     std::make_shared<SpecularMaterial>(random_vec3()))
    );

    std::shared_ptr<Quad> quadLight =
            std::make_shared<Quad>(glm::vec3(-5.f, -1.5f, -2.f), glm::vec3(10.f, 0.f, 0.f),
                                   glm::vec3(0.f, 4.f, 0.f),
                                   emissiveMat);
    scene.addObject(quadLight);

    std::shared_ptr<Quad> mirror =
            std::make_shared<Quad>(glm::vec3(-8.f, 0.f, 6.f),
                                   glm::vec3(0.f, 3.f, 0.f),
                                   glm::vec3(10.f, 0.f, 0.f),
                                   std::make_shared<SpecularMaterial>(random_vec3()));
    mirror->setTransform(glm::vec3(0.f), glm::vec3(1.f),
                         glm::angleAxis(glm::radians(60.f), glm::vec3(0.f, 1.f, 0.)));
    scene.addObject(mirror);
}

void createCornellBox(Scene &scene) {
    std::shared_ptr<DiffuseMaterial> topMat = std::make_shared<DiffuseMaterial>(glm::vec3(1.f));
    std::shared_ptr<DiffuseMaterial> bottomMat = topMat, frontMat = topMat, backMat = topMat;
    std::shared_ptr<DiffuseMaterial> leftMat = std::make_shared<DiffuseMaterial>(
            glm::vec3(1.f, 0.f, 0.f));
    std::shared_ptr<DiffuseMaterial> rightMat = std::make_shared<DiffuseMaterial>(
            glm::vec3(0.f, 1.f, 0.f));

    std::shared_ptr<Quad> topQuad = std::make_shared<Quad>(glm::vec3(0.f),
                                                           glm::vec3(8.f, 0.f, 0.f),
                                                           glm::vec3(0.f, 0.f, 10.f), topMat);
    topQuad->setTransform(glm::vec3(-4.f, 6.f, -5.f));
    std::shared_ptr<Quad> bottomQuad = std::make_shared<Quad>(glm::vec3(0.f),
                                                              glm::vec3(0.f, 0.f, 10.f),
                                                              glm::vec3(8.f, 0.f, 0.f), bottomMat);
    bottomQuad->setTransform(glm::vec3(-4.f, 0.f, -5.f));

    std::shared_ptr<Quad> leftQuad = std::make_shared<Quad>(glm::vec3(0.f),
                                                            glm::vec3(0.f, 6.f, 0.f),
                                                            glm::vec3(0.f, 0.f, 10.f), leftMat);
    leftQuad->setTransform(glm::vec3(-4.f, 0.f, -5.f));
    std::shared_ptr<Quad> rightQuad = std::make_shared<Quad>(glm::vec3(0.f),
                                                             glm::vec3(0.f, 0.f, 10.f),
                                                             glm::vec3(0.f, 6.f, 0.f), rightMat);
    rightQuad->setTransform(glm::vec3(4.f, 0.f, -5.f));

    std::shared_ptr<Quad> backQuad = std::make_shared<Quad>(glm::vec3(0.f),
                                                            glm::vec3(8.f, 0.f, 0.f),
                                                            glm::vec3(0.f, 6.f, 0.f), backMat);
    backQuad->setTransform(glm::vec3(-4.f, 0.f, -5.f));
    std::shared_ptr<Quad> frontQuad = std::make_shared<Quad>(glm::vec3(0.f),
                                                             glm::vec3(0.f, 6.f, 0.f),
                                                             glm::vec3(8.f, 0.f, 0.f), frontMat);
    frontQuad->setTransform(glm::vec3(-4.f, 0.f, 5.f));

    std::shared_ptr<Quad> topAreaLight =
            std::make_shared<Quad>(glm::vec3(-1.f, 5.99f, -1.f), glm::vec3(2.f, 0.f, 0.f),
                                   glm::vec3(0.f, 0.f, 2.f),
                                   std::make_shared<DiffuseEmissiveMaterial>(random_vec3(), 5.f));

    scene.addObject(topQuad);
    scene.addObject(bottomQuad);
    scene.addObject(leftQuad);
    scene.addObject(rightQuad);
    scene.addObject(backQuad);
//    scene.addObject(frontQuad);
    scene.addObject(topAreaLight);

    scene.m_objects.push_back(
            std::make_shared<Sphere>(glm::vec3(-1.f, .5f, -3.f), .5f,
                                     std::make_shared<DiffuseMaterial>(random_vec3())));
//    scene.m_objects.push_back(
//            std::make_shared<Sphere>(glm::vec3(1.f, 3.f, -2.f), .5f,
//                                     std::make_shared<GlossyMaterial>(random_vec3(),
//                                                                      0.f)));
    scene.m_objects.push_back(
            std::make_shared<Sphere>(glm::vec3(0.f, 2.f, -2.f), .5f,
                                     std::make_shared<SpecularMaterial>(random_vec3())));
    scene.m_objects.push_back(
            std::make_shared<Sphere>(glm::vec3(3.f, .4f, -3.f), .4f,
                                     std::make_shared<SpecularMaterial>(random_vec3())));
    scene.m_objects.push_back(
            std::make_shared<Sphere>(glm::vec3(1.f, .5f, -1.f), .5f,
                                     std::make_shared<DiffuseMaterial>(random_vec3())));
}
