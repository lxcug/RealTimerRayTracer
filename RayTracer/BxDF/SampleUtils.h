//
// Created by Tencent_GO on 2024/4/18.
//

#ifndef REAL_TIME_RAY_TRACER_SAMPLEUTILS_H
#define REAL_TIME_RAY_TRACER_SAMPLEUTILS_H

#include "Core/RealTimeRT.h"
#include <iostream>


namespace SampleUtils {

    inline glm::mat3 buildCoordSystem(const glm::vec3 &normal) {
        glm::vec3 x, y, z = normal;

        if (abs(z.x) < .57735027f)
            x = glm::vec3(1.f, 0.f, 0.f);
        else
            x = glm::vec3(0.f, 0.f, 1.f);

        x = normalize(x - dot(x, z) * z);
        y = cross(z, x);

        return {x, y, z};
    }

    inline glm::vec2
    sampleUniformDiskPolar(const glm::vec2 &u = glm::vec2(random_float(), random_float())) {
        float r = std::sqrt(u.x);
        float theta = 2 * pi * u.y;

        return {
                r * std::cos(theta),
                r * std::sin(theta)
        };
    }

    inline glm::vec2
    sampleUniformDiskConcentric(const glm::vec2 &u = glm::vec2(random_float(), random_float())) {
        // TODO

        return {};
    }

