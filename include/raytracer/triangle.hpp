#pragma once


#include <algorithm>
#include <array>
#include <optional>

#include <raytracer/aabb.hpp>
#include <raytracer/vec3.hpp>
#include <raytracer/material.hpp>
#include <raytracer/hit.hpp>
#include <raytracer/ray.hpp>


namespace raytracer {

    struct Triangle {
        
        Vec3 v0, v1, v2;
        const Material material;

        Triangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Material &material) : 
                v0(v0), 
                v1(v1), 
                v2(v2),
                material(material) {}

        
                
        //Ray-triangle intersection using the Moeller-Trumbore algorithm.
        std::optional<Hit> intersect(const Ray &ray, double t_min, double t_max) const   {
            
            const Vec3 edge1 = v1 - v0;
            const Vec3 edge2 = v2 - v0;
            const Vec3 h = cross(ray.direction, edge2);

            const double a = dot(edge1, h);

            if (std::abs(a) < 1e-9) {return std::nullopt;}

            const double f = 1.0 / a;
            const Vec3 s = ray.origin - v0;
            const double u = f * dot(s, h);

            if (u < 0.0 || u > 1.0) {return std::nullopt;}

            const Vec3 q = cross(s, edge1);
            const double v = f * dot(ray.direction, q);

            if (v < 0 || u + v > 1) {return std::nullopt;}

            const double t = f * dot(edge2, q);

            if (t <= t_min || t >= t_max ) {return std::nullopt;}

            Vec3 normal = cross(edge1, edge2).normalized();

            if (dot(normal, ray.direction) > 0) {normal = -normal;}

            return Hit(t, ray.at(t), normal, &material);
        }

        AABB bounding_box() const {
            Vec3 min = v0;
            Vec3 max = v0;
            std::array<const Vec3*, 3> vertices{&v0, &v1, &v2};

            for (const auto& vertice: vertices) {
                min.x = std::min(min.x, vertice->x);
                min.y = std::min(min.y, vertice->y);
                min.z = std::min(min.z, vertice->z);

                max.x = std::max(max.x, vertice->x);
                max.y = std::max(max.y, vertice->y);
                max.z = std::max(max.z, vertice->z);
            }

            return {
                min,
                max
            };
        }
    };
}
