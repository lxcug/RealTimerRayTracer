// @author: lx
// @time: 2024/1/11
// @brief: 

#ifndef REAL_TIME_RAY_TRACER_PRIMITIVE_H
#define REAL_TIME_RAY_TRACER_PRIMITIVE_H

#include "Core/RealTimeRT.h"
#include "Core/Ray.h"
#include "Core/Intersection.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/quaternion.hpp>


class Bounds;

class Primitive {
public:
    virtual bool intersect(const Ray &ray, float tMax, Intersection &isect) const = 0;

    [[nodiscard]] virtual bool intersectP(const Ray &r, float tMax) const { return false; }

    [[nodiscard]] virtual Bounds boundingBox() const = 0;

    virtual void setMaterial(const std::shared_ptr<Material> &mat) {}

    [[nodiscard]] virtual const glm::mat4 &getTransform() const {
        return m_transform;
    }

    void setTransform(const glm::mat4 &transform) {
        m_transform = transform;
    }

    virtual void setTransform(const glm::vec3 &translation, const glm::vec3 &scale = glm::vec3(1.f),
                              const glm::quat &rotation = glm::identity<glm::quat>()) {
        m_transform = glm::translate(glm::identity<glm::mat4>(), translation);
        m_transform = m_transform * glm::mat4_cast(rotation);
        m_transform = glm::scale(m_transform, scale);
    }

protected:
    glm::mat4 m_transform{1.f};
};


#endif //REAL_TIME_RAY_TRACER_PRIMITIVE_H
