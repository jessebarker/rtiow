#pragma once

#include "material.h"

class Lambertian : public Material
{
    vec3 albedo_;
public:
    Lambertian(const vec3& albedo)
        : albedo_(albedo)
    {
    }
    virtual bool scatter(const Ray& ray, const HitInfo& info, vec3& attenuation, Ray& scattered) const
    {
        vec3 target = info.point + info.normal + rg_.getPointInUnitSphere();
        scattered = Ray(info.point, target - info.point, ray.time());
        attenuation = albedo_;
        return true; 
    }
};
