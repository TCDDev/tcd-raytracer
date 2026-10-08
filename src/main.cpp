// Standalone renderer for the Raytracer
// Useage: ./scripts/run.sh <scene.json> <output.png>

#include <algorithm>
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

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

    // 3. Create image 
    Image image(parsed.width, parsed.height);

    // 4. Get the amount of threads the system reports, min 1
    const unsigned int num_threads = std::max(1u, std::thread::hardware_concurrency());

    // 5. Split rows between threads
    const int rows_per_thread = (parsed.height + num_threads - 1) / num_threads;

    std::vector<std::thread> threads;
    // 6. Render in parallel
    std::cout << "rendering...\n";
    for (unsigned int i = 0; i < num_threads; ++i) {
    const int start_y = i * rows_per_thread;
    const int end_y = std::min(start_y + rows_per_thread, parsed.height);

    if (start_y >= end_y) {
        break;
    }

    threads.emplace_back([&, start_y, end_y]() {
        for (int y = start_y; y < end_y; ++y) {
            for (int x = 0; x < parsed.width; ++x) {
                double s = (x + 0.5) / parsed.width;
                double t = (y + 0.5) / parsed.height;

                image.set(x, y, renderer.trace(camera.ray_through(s, t), 0));
            }
        }
    });
}
  // 7. Join threads
  for (auto& thread : threads) {
      thread.join();
  }

  std::cout << "Rendering completed!" << std::endl;

  

  // 8. Write the PNG.
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
