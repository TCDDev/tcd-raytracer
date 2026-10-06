#pragma once

#include <raytracer/vec3.hpp>
#include <raytracer/material.hpp>

namespace raytracer {

    struct Hit {
    double t;    // ray parameter at the intersection
    Vec3 point;  // world-space intersection point
    Vec3 normal; // unit surface normal at that point
    const Material* material;
    };

}
