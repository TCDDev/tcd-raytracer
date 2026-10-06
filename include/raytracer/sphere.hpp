#pragma once

#include <optional>

#include <raytracer/vec3.hpp>
#include <raytracer/material.hpp>
#include <raytracer/hit.hpp>
#include <raytracer/ray.hpp>

namespace raytracer {

    class Sphere {
    public:
        
        const Vec3 center;
        double radius;
        const Material material;
        
        Sphere(const Vec3& center, double radius, const Material& material) : center(center), radius(radius), material(material) {}

        // Ray-sphere intersection.
        // Substitute the ray into |p - center|^2 = radius^2 to get a quadratic
        // a t^2 + b t + c = 0 with:
        //     oc = ray.origin - center
        //     a  = dot(direction, direction)
        //     b  = 2 * dot(oc, direction)
        //     c  = dot(oc, oc) - radius*radius
        // If the discriminant b*b - 4ac < 0 there is no hit -> return std::nullopt.
        // Otherwise take the nearest root t = (-b - sqrt(disc)) / (2a); if it is not
        // in (t_min, t_max), try the far root (-b + sqrt(disc)) / (2a); if that is
        // also out of range, return std::nullopt.
        // On a hit, fill a Hit: t, point = ray.at(t),
        //     normal = (point - center).normalized(),  material = material.
        
        
        std::optional<Hit> intersect(const Ray &ray, double t_min, double t_max) const {
            const Vec3 oc = ray.origin - center;
            const double a = dot(ray.direction, ray.direction);
            const double b = 2 * dot(oc, ray.direction);
            const double c = dot(oc, oc) - radius * radius;

            const double discriminant = b * b - 4 * a * c;

            if (discriminant < 0) {
            return std::nullopt;
            }

            const double sqrt_discriminant = std::sqrt(discriminant);

        
            const double t_near = (-b - sqrt_discriminant) / (2 * a);
            const double t_far = (-b + sqrt_discriminant) / (2 * a);

            double t = t_near;

            if (t_near <= t_min || t_near >= t_max) {
            t = t_far;
            
            if (t_far <= t_min || t_far >= t_max ) {
                return std::nullopt;
            }
            }

            const Vec3 point = ray.at(t);
            const Vec3 normal = (point - center).normalized();

            return Hit{t, point, normal, &material};

        }
    };

}
