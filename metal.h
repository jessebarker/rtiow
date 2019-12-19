#pragma once

#include "material.h"

class Metal : public Material
{
    vec3 albedo_;
    float fuzz_;
public:
    Metal(const vec3& albedo, float fuzz = 0.0f)
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
        vec3 reflected = reflect(direction, info.normal);
        scattered = Ray(info.point, reflected + fuzz_ * rg_.getPointInUnitSphere());
        attenuation = albedo_;
        return (vec3::dot(scattered.direction(), info.normal) > 0);
    }
};
