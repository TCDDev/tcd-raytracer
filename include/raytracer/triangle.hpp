#pragma once

#include <optional>

#include <raytracer/vec3.hpp>
#include <raytracer/material.hpp>
#include <raytracer/hit.hpp>
#include <raytracer/ray.hpp>

namespace raytracer {

    class Triangle {
    public:
        
        Vec3 v0, v1, v2;
        const Material material;

        Triangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Material &material) : 
                v0(v0), 
                v1(v1), 
                v2(v2),
                material(material) {}

        // TODO: Ray-triangle intersection using the Moeller-Trumbore algorithm.
        // Steps (return std::nullopt as soon as any test fails):
        //   edge1 = v1 - v0;  edge2 = v2 - v0;
        //   h = cross(ray.direction, edge2);   a = dot(edge1, h);
        //   if |a| < 1e-9: ray is parallel to the triangle -> miss.
        //   f = 1/a;   s = ray.origin - v0;   u = f * dot(s, h);
        //   if u < 0 or u > 1: miss.
        //   q = cross(s, edge1);   v = f * dot(ray.direction, q);
        //   if v < 0 or u + v > 1: miss.
        //   t = f * dot(edge2, q);
        //   if t not in (t_min, t_max): miss.
        //   normal = cross(edge1, edge2).normalized();
        //   if dot(normal, ray.direction) > 0: normal = -normal;  // face the ray
        //   Fill and return a Hit (t, ray.at(t), normal, material).


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
    };
}
