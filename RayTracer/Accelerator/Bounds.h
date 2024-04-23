// @author: lx
// @time: 2024/1/13
// @brief: 

#ifndef REAL_TIME_RAY_TRACER_BOUNDS_H
#define REAL_TIME_RAY_TRACER_BOUNDS_H

#include "Core/RealTimeRT.h"
#include "Core/Ray.h"
#include "Core/Intersection.h"
#include <array>


class Bounds {
public:
    Bounds() : m_min(glm::point3(float_max)), m_max(glm::point3(float_min)) {}

    Bounds(const glm::point3 &p1, const glm::point3 &p2) : m_min(glm::min(p1, p2)),
                                                           m_max(glm::max(p1, p2)) {
        float delta = 1e-3;
        for (int i = 0; i < 3; i++) {
            if (m_max[i] - m_min[i] < delta) {
                expand(i, delta);
            }
        }
    }

    Bounds(const Bounds &b1, const Bounds &b2) : m_min(glm::min(b1.m_min, b2.m_min)),
                                                 m_max(glm::max(b1.m_max, b2.m_max)) {}

    void expand(int dim, float delta) {
        m_min[dim] -= delta;
        m_max[dim] += delta;
    }

    bool intersect(const Ray &r);

    bool intersect(const glm::vec3 &o, const glm::vec3 &invDir, float rayMax,
                   const std::array<bool, 3> &dirIsNeg);

    bool intersect(const glm::vec3 &o, const glm::vec3 &invDir, float rayMax, int *dirIsNeg);

    [[nodiscard]] glm::vec3 diagonal() const {
        return m_max - m_min;
    }

    [[nodiscard]] glm::point3 middle() const {
        return (m_min + m_max) * .5f;
    }

    Bounds expand(float delta = 1e-4f);

    [[nodiscard]] int maxDim() const;

    [[nodiscard]] bool isEmpty() const;

    const glm::point3 &operator[](int index) const {
        assert(index == 0 || index == 1);

        return index == 0 ? m_min : m_max;
    }

    void unionBounds(const Bounds& b2);

    void unionPoint(const glm::vec3& p);

private:
    glm::point3 m_min, m_max;
};


#endif //REAL_TIME_RAY_TRACER_BOUNDS_H
