#pragma once
#include <cmath>

struct Vec3 {
    float x{}, y{}, z{};
    constexpr Vec3 operator+(const Vec3& v) const { return {x+v.x,y+v.y,z+v.z}; }
    constexpr Vec3 operator-(const Vec3& v) const { return {x-v.x,y-v.y,z-v.z}; }
    constexpr Vec3 operator*(float s) const { return {x*s,y*s,z*s}; }
    constexpr Vec3 operator/(float s) const { return {x/s,y/s,z/s}; }
    Vec3& operator+=(const Vec3& v) { x+=v.x; y+=v.y; z+=v.z; return *this; }
    Vec3& operator-=(const Vec3& v) { x-=v.x; y-=v.y; z-=v.z; return *this; }
    Vec3& operator*=(float s) { x*=s; y*=s; z*=s; return *this; }
};
inline constexpr Vec3 operator*(float s, const Vec3& v) { return v*s; }
inline float dot(const Vec3& a,const Vec3& b){return a.x*b.x+a.y*b.y+a.z*b.z;}
inline float lengthSq(const Vec3& v){return dot(v,v);}
inline float length(const Vec3& v){return std::sqrt(lengthSq(v));}

