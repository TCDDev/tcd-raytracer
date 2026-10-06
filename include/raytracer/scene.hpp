#pragma once

#include <variant>
#include <vector>

#include <raytracer/triangle.hpp>
#include <raytracer/sphere.hpp>
#include <raytracer/light.hpp>


namespace raytracer {
    class Scene {

        private:
            using Object = std::variant<raytracer::Sphere, raytracer::Triangle>;
            
            Vec3 background_{0, 0, 0};
            Vec3 ambient_{0.1, 0.1, 0.1};
            
            std::vector<Object> objects_;
            std::vector<Light> lights_;
        
        public:

            void add(const Sphere& sphere) {
                objects_.push_back(sphere);
            }

            void add(const Triangle& triangle) {
                objects_.push_back(triangle);
            }

            void add(const Light& light) {
                lights_.push_back(light);
            }

            const std::vector<Light>& lights() const {
                return lights_;
            }

            const Vec3& ambient() const {
                return ambient_;
            }

            const Vec3& background() const {
                return background_;
            }

            void set_ambient(const Vec3& ambient) {
                ambient_ = ambient;
            }

            void set_background (const Vec3& background) {
                background_ = background;
            }



            // TODO: Return the CLOSEST hit among all objects, or std::nullopt if the ray
            // misses everything. Loop over objects; each time one reports a hit, keep it
            // as the best so far AND shrink t_max to that hit's t, so subsequent objects
            // only count if they are nearer.
            
            std::optional<Hit> closest_hit(const Ray &ray, double t_min, double t_max) const {
                std::optional<Hit> best = std::nullopt;
                double closest_t =  t_max;

                for (const auto& object : objects_) {
                const auto hit = std::visit(
                    [&](const auto& shape) {
                        return shape.intersect(
                            ray,
                            t_min,
                            closest_t
                        );
                    },
                    object
                );

                if (hit) {
                    closest_t = hit->t;
                    best = hit;
                }
                }

                return best;

            }

            // TODO: Is any object between `point` and a light `light_distance` away in
            // direction `to_light`? Shoot a shadow ray from point (nudged a tiny bit
            // along to_light, e.g. point + to_light * 1e-4) and return whether
            // closest_hit finds anything with t in (1e-4, light_distance).
            
            bool in_shadow(const Vec3 &point, const Vec3 &to_light, double light_distance) const {
                const Ray shadow_ray(point + to_light * 1e-4, to_light);

                const auto hit = closest_hit(shadow_ray, 1e-4, light_distance);

                return hit.has_value();
            }

          
    };


}
