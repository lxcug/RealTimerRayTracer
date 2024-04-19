// @author: lx
// @time: 2024/1/11
// @brief: 

#ifndef REAL_TIME_RAY_TRACER_REALTIMERT_H
#define REAL_TIME_RAY_TRACER_REALTIMERT_H

#include "glm/glm.hpp"
#include <random>


#define RENDER_MODE_REAL_TIME 1
#if RENDER_MODE_REAL_TIME
#define RENDER_MODE_OFFLINE 0
#else
#define RENDER_MODE_OFFLINE 1
#endif


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

inline glm::vec3 random_vec3(float min = 0., float max = 1.) {
    return {random_float(min, max), random_float(min, max), random_float(min, max)};
}

inline glm::vec3 random_vec3(uint32_t &seed) {
    return {random_float(seed) * 2.f - 1.f, random_float(seed) * 2.f - 1.f,
            random_float(seed) * 2.f - 1.f};
}

template<class T>
T lerp(const T &t1, const T &t2, float t) {
    return {t1 * (1 - t) + t2 * t};
}

#endif //REAL_TIME_RAY_TRACER_REALTIMERT_H
