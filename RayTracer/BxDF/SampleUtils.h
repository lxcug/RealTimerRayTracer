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

        return {x, y, z};
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

        return {};
    }

    glm::vec3 sampleCosineHemisphere(const glm::vec3 &normal) {
        float xi1 = random_float(), xi2 = random_float();

        float cosTheta = std::sqrt(xi1);
        float phi = 2.f * pi * xi2;
        float sinTheta = std::sqrt(1.f - cosTheta * cosTheta);

        glm::vec3 localDir(
                sinTheta * cos(phi),
                sinTheta * sin(phi),
                cosTheta
        );

        return buildCoordSystem(normal) * localDir;
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


    // Refer: PBRT 9.6 Roughness Using Microfacet Theory
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
            float cosTheta_m = glm::dot(wm, normal);
            float sinTheta_m = std::sqrt(1 - cosTheta_m * cosTheta_m);
            float cos2Theta_m = cosTheta_m * cosTheta_m;
            float tan2Theta_m = sinTheta_m * sinTheta_m / cos2Theta_m;
            float alpha2 = roughness * roughness;
            float cos4Theta_m = cos2Theta_m * cos2Theta_m;

            float denom = pi * alpha2 * cos4Theta_m * pow((1.f + tan2Theta_m / alpha2), 2.f);

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
            if (isInf(tan2Theta))
                return 0.f;

            float alpha2 = roughness * roughness;
            float nom = std::sqrt(1 + alpha2 * tan2Theta) - 1.f;

            return nom / 2.f;
        }

        /*!
         *
         * @param w View direction or Light Incident direction
         * @param normal The marco normal of the surface
         * @param roughness
         * @return The fraction of microfacets that are visible from w
         */
        float G1_Smith(const glm::vec3 &w, const glm::vec3 &normal, float roughness) {
            float theta = std::acos(glm::dot(w, normal));

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
            float theta_i = std::acos(glm::dot(wi, normal));
            float theta_o = std::acos(glm::dot(wo, normal));

            return 1.f / (1.f + Lambda(theta_i, roughness) + Lambda(theta_o, roughness));
        }

        /*!
         *
         * @param wo View direction
         * @param wm The normal of microfacet
         * @param normal The marco normal of the surface
         * @param roughness
         * @return
         */
        float
        D(const glm::vec3 &wo, const glm::vec3 &wm, const glm::vec3 &normal, float roughness) {
            float cosTheta = std::max(0.f, glm::dot(wo, normal));
            float G1 = G1_Smith(wo, normal, roughness);
            float D_ggx = D_GGX(wm, normal, roughness);

            return G1 / cosTheta * D_ggx * std::max(0.f, glm::dot(wo, wm));
        }

        /*!
         *
         * @param w View direction
         * @param wm The normal of microfacet
         * @param normal The marco normal of the surface
         * @param roughness
         * @return The probability density of wm against a surface normal from the view direction w
         */
        float PDF_wm(const glm::vec3 &w, const glm::vec3 &wm, const glm::vec3 &normal,
                     float roughness) {
            return D(w, wm, normal, roughness);
        }

        /*!
         * Since wi is derived by wo and wm, we don't pass wi into this function explicitly
         * @param wo
         * @param wm
         * @param normal
         * @param roughness
         * @return  The probability density of wi against a surface normal from the view direction w
         */
        float
        PDF_wi(const glm::vec3 &wo, const glm::vec3 &wm, const glm::vec3 &normal, float roughness) {
            float pdf_wm = PDF_wm(wo, wm, normal, roughness);

            return pdf_wm / (4.f * std::max(0.f, glm::dot(wo, wm)));
        }

        float Fresnel(const glm::vec3 &wi, const glm::vec3 &wm, float F0 = 0.04) {
            float cosTheta = std::max(0.f, glm::dot(wi, wm));

            return F0 + (1.f - F0) * pow(1 - cosTheta, 5.f);
        }

        float
        BRDF(const glm::vec3 &wi, const glm::vec3 &wo, const glm::vec3 &normal,
             float roughness) {
            glm::vec3 wm = glm::normalize(wi + wo);
            float cosTheta_i = std::max(0.f, glm::dot(wi, normal));
            float cosTheta_o = std::max(0.f, glm::dot(wo, normal));
            float D = D_GGX(wm, normal, roughness);
            float F = Fresnel(wi, wm);
            float G = G_Smith(wi, wo, normal, roughness);

            return D * F * G / (4.f * cosTheta_i * cosTheta_o);
        }

        /*!
         *
         * @param wo View direction
         * @param normal The marco normal of the surface
         * @param roughness
         * @return Sampled microfacet normal(in normal space)
         */
        glm::vec3
        sampleMicrofacetNormal(const glm::vec3 &wo, const glm::vec3 &normal, float roughness) {
            // Transform all directions to Normal Coord System
            glm::mat3 world2Normal = buildCoordSystem(normal);
            glm::vec3 wo_normal = world2Normal * wo;

            // Normalize alpha * wo
            glm::vec3 woh_normal = glm::normalize(
                    glm::vec3(roughness * wo_normal.x, roughness * wo_normal.y, wo_normal.z));
            // Transform all directions to View Coord System(the z-axis is woh)
            glm::mat3 normal2View = buildCoordSystem(woh_normal);
//            glm::vec3 T1 = normal2View[0], T2 = normal2View[1];
            glm::vec3 T1 = glm::cross(glm::vec3(0.f, 0.f, 1.f), woh_normal);
            glm::vec3 T2 = glm::cross(woh_normal, T1);

            glm::vec2 p = sampleUniformDiskPolar();
            float h = std::sqrt(1 - p.x * p.x);
            p.y = lerp((1 + woh_normal.z) / 2.f, h, p.y);
            float pz = std::sqrt(std::max(0.f, 1 - glm::length(p)));
            glm::vec3 nh = p.x * T1 + p.y * T2 + pz * woh_normal;

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
