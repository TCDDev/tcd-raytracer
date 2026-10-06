#pragma once

#include <cmath>
#include <cstdlib>
#include <cassert>

namespace raytracer {

    struct Vec3 {
    
        double x = 0.0, y = 0.0, z = 0.0;
        
        Vec3() = default;
        Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

        // Negation:  -v
        Vec3 operator-() const { return {-x, -y, -z}; } 
        
        // Vector addition / subtraction:  a + b ,  a - b
        Vec3 operator+(const Vec3 &v) const { return {x + v.x, y + v.y, z + v.z}; } 
        Vec3 operator-(const Vec3 &v) const { return {x - v.x, y - v.y, z - v.z}; } 
        
        // Component-wise product (handy for modulating colours):  a * b
        Vec3 operator*(const Vec3 &v) const { return {x * v.x, y * v.y, z * v.z}; } 
        
        // Scaling by a scalar:  v * s   and   v / s
        Vec3 operator*(double s) const { return {x * s, y * s, z * s}; } 
        Vec3 operator/(double s) const {
            const double mult_inverse = 1.0 / s;
            return *this * mult_inverse;
        } 
 
        // Compound addition:  a += b   (return *this)
        Vec3& operator+=(const Vec3 &v) {
            x += v.x;
            y += v.y;
            z += v.z;

            return *this;
        }   
  
        // Squared length, length, and a unit vector in the same direction.
        double length_squared() const { return x * x + y * y + z * z; } 
        double length() const { return std::sqrt(length_squared()); }          
        Vec3 normalized() const { return *this / length(); }        
    };


    // scalar * vector (so both v*s and s*v work). TODO: return v * s
    inline Vec3 operator*(double s, const Vec3 &v) { return v * s; }

    // Dot product. TODO
    inline double dot(const Vec3 &a, const Vec3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

    // Cross product. TODO:  (ay*bz - az*by, az*bx - ax*bz, ax*by - ay*bx)
    inline Vec3 cross(const Vec3 &a, const Vec3 &b) { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; }

    // Convenience function for checking if a vector is a unit vector

    inline bool is_unit_vector(const Vec3& v, double epsilon = 1e-9) {
        return std::abs(v.length_squared() - 1.0) < epsilon;
    }

    //reflection formula:  r = d - 2 (d . n) n
    // (n is assumed to be a unit vector.)
    inline Vec3 reflect(const Vec3 &d, const Vec3 &n) {
        assert(is_unit_vector(n) && "reflect(): n must be a unit vector");
        return d - 2 * dot(d, n) * n;
    }
}