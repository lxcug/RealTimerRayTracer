#include "Core/Camera.h"
#include "Walnut/Input/Input.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


Camera::Camera(float vfov, float nearClip, float farClip)
        : m_vertical_fov(vfov), m_near_clip(nearClip), m_far_clip(farClip) {
    recalculateView();
    recalculateProjection();
    recalculateRayDirections();
}

bool Camera::onUpdate(float ts) {
    using namespace Walnut;

    glm::vec2 mousePos = Input::GetMousePosition();
    glm::vec2 delta = (mousePos - m_LastMousePosition) * 0.002f;
    m_LastMousePosition = mousePos;

    if (!Input::IsMouseButtonDown(MouseButton::Right)) {
        Input::SetCursorMode(CursorMode::Normal);
        return false;
    }

    Input::SetCursorMode(CursorMode::Locked);

    bool moved = false;

    constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
    glm::vec3 rightDirection = glm::cross(m_forward_direction, upDirection);

    float speed = getMoveSpeed();

    // Movement
    if (Input::IsKeyDown(KeyCode::W)) {
        m_position += m_forward_direction * speed * ts;
        moved = true;
    } else if (Input::IsKeyDown(KeyCode::S)) {
        m_position -= m_forward_direction * speed * ts;
        moved = true;
    }
    if (Input::IsKeyDown(KeyCode::A)) {
        m_position -= rightDirection * speed * ts;
        moved = true;
    } else if (Input::IsKeyDown(KeyCode::D)) {
        m_position += rightDirection * speed * ts;
        moved = true;
    }
    if (Input::IsKeyDown(KeyCode::Q)) {
        m_position -= upDirection * speed * ts;
        moved = true;
    } else if (Input::IsKeyDown(KeyCode::E)) {
        m_position += upDirection * speed * ts;
        moved = true;
    }

    // Rotation
    if (delta.x != 0.0f || delta.y != 0.0f) {
        float pitchDelta = delta.y * getRotationSpeed();
        float yawDelta = delta.x * getRotationSpeed();

        glm::quat q = glm::normalize(glm::angleAxis(-pitchDelta, rightDirection) *
                                     glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f)));
        m_forward_direction = glm::rotate(q, m_forward_direction);

        moved = true;
    }

    if (moved) {
        recalculateView();
        recalculateRayDirections();
    }

    return moved;
}

void Camera::onResize(uint32_t width, uint32_t height) {
    if (width == m_viewport_width && height == m_viewport_height)
        return;

    m_viewport_width = width;
    m_viewport_height = height;

    m_dx = 2.f / (float) m_viewport_width;
    m_dy = 2.f / (float) m_viewport_height;

    recalculateProjection();
    recalculateRayDirections();
}

void Camera::recalculateView() {
    m_view = glm::lookAt(m_position, m_forward_direction + m_position, glm::vec3(0.f, 1.f, 0.f));
    m_inv_view = glm::inverse(m_view);
}

void Camera::recalculateProjection() {
    m_projection = glm::perspective(glm::radians(m_vertical_fov), m_viewport_width / (float) m_viewport_height,
                                    m_near_clip, m_far_clip);
    m_inv_projection = glm::inverse(m_projection);
}

void Camera::recalculateRayDirections() {
    m_ray_directions.resize(m_viewport_width * m_viewport_height * m_spp);

    uint32_t cnt = 0;
    for (uint32_t y = 0; y < m_viewport_height; y++) {
        for (uint32_t x = 0; x < m_viewport_width; x++) {
            for (int s = 0; s < m_spp; s++) {
                glm::vec2 coord = {(float) x / (float) m_viewport_width, (float) y / (float) m_viewport_height};
                coord = coord * 2.0f - 1.0f;

                // Note: the z of target can be any value in [-1, 1], since if two target has the same x and y, they are
                //  on the same ray(shoot from camera), in other words, the points on the same ray in view space will be
                //  projected on the same pixel.
                glm::vec4 target = m_inv_projection *
                                   (glm::vec4(coord.x + random_float() * m_dx, coord.y + random_float() * m_dy, 0.,
                                              1.));
                glm::vec3 rayDirection = glm::vec3(
                        m_inv_view * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0));
//            std::cout << rayDirection.x << " " << rayDirection.y << " " << rayDirection.z << std::endl;
                m_ray_directions[cnt++] = rayDirection;
            }
        }
    }
}

glm::vec3 Camera::calSuperSamplingOffset(uint32_t index) const {
    const glm::vec3 &cached_ray = m_ray_directions[index];
    glm::vec4 project_offset = m_inv_projection * glm::vec4(random_float() * m_dx, random_float() * m_dy, 0.f, 1.f);
    project_offset /= project_offset.w;
    return cached_ray + glm::vec3(project_offset);
}
