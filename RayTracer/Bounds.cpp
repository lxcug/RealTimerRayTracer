#include "Bounds.h"


bool Bounds::intersect(const Ray &r) {
    int dirIsNeg[3] = {r.direction.x < 0., r.direction.y < 0.,
                       r.direction.z < 0.};

    return intersect(r.origin, 1.f / r.direction, float_infinity, dirIsNeg);
}

bool Bounds::intersect(const glm::vec3 &o, const glm::vec3 &invDir, float rayMax, int *dirIsNeg) {
    const Bounds &bounds = *this;
    float tMin = (bounds[dirIsNeg[0]].x - o.x) * invDir.x;
    float tMax = (bounds[1 - dirIsNeg[0]].x - o.x) * invDir.x;

    float tYMin = (bounds[dirIsNeg[1]].y - o.y) * invDir.y;
    float tYMax = (bounds[1 - dirIsNeg[1]].y - o.y) * invDir.y;

    if (tMin > tYMax || tYMin > tMax)
        return false;
    if (tYMin > tMin)
        tMin = tYMin;
    if (tYMax < tMax)
        tMax = tYMax;

    float tZMin = (bounds[dirIsNeg[2]].z - o.z) * invDir.z;
    float tZMax = (bounds[1 - dirIsNeg[2]].z - o.z) * invDir.z;

    if (tMin > tZMax)
        return false;
    if (tZMin > tMin)
        tMin = tZMin;
    if (tZMax < tMax)
        tMax = tZMax;

    return tMin < rayMax && tMax > 0;
}

bool Bounds::intersect(const glm::vec3 &o, const glm::vec3 &invDir, float rayMax,
                       const std::array<bool, 3> &dirIsNeg) {
    const Bounds &bounds = *this;
    float tMin = (bounds[dirIsNeg[0]].x - o.x) * invDir.x;
    float tMax = (bounds[1 - dirIsNeg[0]].x - o.x) * invDir.x;

    float tYMin = (bounds[dirIsNeg[1]].y - o.y) * invDir.y;
    float tYMax = (bounds[1 - dirIsNeg[1]].y - o.y) * invDir.y;

    if (tMin > tYMax || tYMin > tMax)
        return false;
    if (tYMin > tMin)
        tMin = tYMin;
    if (tYMax < tMax)
        tMax = tYMax;

    float tZMin = (bounds[dirIsNeg[2]].z - o.z) * invDir.z;
    float tZMax = (bounds[1 - dirIsNeg[2]].z - o.z) * invDir.z;

    if (tMin > tZMax)
        return false;
    if (tZMin > tMin)
        tMin = tZMin;
    if (tZMax < tMax)
        tMax = tZMax;

    return tMax > 0;
}

Bounds Bounds::expand(float delta) {
    Bounds ret;
    ret.m_min = this->m_min - delta;
    ret.m_max = this->m_max + delta;

    return ret;
}

int Bounds::maxDim() const {
    auto diag = diagonal();
    if (diag.x > diag.y && diag.x > diag.z)
        return 0;
    else if (diag.y > diag.z)
        return 1;
    else
        return 2;
}

bool Bounds::isEmpty() const {
    return m_min.x >= m_max.x || m_min.y >= m_max.y || m_min.z >= m_max.z;
}

void Bounds::unionBounds(const Bounds &b2) {
    this->m_min = glm::min(this->m_min, b2.m_min);
    this->m_max = glm::max(this->m_max, b2.m_max);
}

void Bounds::unionPoint(const glm::vec3 &p) {
    this->m_min = glm::min(this->m_min, p);
    this->m_max = glm::max(this->m_max, p);
}
