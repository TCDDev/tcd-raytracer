#include <raytracer/aabb.hpp>
#include <raytracer/camera.hpp>
#include <raytracer/hit.hpp>
#include <raytracer/image.hpp>
#include <raytracer/light.hpp>
#include <raytracer/material.hpp>
#include <raytracer/object.hpp>
#include <raytracer/parsers.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/renderer.hpp>
#include <raytracer/scene.hpp>
#include <raytracer/sphere.hpp>
#include <raytracer/triangle.hpp>
#include <raytracer/vec3.hpp>


#include <cmath>
#include <cstdlib>
#include <doctest/doctest.h>
#include <limits>
#include <nlohmann/json.hpp>
#include "nlohmann/json_fwd.hpp"
#include <optional>
#include <string>

using namespace raytracer;
using json = nlohmann::json;

// ============================================================================
// PART 1: Vec3 - a 3D vector, used for points, directions, and colors.
// ============================================================================

TEST_CASE("Part 1: Vec3") {
  Vec3 a(1, 2, 3), b(4, 5, 6);
  CHECK((a + b).x == doctest::Approx(5));
  CHECK((b - a).y == doctest::Approx(3));
  CHECK((a * 2.0).z == doctest::Approx(6));
  CHECK((2.0 * a).x == doctest::Approx(2));
  CHECK((b / 2.0).x == doctest::Approx(2));
  CHECK((-a).y == doctest::Approx(-2));
  CHECK(dot(a, b) == doctest::Approx(32));
  Vec3 c = cross(Vec3(1, 0, 0), Vec3(0, 1, 0));
  CHECK(c.x == doctest::Approx(0));
  CHECK(c.y == doctest::Approx(0));
  CHECK(c.z == doctest::Approx(1));
  CHECK(Vec3(3, 4, 0).length() == doctest::Approx(5));
  Vec3 n = Vec3(0, 3, 4).normalized();
  CHECK(n.length() == doctest::Approx(1));
  CHECK(n.y == doctest::Approx(0.6));
}

// ============================================================================
// PART 2: reflect - mirror a direction about a surface normal.
// ============================================================================

TEST_CASE("Part 2: reflect") {
  Vec3 r = reflect(Vec3(0, -1, 0), Vec3(0, 1, 0));
  CHECK(r.x == doctest::Approx(0));
  CHECK(r.y == doctest::Approx(1));
  CHECK(r.z == doctest::Approx(0));
  Vec3 r2 = reflect(Vec3(1, -1, 0), Vec3(0, 1, 0));
  CHECK(r2.x == doctest::Approx(1));
  CHECK(r2.y == doctest::Approx(1));
}

// ============================================================================
// PART 3: Ray - a half-line  origin + t * direction  (t >= 0).
// ============================================================================

TEST_CASE("Part 3: Ray") {
  Ray ray(Vec3(1, 2, 3), Vec3(0, 0, 5));
  CHECK(ray.direction.z == doctest::Approx(1)); // normalized
  Vec3 p = ray.at(2.0);
  CHECK(p.x == doctest::Approx(1));
  CHECK(p.z == doctest::Approx(5));
}


// ============================================================================
// PART 4: Sphere intersection
// ============================================================================

TEST_CASE("Part 4: Sphere intersection & bounding box") {
  Material m;
  Sphere s(Vec3(0, 0, -5), 1.0, m);
  Ray ray(Vec3(0, 0, 0), Vec3(0, 0, -1));
  auto hit = s.intersect(ray, 1e-4, std::numeric_limits<double>::infinity());
  REQUIRE(hit.has_value());
  CHECK(hit->t == doctest::Approx(4.0));
  CHECK(hit->normal.z == doctest::Approx(1.0));
  Ray miss(Vec3(0, 5, 0), Vec3(0, 0, -1));
  CHECK_FALSE(s.intersect(miss, 1e-4, std::numeric_limits<double>::infinity()).has_value());

  const AABB box = s.bounding_box();

  CHECK(box.min.x == doctest::Approx(-1.0));
  CHECK(box.min.y == doctest::Approx(-1.0));
  CHECK(box.min.z == doctest::Approx(-6.0));

  CHECK(box.max.x == doctest::Approx(1.0));
  CHECK(box.max.y == doctest::Approx(1.0));
  CHECK(box.max.z == doctest::Approx(-4.0));
}

// ============================================================================
// PART 5: Triangle intersection (Moeller-Trumbore algorithm)
// ============================================================================

TEST_CASE("Part 5: Triangle intersection") {
  Material m;
  Triangle tri(Vec3(-1, -1, -5), Vec3(1, -1, -5), Vec3(0, 1, -5), m);
  Ray ray(Vec3(0, 0, 0), Vec3(0, 0, -1));
  auto hit = tri.intersect(ray, 1e-4, std::numeric_limits<double>::infinity());
  REQUIRE(hit.has_value());
  CHECK(hit->t == doctest::Approx(5.0));
  CHECK(std::abs(hit->normal.z) == doctest::Approx(1.0));
  Ray miss(Vec3(3, 3, 0), Vec3(0, 0, -1));
  CHECK_FALSE(tri.intersect(miss, 1e-4, std::numeric_limits<double>::infinity()).has_value());

  const AABB box = tri.bounding_box();

  CHECK(box.min.x == doctest::Approx(-1.0));
  CHECK(box.min.y == doctest::Approx(-1.0));
  CHECK(box.min.z == doctest::Approx(-5.0));

  CHECK(box.max.x == doctest::Approx(1.0));
  CHECK(box.max.y == doctest::Approx(1.0));
  CHECK(box.max.z == doctest::Approx(-5));
}

