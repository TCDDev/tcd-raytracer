// parsers.hpp

#pragma once

#include <string>

#include <raytracer/camera.hpp>
#include <raytracer/scene.hpp>

namespace raytracer {

struct CameraConfig {
    Vec3 eye{0, 0, 0};
    Vec3 look_at{0, 0, -1};
    Vec3 up{0, 1, 0};
    double fov = 60.0;
};

struct ParsedScene {
    Scene scene;
    CameraConfig camera;
    int width = 800;
    int height = 600;
};

ParsedScene parse_scene(
    const std::string& filename
);

}