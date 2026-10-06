#pragma once

#include <raytracer/vec3.hpp>

namespace raytracer {

    struct Material {
    Vec3 diffuse {0.8, 0.8, 0.8};  // base color under direct light
    Vec3 specular {1.0, 1.0, 1.0}; // color of the shiny highlight
    double shininess = 32.0;             // Phong exponent (bigger = tighter)
    double reflectivity = 0.0;           // 0 = matte, 1 = perfect mirror
    };

}
