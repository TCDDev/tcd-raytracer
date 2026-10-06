#pragma once


#include <raytracer/material.hpp>
#include <raytracer/hit.hpp>
#include <raytracer/ray.hpp>

/* namespace raytracer {
    
    class Object {
    public:
        Material material;
        
        explicit Object(const Material &material) : material(material) {}
        
        virtual ~Object() = default;
        
        // Return the closest hit with t in (t_min, t_max), or std::nullopt on a miss.
        virtual std::optional<Hit> intersect(const Ray &ray, double t_min, double t_max) const = 0;
    };

}
 */