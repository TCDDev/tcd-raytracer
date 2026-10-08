#pragma once

#include <raytracer/vec3.hpp>   

namespace raytracer {

    struct Ray {
        Vec3 origin; 
        Vec3 direction; // direction is kept normalized by the constructor
  
  
        Ray(const Vec3 &origin, const raytracer::Vec3 &direction): origin(origin), direction(direction.normalized()) {}
  
        // returns the point at parameter t along the ray:  origin + direction * t
        Vec3 at(double t) const { return {origin + direction * t}; }
    };

}