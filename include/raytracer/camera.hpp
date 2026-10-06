#pragma once

#include <cmath>
#include <numbers>

#include <raytracer/vec3.hpp>
#include <raytracer/ray.hpp>

namespace raytracer {
    
    class Camera {
        
        private:
        Vec3 eye_, forward_, horizontal_, vertical_;
        
        public:
        
        // TODO: In the constructor body, build the camera basis and image-plane
        // vectors from the members initialised below (eye_ is already stored):
        //     aspect  = width / height
        //     theta   = vfov_deg * pi / 180        // use std::numbers::pi_v<double>
        //     half_h  = tan(theta / 2);   half_w = aspect * half_h
        //     forward_ = (look_at - eye).normalized()
        //     right_   = cross(forward_, up).normalized()
        //     true_up_ = cross(right_, forward_)
        //     horizontal_ = right_ * half_w;   vertical_ = true_up_ * half_h
        
        Camera(const Vec3 &eye, const Vec3 &look_at, const Vec3 &up, double vfov_deg, int image_width, int image_height) : eye_(eye) {
                const double aspect = static_cast<double>(image_width) / image_height;
                const double theta = vfov_deg * std::numbers::pi_v<double> / 180;
                
                const double half_h = std::tan(theta / 2);
                const double half_w = aspect * half_h;

                forward_ = (look_at - eye).normalized();

                const Vec3 right = cross(forward_, up).normalized();
                const Vec3 true_up = cross(right, forward_);

                horizontal_ = right * half_w;
                vertical_ = true_up * half_h;
        }

        // TODO: Return the ray through pixel coordinate (s, t), where s and t are in
        // [0, 1] with (0, 0) at the top-left. Map them to [-1, 1] (flip t so row 0 is
        // the top):  u = 2s - 1,  v = 1 - 2t.  Then
        //     direction = forward_ + horizontal_ * u + vertical_ * v
        // and return Ray(eye_, direction).
        
        Ray ray_through(double s, double t) const {
            const double u = 2.0 * s - 1;
            const double v = 1.0 - 2.0 * t;

            const Vec3 direction = forward_ + horizontal_ * u + vertical_ * v;

            return Ray(eye_, direction);
        }
    };

}
