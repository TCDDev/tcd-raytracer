#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <utility>

#include <raytracer/ray.hpp>
#include <raytracer/vec3.hpp>


namespace raytracer {

     struct AABB {
                Vec3 min;
                Vec3 max;

                bool hit (const Ray& ray, double t_min, double t_max) const {
                    std::array origin{ray.origin.x, ray.origin.y, ray.origin.z};
                    std::array direction{ray.direction.x, ray.direction.y, ray.direction.z};
                    std::array box_min{min.x, min.y, min.z};
                    std::array box_max{max.x, max.y, max.z};


                    // Treat the AABB as three axis-aligned slabs.
                    // For each axis, find the interval of ray parameter t for which
                    // the ray lies inside that slab.
                    for (std::size_t i = 0; i < origin.size(); ++i) {
                        
                        
                        // If the ray is parallel to the i (X/Y/Z) slab and its origin.i is outside [min.i, max.i], 
                        // it can never enter the box
                        if (direction[i] == 0) {
                            if (origin[i] < box_min[i] || origin[i] > box_max[i]) {
                                return false;
                            }

                            continue;
                        }
                        
                        double t0 = (box_min[i] - origin[i]) / direction[i];
                        double t1 = (box_max[i] - origin[i]) / direction[i];

                        if (t0 > t1) {
                            std::swap(t0, t1);
                        }

                        t_min = std::max(t_min, t0);
                        t_max = std::min(t_max, t1);

                        if (t_min > t_max) {
                            return false;
                        }
                    }

                    return true;
                }
            };

}
