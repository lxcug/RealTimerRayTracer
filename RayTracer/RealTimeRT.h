// @author: lx
// @time: 2024/1/11
// @brief: 

#ifndef REAL_TIME_RAY_TRACER_REALTIMERT_H
#define REAL_TIME_RAY_TRACER_REALTIMERT_H

#include "glm/glm.hpp"
#include <random>


namespace glm {
    using point3 = vec3;
}

const float float_infinity = std::numeric_limits<float>::infinity();
const float float_max = std::numeric_limits<float>::max();
const float float_min = std::numeric_limits<float>::min();
const float pi = 3.1415926535897932385;
const float inv_pi = 1.f / pi;


inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline uint32_t PCG_hash(uint32_t input) {
    uint32_t state = input * 747796405u + 2891336453u;
    uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

inline float radicalInverse(uint32_t bits) {
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555) << 1u) | ((bits & 0xAAAAAAAA) >> 1u);
    bits = ((bits & 0x33333333) << 2u) | ((bits & 0xCCCCCCCC) >> 2u);
    bits = ((bits & 0x0F0F0F0F) << 4u) | ((bits & 0xF0F0F0F0) >> 4u);
    bits = ((bits & 0x00FF00FF) << 8u) | ((bits & 0xFF00FF00) >> 8u);
    return (float) (bits) * 2.3283064365386963e-10f;
}

inline glm::vec2 Hammersley(uint32_t i, uint32_t N) {
    return {(float) i / (float) N, radicalInverse(i)};
}

inline float random_float(float min = 0., float max = 1.) {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return min + distribution(generator) * (max - min);
}

inline float random_float(uint32_t &seed) {
    seed = PCG_hash(seed);
    return (float) seed / (float) std::numeric_limits<float>::max();
}

inline int random_int(int min = 0, int max = 1) {
    return static_cast<int>(random_float(min, max + 1));
}

inline glm::mat3 buildCoord(const glm::vec3 &normal) {
    glm::vec3 x, y, z = normal;

    if (abs(z.x) < .57735027f)
        x = glm::vec3(1.f, 0.f, 0.f);
    else
        x = glm::vec3(0.f, 0.f, 1.f);

    x = normalize(x - dot(x, z) * z);
    y = cross(z, x);

    glm::mat3 ret(x, y, z);
    return ret;
}

inline glm::vec3 cos_weighted_hemisphere_sampling(const glm::vec3 &normal) {
    float xi1 = random_float(), xi2 = random_float();

    // Refer: PBRT
//    xi1 = xi1 * 2.f - 1.f;
//    xi2 = xi2 * 2.f - 1.f;
//    glm::vec2 point;
//    if (xi1 == 0 && xi2 == 0)
//        point = glm::vec2(0., 0.);
//    float theta, r;
//    if (std::abs(xi1) > std::abs(xi2)) {
//        r = xi1;
//        theta = pi / 4.f * (xi2 / xi1);
//    } else {
//        r = xi2;
//        theta = pi / 2.f - pi / 4. * (xi1 / xi2);
//    }
//    point = r * glm::vec2(cos(theta), sin(theta));
//    float z = sqrt(std::max(0.f, 1 - point.x * point.x - point.y * point.y));
//    glm::vec3 local_dir = glm::vec3(point, z);

    float theta = std::acos(sqrt(1.f - xi1));
    float phi = 2.f * pi * xi2;
    float sin_theta = sin(theta);
    glm::vec3 local_dir(
            sin_theta * cos(phi),
            sin_theta * sin(phi),
            cos(theta)
    );

    return buildCoord(normal) * local_dir;
}

inline glm::vec3 uniform_on_hemisphere(const glm::vec3 &normal) {
    float xi1 = random_float(), xi2 = random_float();
    float sqrted = std::sqrt(1.f - xi1 * xi1);

    glm::vec3 local_dir(
            std::cos(2.f * pi * xi2) * sqrted,
            std::sin(2.f * pi * xi2) * sqrted,
            xi1
    );

    return dot(local_dir, normal) > 0.f ? local_dir : -local_dir;
}

inline glm::vec3 random_vec3(float min = 0., float max = 1.) {
    return {random_float(min, max), random_float(min, max), random_float(min, max)};
}

inline glm::vec3 random_vec3(uint32_t &seed) {
    return {random_float(seed) * 2.f - 1.f, random_float(seed) * 2.f - 1.f, random_float(seed) * 2.f - 1.f};
}

inline glm::vec3 random_in_unit_sphere() {
    while (true) {
        auto p = random_vec3(-1, 1);
        if (glm::length(p) * glm::length(p) < 1)
            return p;
    }
}

inline glm::vec3 uniform_on_unit_sphere() {
    float xi1 = random_float(), xi2 = random_float();

    float z = 1 - 2 * xi1;
    float r = std::sqrt(std::max(0.f, 1.f - z * z));
    float phi = 2.f * pi * xi2;
    return {r * std::cos(phi), r * std::sin(phi), z};
}

#endif //REAL_TIME_RAY_TRACER_REALTIMERT_H
