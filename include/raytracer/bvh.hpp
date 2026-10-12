#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <variant>
#include <vector>

#include <raytracer/aabb.hpp>
#include <raytracer/hit.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/sphere.hpp>
#include <raytracer/triangle.hpp>

namespace raytracer {

    using Object = std::variant<Sphere, Triangle>;
    using Objects = std::vector<Object>;

    struct BVHNode {
        AABB box;

        std::unique_ptr<BVHNode> left;
        std::unique_ptr<BVHNode> right;

        std::vector<std::size_t> object_indices;
    };

    std::optional<Hit> traverse_bvh(
        const BVHNode* node,
        const Objects& objects,
        const Ray& ray,
        double t_min,
        double t_max
    );

    std::unique_ptr<BVHNode> build_bvh(const Objects& objects);

} // namespace raytracer