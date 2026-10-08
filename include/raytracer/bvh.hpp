#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <variant>
#include <vector>

#include <raytracer/aabb.hpp>
#include <raytracer/hit.hpp>
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

    inline AABB surrounding_box(const Objects& objects, const std::vector<std::size_t>& object_indices) {
        if (object_indices.empty()) {
            throw std::invalid_argument("surrounding_box requires at least one object");
        }
        
        const Object& first_object = objects[object_indices[0]];

        AABB first_box = std::visit(
            [](const auto& shape) {
                return shape.bounding_box();
            }, first_object
        );

        Vec3 min = first_box.min;
        Vec3 max = first_box.max;
        
        for (std::size_t index : object_indices) {
            const Object& object = objects[index];

            const AABB box = std::visit(
                [](const auto& shape) {
                    return shape.bounding_box();
                }, object
            );

            min.x = std::min(min.x, box.min.x);
            min.y = std::min(min.y, box.min.y);
            min.z = std::min(min.z, box.min.z);

            max.x = std::max(max.x, box.max.x);
            max.y = std::max(max.y, box.max.y);
            max.z = std::max(max.z, box.max.z);
        }

        return{
            min,
            max
        };
    }

    inline int longest_axis(const AABB& box) {
        double x = box.max.x - box.min.x;
        double y= box.max.y - box.min.y;
        double z = box.max.z - box.min.z;

        if (x >= y && x >= z) {return 0;}
        if (y >= z) {return 1;}
        return 2;
    }

    inline double center_on_axis(const Object& object, int axis) {
        const AABB box = std::visit(
            [](const auto& shape) {
                return shape.bounding_box();
            }, object
        );

        if (axis == 0) {return (box.min.x + box.max.x) / 2;}
        if (axis == 1) {return (box.min.y + box.max.y) / 2;}
        return (box.min.z + box.max.z) / 2;
    }

    inline std::optional<Hit> traverse_bvh(const BVHNode* node, const Objects& objects, const Ray& ray, double t_min, double t_max) {
        if (node == nullptr) {return std::nullopt;}

        if (!node->box.hit(ray, t_min, t_max)) {return std::nullopt;}

        std::optional<Hit> best = std::nullopt;
        double closest_t = t_max;

        // Check if the node is a leaf by ensuring it has no child nodes
        if (!node->left && !node->right) {
            for (std::size_t index : node->object_indices) {
                const Object& current_object = objects[index];

                const auto hit = std::visit(
                    [&](const auto& shape) {
                        return shape.intersect(ray, t_min, closest_t);
                    }, current_object
                );

                if (hit) {
                    closest_t = hit->t;
                    best = hit;
                }
            }

            return best;
        }

        // Recurse left
        std::optional<Hit> left_hit = traverse_bvh(node->left.get(), objects, ray, t_min, t_max);

        double right_t_max = t_max;
        
        // If there is a left hit, tighten the t value
        if (left_hit) {
            right_t_max = left_hit->t;
        }

        // Recurse right
        std::optional<Hit> right_hit = traverse_bvh(node->right.get(), objects, ray, t_min, right_t_max);

        if (right_hit) {
            return right_hit;
        }

        return left_hit;
    }

    inline std::unique_ptr<BVHNode> build_bvh_tree(const Objects& objects, std::vector<size_t> object_indices) {
        auto node = std::make_unique<BVHNode>();

        // 1. Compute one bounding box containing all objects in "object_indicies"
        node->box = surrounding_box(objects, object_indices);

        // 2. Base case: few enough objects means make this a leaf
        if (object_indices.size() <= 2) {
            node->object_indices = std::move(object_indices);
            return node;
        }

        // 3. Pick the longest axis of node->box (0 = X, 1 = Y, 2 = Z)
        const int axis = longest_axis(node->box);

        // 4. Sort object indices by bounding-box center on that axis
        std::sort(object_indices.begin(), object_indices.end(), [&](std::size_t lhs, std::size_t rhs) {
            return center_on_axis(objects[lhs], axis) < center_on_axis(objects[rhs], axis);
        });

        // 5. Split the indicies in half
        const std::size_t midpoint_index = object_indices.size() / 2;

        std::vector<std::size_t> left_indices(object_indices.begin(), object_indices.begin() + midpoint_index);
        std::vector<std::size_t> right_indicies(object_indices.begin() + midpoint_index, object_indices.end());

        // 6. Recursively build both children
        node->left = build_bvh_tree(objects, std::move(left_indices));
        node->right = build_bvh_tree(objects, std::move(right_indicies));

        return node;
    }

    inline std::unique_ptr<BVHNode> build_bvh(const Objects& objects) {
        std::vector<std::size_t> object_indices(objects.size());
        std::iota(object_indices.begin(), object_indices.end(), 0);

        return build_bvh_tree(objects, std::move(object_indices));
    }

}
