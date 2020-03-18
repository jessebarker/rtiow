#pragma once

#include "random.h"

class Material
{
public:
    virtual bool scatter(const Ray& ray, const HitInfo& info, vec3& attenuation, Ray& scattered) const = 0;
    virtual vec3 emitted(const vec2& uv, const vec3& point) const
    {
        return vec3();
    }
    static vec3 reflect(const vec3& v, const vec3& n)
    {
        return v - 2.0f * vec3::dot(v, n) * n;
    }
    mutable RandomGenerator rg_;
};