// ============================================================================
// PART 6: Camera - a pinhole camera that shoots primary rays through pixels.
// ============================================================================

TEST_CASE("Part 6: Camera") {
  Camera cam(Vec3(0, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0), 90.0, 100, 100);
  Ray center = cam.ray_through(0.5, 0.5);
  CHECK(center.direction.x == doctest::Approx(0).epsilon(0.01));
  CHECK(center.direction.y == doctest::Approx(0).epsilon(0.01));
  CHECK(center.direction.z == doctest::Approx(-1).epsilon(0.01));
}

// ============================================================================
// PART 7: Scene - the world: objects + lights, with the two ray queries the
// renderer needs.
// ============================================================================


TEST_CASE("Part 7: Scene queries") {
  Scene scene;
  scene.add(
    Sphere{Vec3{0.0, 0.0, -5.0}, 1.0, Material{}}
  );
  scene.create_bvh();

  Ray ray(Vec3(0, 0, 0), Vec3(0, 0, -1));
  auto hit = scene.closest_hit(ray, 1e-4, std::numeric_limits<double>::infinity());
  REQUIRE(hit.has_value());
  CHECK(hit->t == doctest::Approx(4.0));
  // Blocker between a far point and a light: should be in shadow.
  CHECK(scene.in_shadow(Vec3(0, 0, 5), Vec3(0, 0, -1).normalized(), 100.0) == true);
  // Nothing between point and light: not shadowed.
  CHECK(scene.in_shadow(Vec3(0, 0, 5), Vec3(0, 1, 0).normalized(), 100.0) == false);
}


// ============================================================================
// PART 8/9: Renderer - Phong local illumination (shade) + recursive mirror
// reflection (trace).
// ============================================================================

TEST_CASE("Part 8: Phong shading") {
  Scene scene;
  scene.set_ambient(Vec3{0, 0, 0});
  scene.add(Light{Vec3(0, 0, 5), Vec3(1, 1, 1)});
  // scene.create_bvh();
  Renderer r(scene);
  
  Material material{
    .diffuse = Vec3{0.4, 0.6, 0.8},
    .specular = Vec3{0.0, 0.0, 0.0},
    .shininess = 1.0
  };

  Hit hit {
    .t = 1.0,
    .point = Vec3{0.0, 0.0, 0.0},
    .normal = Vec3{0.0, 0.0, 1.0},
    .material = &material
  };

  Ray ray(Vec3(0, 0 , 10), Vec3(0, 0, -1));

  Vec3 c = r.shade(hit, ray);
  CHECK(c.x == doctest::Approx(0.4));
  CHECK(c.y == doctest::Approx(0.6));
  CHECK(c.z == doctest::Approx(0.8));
}

TEST_CASE("Part 9: trace hits and misses") {
  Scene scene;
  scene.set_background(Vec3{0.1, 0.2, 0.3});
  scene.set_ambient(Vec3{0, 0, 0});
  scene.add(Light{Vec3{0, 0, 5}, Vec3{1, 1, 1}});
  Material red;
  red.diffuse = Vec3(0.9, 0.1, 0.1);
  red.specular = Vec3(0, 0, 0);
  scene.add(Sphere{Vec3{0, 0, -5}, 1.0, red});
  scene.create_bvh();
  Renderer r(scene);
  // Miss -> background.
  Vec3 bg = r.trace(Ray(Vec3(0, 10, 0), Vec3(0, 1, 0)), 0);
  CHECK(bg.x == doctest::Approx(0.1));
  CHECK(bg.z == doctest::Approx(0.3));
  // Hit -> red dominant.
  Vec3 hit = r.trace(Ray(Vec3(0, 0, 0), Vec3(0, 0, -1)), 0);
  CHECK(hit.x > hit.y);
  CHECK(hit.x > hit.z);
}


// ============================================================================
// PART 10: Render the scene to a PNG. This test drives everything above; once
// all earlier parts work, it produces "out.png" - open it to see your render!
// ============================================================================
TEST_CASE("Part 10: render scene.txt to out.png") {
  // NOTE: run from the cpplings repo root so this relative path resolves.
  ParsedScene parsed = parse_scene("scenes/test.json");
  Camera camera(parsed.camera.eye, parsed.camera.look_at, parsed.camera.up,
                parsed.camera.fov, parsed.width, parsed.height);
  Renderer renderer(parsed.scene);
  Image image(parsed.width, parsed.height);
  for (int y = 0; y < parsed.height; ++y)
    for (int x = 0; x < parsed.width; ++x) {
      double s = (x + 0.5) / parsed.width;
      double t = (y + 0.5) / parsed.height;
      image.set(x, y, renderer.trace(camera.ray_through(s, t), 0));
    }
  CHECK(image.save_png("out.png") == true);
  // Corner ray misses the centred sphere -> background sky.
  const Vec3 &corner = image.at(0, 0);
  CHECK(corner.x == doctest::Approx(parsed.scene.background().x));
  CHECK(corner.z == doctest::Approx(parsed.scene.background().z));
  // Center ray hits the red sphere -> red channel dominates.
  const Vec3 &mid = image.at(parsed.width / 2, parsed.height / 2);
  CHECK(mid.x > mid.y);
  CHECK(mid.x > mid.z);
}
