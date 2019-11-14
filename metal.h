#pragma once

#include "material.h"

class Metal : public Material
{
    vec3 albedo_;
    float fuzz_;
    vec3 Reflect(const vec3& v, const vec3& n) const
    {
        return v - 2.0f * vec3::dot(v, n) * n;
    }
public:
    Metal(const vec3& albedo, float fuzz)
        : albedo_(albedo)
        , fuzz_(fuzz)
    {
        if (fuzz_ > 1.0f)
        {
            fuzz_ = 1.0f;
        }
    }
    virtual bool scatter(const Ray& ray, const HitInfo& info, vec3& attenuation, Ray& scattered) const
    {
        vec3 direction = ray.direction();
        direction.normalize();
        vec3 reflected = Reflect(direction, info.normal);
        scattered = Ray(info.point, reflected + fuzz_ * rg_.getPointInUnitSphere());
        attenuation = albedo_;
        return (vec3::dot(scattered.direction(), info.normal) > 0);
    }
};
