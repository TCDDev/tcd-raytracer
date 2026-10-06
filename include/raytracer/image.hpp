#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

#include <stb_image_write.h>

#include <raytracer/vec3.hpp>

namespace raytracer {

    class Image {
    
        private:
        int width_, height_;
        std::vector<Vec3> pixels_;

        size_t index(int x, int y) const { return static_cast<size_t>(y) * width_ + x; }
 
  
        public:
        Image(int width, int height)
            : width_(width), height_(height), pixels_(static_cast<size_t>(width) * height) {}
        
        int width() const { return width_; }
        
        int height() const { return height_; }
        
        void set(int x, int y, const Vec3 &color) { pixels_[index(x, y)] = color; }
        
        const Vec3 &at(int x, int y) const { return pixels_[index(x, y)]; }
        
        bool save_png(const std::string &filename) const {
            std::vector<uint8_t> data;
            data.reserve(pixels_.size() * 3);

            for (const Vec3& pixel: pixels_) {
            std::array<double, 3> colors{pixel.x, pixel.y, pixel.z}; // RGB values in order

            for (double color : colors) {
                color = std::clamp(color, 0.0, 1.0);
                color = std::pow(color, 1.0 / 2.2); // gamma correction
                data.push_back(static_cast<uint8_t>(color * 255.0 + 0.5)); // quantization
            }

            }

            // Write the file, then return true if successful and false if not
            return stbi_write_png(filename.c_str(), width_, height_, 3, data.data(), width_ * 3) != 0;
        }

    };
}
