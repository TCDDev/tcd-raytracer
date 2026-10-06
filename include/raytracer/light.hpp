#pragma once

#include <raytracer/vec3.hpp>

namespace raytracer {
    
    struct Light {
    Vec3 position;
    Vec3 color {1.0, 1.0, 1.0};
    };

}
