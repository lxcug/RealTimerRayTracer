//
// Created by Tencent_GO on 2024/4/18.
//

#ifndef REAL_TIME_RAY_TRACER_SAMPLEUTILS_H
#define REAL_TIME_RAY_TRACER_SAMPLEUTILS_H

#include "Core/RealTimeRT.h"


namespace SampleUtils {

    glm::mat3 buildCoordSystem(const glm::vec3 &normal) {
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

    glm::vec2
    sampleUniformDiskPolar(const glm::vec2 &u = glm::vec2(random_float(), random_float())) {
        float r = std::sqrt(u.x);
        float theta = 2 * pi * u.y;

        return {
                r * std::cos(theta),
                r * std::sin(theta)
        };
    }

    glm::vec2
    sampleUniformDiskConcentric(const glm::vec2 &u = glm::vec2(random_float(), random_float())) {
        // TODO
    }

    glm::vec3 sampleCosineHemisphere(const glm::vec3 &normal) {
        float xi1 = random_float(), xi2 = random_float();

        float theta = std::acos(sqrt(1.f - xi1));
        float phi = 2.f * pi * xi2;
        float sin_theta = std::sin(theta);
        glm::vec3 local_dir(
                sin_theta * cos(phi),
                sin_theta * sin(phi),
                cos(theta)
        );

        if (glm::dot(local_dir, normal) < 0.f)
            return -local_dir;
        else
            return local_dir;
    }

    /*!
     * Transfer spherical direction into Cartesian coordinate
     * @param dir Dir must be normalized
     * @param theta Out parameter zenith
     * @param phi Out parameter azimuth
     */
    void coord2Spherical(const glm::vec3 &dir, float &theta, float &phi) {
        theta = std::acos(dir.z);

        phi = std::atan2(dir.y, dir.x);

        if (phi < 0.f) {
            phi += 2.f * pi;
        }
    }


    namespace Microfacet {
#if RENDER_MODE_REAL_TIME

        /*!
         *
         * @param wm The normal of microfacet
         * @param normal The marco normal of the surface
         * @param roughness
         * @return The relative differential area of microfacets with surface normal wm
         */
        float D_GGX(const glm::vec3 &wm, const glm::vec3 &normal, float roughness) {
            glm::mat3 worldToNormal = buildCoordSystem(normal);
            glm::vec3 wm_normal = worldToNormal * wm;

            float theta, phi;
            coord2Spherical(wm_normal, theta, phi);
            float sinTheta = std::sin(theta), cosTheta = std::cos(theta);
            float cos2Theta = cosTheta * cosTheta;

            float tan2theta = sinTheta * sinTheta / cos2Theta;
            float cos4Theta = cos2Theta * cos2Theta;
            float alpha2 = roughness * roughness;
            float denom = pi * alpha2 * cos4Theta * (1.f + tan2theta / alpha2);

            return 1.f / denom;
        }

        /*!
         * Helper function for calculating G1_Smith
         * @param theta The azimuth of the view direction
         * @param roughness
         * @return
         */
        float Lambda(float theta, float roughness) {
            float tan2Theta = std::pow(std::tan(theta), 2.f);
            float alpha2 = roughness * roughness;
            float nom = std::sqrt(1 + alpha2 * tan2Theta) - 1.f;

            return nom / 2.f;
        }

        /*!
         *
         * @param w View direction
         * @param normal The marco normal of the surface
         * @param roughness
         * @return The fraction of microfacets that are visible from w
         */
        float G1_Smith(const glm::vec3 &w, const glm::vec3 &normal, float roughness) {
            glm::mat3 worldToNormal = buildCoordSystem(normal);
            glm::vec3 w_normal = worldToNormal * w;

            float theta = std::acos(w_normal.z);

            return 1.f / (1.f + Lambda(theta, roughness));
        }

        /*!
         *
         * @param wi Light incident direction
         * @param wo View direction
         * @param normal The marco normal of the surface
         * @param roughness
         * @return
         */
        float
        G_Smith(const glm::vec3 &wi, const glm::vec3 &wo, const glm::vec3 &normal,
                float roughness) {
            glm::mat3 worldToNormal = buildCoordSystem(normal);
            glm::vec3 wi_normal = worldToNormal * wi;
            glm::vec3 wo_normal = worldToNormal * wo;
            float theta_i = std::acos(wi_normal.z), theta_o = std::acos(wo_normal.z);

            return 1.f / (1.f + Lambda(theta_i, roughness) + Lambda(theta_o, roughness));
        }

        /*!
         *
         * @param w View direction
         * @param wm The normal of microfacet
         * @param normal The marco normal of the surface
         * @param roughness
         * @return
         */
        float D(const glm::vec3 &w, const glm::vec3 &wm, const glm::vec3 &normal, float roughness) {
            float cosTheta = glm::dot(w, normal);
            float G1 = G1_Smith(w, normal, roughness);
            float D_ggx = D_GGX(wm, normal, roughness);

            return G1 / cosTheta * D_ggx * std::max(0.f, glm::dot(w, wm));
        }

        /*!
         *
         * @param w View direction
         * @param wm The normal of microfacet
         * @param normal The marco normal of the surface
         * @param roughness
         * @return The probability density of wm against a surface normal from the view direction w
         */
        float
        PDF(const glm::vec3 &w, const glm::vec3 &wm, const glm::vec3 &normal, float roughness) {
            return D(w, wm, normal, roughness);
        }

        /*!
         *
         * @param wo View direction
         * @param normal The marco normal of the surface
         * @param roughness
         * @return Sampled incident direction
         */
        glm::vec3 sampleMicrofacet(const glm::vec3 &wo, const glm::vec3 &normal, float roughness) {
            // Transform all directions to Normal Coord System
            glm::mat3 world2Normal = buildCoordSystem(normal);
            glm::vec3 wo_normal = world2Normal * wo;

            // Normalize alpha * wo
            glm::vec3 woh_normal = glm::normalize(roughness * wo_normal);
            // Transform all directions to View Coord System(the z-axis is woh)
            glm::mat3 normal2View = buildCoordSystem(woh_normal);
            glm::vec3 T1 = normal2View[0], T2 = normal2View[2];

            glm::vec2 p = sampleUniformDiskPolar();
            float h = std::sqrt(1 - p.x * p.x);
            p.y = lerp((1 + woh_normal.z) / 2.f, h, p.y);
            float pz = std::sqrt(std::max(0.f, 1 - glm::length(p)));
            glm::vec3 nh = normal2View * glm::vec3(p, pz);

            glm::vec3 wi_normal = glm::normalize(glm::vec3(
                    roughness * nh.x,
                    roughness * nh.y,
                    std::max(1e-6f, nh.z)
            ));

            glm::vec3 wi_world = glm::inverse(world2Normal) * wi_normal;

            return wi_world;
        }

#else
        float D_GGX(const glm::vec3 &normal, float roughness_x, float roughness_y) {
        float theta, phi;
        coord2Spherical(normal, theta, phi);
        float sinTheta = std::sin(theta), cosTheta = std::cos(theta);
        float cos2Theta = cosTheta * cosTheta;

        float tan2theta = sinTheta * sinTheta / cos2Theta;
        float cos4Theta = cos2Theta * cos2Theta;
        float alpha_x2 = roughness_x * roughness_x;
        float alpha_y2 = roughness_y * roughness_y;
        float cos2Phi = std::pow(std::cos(phi), 2.f);
        float sin2Phi = std::pow(std::sin(phi), 2.f);

        float denom = pi * roughness_x * roughness_y * cos4Theta *
                      (1.f + tan2theta * (cos2Phi / alpha_x2 + sin2Phi / alpha_y2));

        return 1.f / denom;
    }
#endif
    }

}


#endif //REAL_TIME_RAY_TRACER_SAMPLEUTILS_H
