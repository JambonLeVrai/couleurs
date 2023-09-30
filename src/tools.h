#ifndef TOOLS_H
#define TOOLS_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#ifndef PI
#define PI 3.14159265359
#endif

template <class T>
class Vec2 {
    public:
        Vec2(T x, T y): _x(x), _y(y) {}
        
        Vec2<T> operator+(Vec2 const& a) const {
            return Vec2<T>(_x + a.x(), _y + a.y());
        }

        Vec2<T> operator-(Vec2 const& a) const {
            return Vec2<T>(_x - a.x(), _y - a.y());
        }

        Vec2<T> operator*(T const& a) const {
            return Vec2<T>(a*_x, a*_y);
        }

        Vec2<T> operator/(T const& a) const {
            return Vec2<T>(_x/a, _y/a);
        }
        
        T x() const { return _x; }
        T y() const { return _y; }

        static T dot(Vec2 const& a, Vec2 const& b) {
            return a.x() * b.x() + a.y() * b.y();
        }
        static T sq_norm(Vec2 const& a){
            return dot(a, a);
        }
    
    private:
        T _x, _y;
};

template <class T>
class Vec3 {
    public:
        Vec3(T x, T y, T z) : _x(x), _y(y), _z(z) {}
        
        Vec3<T> operator+(Vec3 const& a) const {
            return Vec3<T>(_x + a.x(), _y + a.y(), _z + a.z());
        }

        Vec3<T> operator-(Vec3 const& a) const {
            return Vec3<T>(_x - a.x(), _y - a.y(), _z - a.z());
        }

        Vec3<T> operator*(T const& a) const {
            return Vec3<T>(a*_x, a*_y, a*_z);
        }

        Vec3<T> operator/(T const& a) const {
            return Vec3<T>(_x/a, _y/a, _z/a);
        }
        
        T x() const { return _x; }
        T y() const { return _y; }
        T z() const { return _z; }

        static T dot(Vec3 const& a, Vec3 const& b) {
            return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
        }
        static T sq_norm(Vec3 const& a) {
            return dot(a, a);
        }
    
    private:
        T _x, _y, _z;
};

using Vec2f = Vec2<float>;
using ColorF = Vec3<float>;

float deg_to_radians(float deg);
float radians_to_deg(float radians);
Vec2f vec_from_angle(float deg);

ColorF color_from_rgb(uint8_t R, uint8_t G, uint8_t B);
uint32_t color_to_rgb(ColorF const& color);

#endif