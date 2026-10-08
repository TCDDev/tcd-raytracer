#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <stdexcept>
#include <variant>
#include <vector>

#include <raytracer/aabb.hpp>
#include <raytracer/bvh.hpp>
#include <raytracer/triangle.hpp>
#include <raytracer/sphere.hpp>
#include <raytracer/light.hpp>


namespace raytracer {
    class Scene {

        private:
            using Object = std::variant<Sphere, Triangle>;
            using Objects = std::vector<Object>;
            
            Vec3 background_{0, 0, 0};
            Vec3 ambient_{0.1, 0.1, 0.1};
            
            std::vector<Object> objects_;
            std::vector<Light> lights_;

            std::unique_ptr<BVHNode> root_;
        
        public:

            void add(const Sphere& sphere) {objects_.push_back(sphere);}
            void add(const Triangle& triangle) {objects_.push_back(triangle);}
            void add(const Light& light) {lights_.push_back(light);}

            const std::vector<Light>& lights() const {return lights_;}
            const Vec3& ambient() const {return ambient_;}
            const Vec3& background() const {return background_;}

            void set_ambient(const Vec3& ambient) {ambient_ = ambient;}
            void set_background (const Vec3& background) {background_ = background;}

            // Must be called after all scene objects have been added
            void create_bvh() {
                root_ = build_bvh(objects_);
            }

            
            std::optional<Hit> closest_hit(const Ray &ray, double t_min, double t_max) const {
                if (objects_.empty()) {
                    return std::nullopt;
                }
                
                if (!root_) {
                    throw std::logic_error("BVH has not been created");
                }

                return traverse_bvh(root_.get(), objects_, ray, t_min, t_max);

            }

            bool in_shadow(const Vec3 &point, const Vec3 &to_light, double light_distance) const {
                const Ray shadow_ray(point + to_light * 1e-4, to_light);

                const auto hit = closest_hit(shadow_ray, 1e-4, light_distance);

                return hit.has_value();
            }

          
    };


}
