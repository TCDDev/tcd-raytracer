// Standalone renderer for the Raytracer
// Useage: ./scripts/run.sh <scene.json> <output.png>

#include <exception>
#include <iostream>
#include <string>

#include <raytracer/camera.hpp>
#include <raytracer/image.hpp>
#include <raytracer/parsers.hpp>
#include <raytracer/renderer.hpp>
#include <raytracer/scene.hpp>

using namespace raytracer;

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " <scene.json> <output.png>\n";
    return 1;
  }

  const std::string scene_file = argv[1];
  const std::string output_file = argv[2];

  try {
    // 1. Read the scene from the text file.
    ParsedScene parsed = parse_scene(scene_file);

    // 2. Build the camera and renderer.
    Camera camera(parsed.camera.eye, parsed.camera.look_at, parsed.camera.up,
                  parsed.camera.fov, parsed.width, parsed.height);
    Renderer renderer(parsed.scene);

    // 3. Render, one primary ray per pixel.
    Image image(parsed.width, parsed.height);
    for (int y = 0; y < parsed.height; ++y) {
      for (int x = 0; x < parsed.width; ++x) {
        double s = (x + 0.5) / parsed.width;
        double t = (y + 0.5) / parsed.height;
        image.set(x, y, renderer.trace(camera.ray_through(s, t), 0));
      }
      std::cout << "\rrendering... " << (100 * (y + 1) / parsed.height) << '%' << std::flush;
    }
    std::cout << '\n';

    // 4. Write the PNG.
    if (!image.save_png(output_file)) {
      std::cerr << "error: could not write " << output_file << '\n';
      return 1;
    }
    std::cout << "wrote " << output_file << '\n';
  } catch (const std::exception &e) {
    std::cerr << "error: " << e.what() << '\n';
    return 1;
  }
  return 0;
}
