#pragma once

#include <raytracer/scene.hpp>

namespace raytracer {
    
    class Renderer {
        
        private:
            const Scene &scene_;
            int max_depth_;
        
        public:
            explicit Renderer(const Scene &scene, int max_depth = 4): scene_(scene), max_depth_(max_depth) {}

            // TODO (PART 8): Phong shading at a hit point.
            // Start with the ambient term:  color = scene_.ambient * material.diffuse.
            // view_dir = (-ray.direction).normalized().
            // For each light in scene_.lights:
            //     to_light = light.position - hit.point;  dist = to_light.length();
            //     light_dir = to_light / dist;
            //     if scene_.in_shadow(hit.point, light_dir, dist): skip this light.
            //     diffuse:  diff = max(0, dot(hit.normal, light_dir));
            //               color += material.diffuse * light.color * diff;
            //     specular (only when diff > 0):
            //               refl = reflect(-light_dir, hit.normal);
            //               spec = pow(max(0, dot(view_dir, refl)), material.shininess);
            //               color += material.specular * light.color * spec;
            // Return color.
        
            
            Vec3 shade(const Hit &hit, const Ray &ray) const {
            Vec3 color = scene_.ambient() * hit.material->diffuse;

            const Vec3 view_dir = (-ray.direction).normalized();

            for (const auto& light : scene_.lights()) {
            const Vec3 to_light = light.position - hit.point;
            const double dist = to_light.length();
            
            const Vec3 light_dir = to_light / dist;
            

            if (scene_.in_shadow(hit.point, light_dir, dist)) {
                continue;
            }

            const double diff = std::max(0.0, dot(hit.normal, light_dir));

            color += hit.material->diffuse * light.color * diff;

            if (diff > 0.0) {
                Vec3 refl = reflect(-light_dir, hit.normal);
                double spec = std::pow(std::max(0.0, dot(view_dir, refl)), hit.material->shininess);
                color += hit.material->specular * light.color * spec;

            }
            }

            return color;
        }

        // TODO (PART 9): Trace one ray and return its colour, with recursion for
        // mirror reflections.
        //   Base case: if depth > max_depth_, return Vec3(0,0,0).
        //   Find the nearest hit with scene_.closest_hit(ray, 1e-4, infinity).
        //   If nothing is hit, return scene_.background.
        //   Otherwise color = shade(hit, ray). Let r = hit.material.reflectivity.
        //   If r > 0: build the reflected ray from the hit point (nudged along the
        //   normal by 1e-4) in direction reflect(ray.direction, hit.normal), then
        //   RECURSE: color = color * (1 - r) + trace(reflected, depth + 1) * r.
        //   Return color.
        
        
        Vec3 trace(const Ray &ray, int depth) const {
            if (depth > max_depth_) {
            return Vec3(0, 0, 0);
            }

            auto nearest_hit = scene_.closest_hit(ray, 1e-4, std::numeric_limits<double>::infinity());

            if (!nearest_hit) {
            return scene_.background();
            }

            Vec3 color = shade(nearest_hit.value(), ray);

            double r = nearest_hit->material->reflectivity;

            if (r > 0.0) {
            const Ray reflected(nearest_hit->point + nearest_hit->normal * 1e-4, reflect(ray.direction, nearest_hit->normal));

            color = color * (1.0 - r) + trace(reflected, depth + 1) * r;
            }

            return color;

        }

    };

}
