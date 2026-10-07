#pragma once

#include <raytracer/ray.hpp>
#include <raytracer/vec3.hpp>

namespace raytracer {

     struct AABB_ {
                Vec3 min;
                Vec3 max;

                bool hit (const Ray& ray, double t_min, double t_max);
            };

}
