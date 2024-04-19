// @author: lx
// @time: 2024/1/11
// @brief: 

#ifndef REAL_TIME_RAY_TRACER_CAMERA_H
#define REAL_TIME_RAY_TRACER_CAMERA_H

#include "Core/RealTimeRT.h"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>


class Camera {
public:
    Camera(float vfov, float nearClip, float farClip);

    bool onUpdate(float ts);

    void onResize(uint32_t width, uint32_t height);

    [[nodiscard]] const glm::mat4 &getProjection() const { return m_projection; }

    [[nodiscard]] const glm::mat4 &getInverseProjection() const { return m_inv_projection; }

    [[nodiscard]] const glm::mat4 &getView() const { return m_view; }

    [[nodiscard]] const glm::mat4 &getInverseView() const { return m_inv_view; }

    [[nodiscard]] const glm::vec3 &getPosition() const { return m_position; }

    [[nodiscard]] const glm::vec3 &getDirection() const { return m_forward_direction; }

    [[nodiscard]] const std::vector<glm::vec3> &getRayDirections() const { return m_ray_directions; }

    glm::vec3 calSuperSamplingOffset(uint32_t index) const;

    void setSpp(int spp) {
        if (m_spp != spp) {
            m_spp = spp;
            recalculateRayDirections();
        }
    }

private:
    void recalculateView();

    void recalculateProjection();

    void recalculateRayDirections();

    [[nodiscard]] float getRotationSpeed() { return .2f; }

    [[nodiscard]] float getMoveSpeed() { return 4.f; }

private:
    glm::mat4 m_view{glm::identity<glm::mat4>()}, m_projection{glm::identity<glm::mat4>()};
    glm::mat4 m_inv_view{glm::identity<glm::mat4>()}, m_inv_projection{glm::identity<glm::mat4>()};

    float m_vertical_fov{45.f};
    float m_near_clip{.1f}, m_far_clip{1000.};

    glm::vec3 m_position{glm::vec3(0., 5., 12.f)};
    glm::vec3 m_forward_direction{glm::vec3(0.f, -.3f, -1.f)};

    // Note: cached ray directions
    std::vector<glm::vec3> m_ray_directions;

    glm::vec2 m_LastMousePosition{0.0f, 0.0f};

    uint32_t m_viewport_width = 1920, m_viewport_height = 1080;

    float m_dx, m_dy;

    int m_spp = 1;
};


#endif //REAL_TIME_RAY_TRACER_CAMERA_H