    inline glm::vec3 sampleCosineHemisphere(const glm::vec3 &normal) {
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
    inline void coord2Spherical(const glm::vec3 &dir, float &theta, float &phi) {
        theta = std::acos(dir.z);

        phi = std::atan2(dir.y, dir.x);

        if (phi < 0.f) {
            phi += 2.f * pi;
        }
    }

    inline bool isInSameHemisphere(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &normal) {
        glm::mat3 world2Normal = buildCoordSystem(normal);

        return (world2Normal * v1).z * (world2Normal * v2).z >= 0.f;
    }

    // Refer: PBRT 9.6 alpha Using Microfacet Theory
    namespace Microfacet {
#if RENDER_MODE_REAL_TIME

        /*!
         *
         * @param wm The normal of microfacet
         * @param normal The marco normal of the surface
         * @param alpha
         * @return The relative differential area of microfacets with surface normal wm
         */
        inline float D_GGX(const glm::vec3 &wm, const glm::vec3 &normal, float alpha) {
            float cosTheta_m = std::max(0.f, glm::dot(wm, normal));
            float sinTheta_m = std::sqrt(1 - cosTheta_m * cosTheta_m);
            float cos2Theta_m = cosTheta_m * cosTheta_m;
            float tan2Theta_m = sinTheta_m * sinTheta_m / cos2Theta_m;
            if (isInf(tan2Theta_m))
                return 0.f;
            float alpha2 = alpha * alpha;
            float cos4Theta_m = cos2Theta_m * cos2Theta_m;

            float denom = pi * alpha2 * cos4Theta_m * pow((1.f + tan2Theta_m / alpha2), 2.f);

            return 1.f / denom;
        }

        /*!
         * Helper function for calculating G1_Smith
         * @param theta The azimuth of the view direction
         * @param alpha
         * @return
         */
        inline float Lambda(float theta, float alpha) {
            float tan2Theta = std::pow(std::tan(theta), 2.f);
            if (isInf(tan2Theta))
                return 0.f;

            float alpha2 = alpha * alpha;
            float nom = std::sqrt(1.f + alpha2 * tan2Theta) - 1.f;

            return nom / 2.f;
        }

        /*!
         *
         * @param w View direction or Light Incident direction
         * @param normal The marco normal of the surface
         * @param alpha
         * @return The fraction of microfacets that are visible from w
         */
        inline float G1_Smith(const glm::vec3 &w, const glm::vec3 &normal, float alpha) {
            float theta = std::acos(glm::dot(w, normal));

            return 1.f / (1.f + Lambda(theta, alpha));
        }

        /*!
         *
         * @param wi Light incident direction
         * @param wo View direction
         * @param normal The marco normal of the surface
         * @param alpha
         * @return
         */
        inline float
        G_Smith(const glm::vec3 &wi, const glm::vec3 &wo, const glm::vec3 &normal,
                float alpha) {
            float theta_i = std::acos(glm::dot(wi, normal));
            float theta_o = std::acos(glm::dot(wo, normal));

            return 1.f / (1.f + Lambda(theta_i, alpha) + Lambda(theta_o, alpha));
        }

        /*!
         *
         * @param w View direction
         * @param wm The normal of microfacet
         * @param normal The marco normal of the surface
         * @param alpha
         * @return The probability density of wm against a surface normal from the view direction w
         */
        inline float PDF_wm(const glm::vec3 &wo, const glm::vec3 &wm, const glm::vec3 &normal,
                     float alpha) {
            float cosTheta = std::max(0.f, glm::dot(wo, normal));
            float G1 = G1_Smith(wo, normal, alpha);
            float D_ggx = D_GGX(wm, normal, alpha);

            return G1 / cosTheta * D_ggx * std::max(0.f, glm::dot(wo, wm));
        }

        /*!
         * Since wi is derived by wo and wm, we don't pass wi into this function explicitly
         * @param wo
         * @param wm
         * @param normal
         * @param alpha
         * @return  The probability density of wi against a surface normal from the view direction w
         */
        inline float
        PDF_wi(const glm::vec3 &wo, const glm::vec3 &wm, const glm::vec3 &normal, float alpha) {
            float pdf_wm = PDF_wm(wo, wm, normal, alpha);

            return pdf_wm / (4.f * std::abs(glm::dot(wo, wm)));
        }

        float Fresnel(const glm::vec3 &wo, const glm::vec3 &wm, float F0 = 0.04) {
            float cosTheta = std::max(0.f, glm::dot(wo, wm));

            return F0 + (1.f - F0) * pow(1 - cosTheta, 5.f);
        }

        /*!
         *
         * @param wi
         * @param wo
         * @param normal
         * @param alpha
         * @return
         */
        inline float
        BRDF(const glm::vec3 &wi, const glm::vec3 &wo, const glm::vec3 &normal,
             float alpha) {
            glm::vec3 wm = glm::normalize(wi + wo);
            float cosTheta_i = std::abs(glm::dot(wi, normal));
            float cosTheta_o = std::abs(glm::dot(wo, normal));
            float D = D_GGX(wm, normal, alpha);
            float F = Fresnel(wo, wm, 0.21);  // TODO
            float G = G_Smith(wi, wo, normal, alpha);

            return D * F * G / (4.f * cosTheta_i * cosTheta_o);
        }

        /*!
         *
         * @param wo View direction
         * @param normal The marco normal of the surface
         * @param alpha
         * @return Sampled microfacet normal(in normal space)
         */
        inline glm::vec3
        sampleMicrofacetNormal(const glm::vec3 &wo, const glm::vec3 &normal, float alpha) {
            // Transform all directions to Normal Coord System
            glm::mat3 world2Normal = buildCoordSystem(normal);
            glm::vec3 wo_normal = world2Normal * wo;

            glm::vec3 woh_normal = glm::normalize(
                    glm::vec3(alpha * wo_normal.x, alpha * wo_normal.y, wo_normal.z));
            if (woh_normal.z < 0.f)
                woh_normal = -wo_normal;

            // Transform all directions to View Coord System(the z-axis is woh)
//            glm::mat3 normal2View = buildCoordSystem(woh_normal);
//            glm::vec3 T1 = normal2View[0], T2 = normal2View[1];
            glm::vec3 T1 = woh_normal.z < 0.99999f ? glm::normalize(
                    glm::cross(glm::vec3(0.f, 0.f, 1.f), woh_normal)) : glm::vec3(1.f, 0.f, 0.f);
            glm::vec3 T2 = glm::cross(woh_normal, T1);

            glm::vec2 p = sampleUniformDiskPolar();
            float h = std::sqrt(1 - p.x * p.x);
            p.y = lerp((1 + woh_normal.z) / 2.f, h, p.y);
            float pz = std::sqrt(std::max(0.f, 1.f - pow(glm::length(p), 2.f)));
            glm::vec3 nh = p.x * T1 + p.y * T2 + pz * woh_normal;

            glm::vec3 wm_normal = glm::normalize(glm::vec3(
                    alpha * nh.x,
                    alpha * nh.y,
                    std::max(1e-6f, nh.z)
            ));

            glm::vec3 wm_world = glm::inverse(world2Normal) * wm_normal;
            wm_world = faceForward(wm_world, normal);

            return sampleCosineHemisphere(normal);
        }

#else
        inline float D_GGX(const glm::vec3 &normal, float alpha_x, float alpha_y) {
            float theta, phi;
            coord2Spherical(normal, theta, phi);
            float sinTheta = std::sin(theta), cosTheta = std::cos(theta);
            float cos2Theta = cosTheta * cosTheta;

            float tan2theta = sinTheta * sinTheta / cos2Theta;
            float cos4Theta = cos2Theta * cos2Theta;
            float alpha_x2 = alpha_x * alpha_x;
            float alpha_y2 = alpha_y * alpha_y;
            float cos2Phi = std::pow(std::cos(phi), 2.f);
            float sin2Phi = std::pow(std::sin(phi), 2.f);

            float denom = pi * alpha_x * alpha_y * cos4Theta *
                          (1.f + tan2theta * (cos2Phi / alpha_x2 + sin2Phi / alpha_y2));

            return 1.f / denom;
    }
#endif
    }

}


#endif //REAL_TIME_RAY_TRACER_SAMPLEUTILS_H
