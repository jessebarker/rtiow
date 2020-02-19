#pragma once

#include "material.h"
#include "texture.h"

class Lambertian : public Material
{
    Texture* albedo_;
public:
    Lambertian() = delete;
    Lambertian(Texture* albedo)
        : albedo_(albedo)
    {
    }
    virtual bool scatter(const Ray& ray, const HitInfo& info, vec3& attenuation, Ray& scattered) const
    {
        vec3 target = info.point + info.normal + rg_.getPointInUnitSphere();
        scattered = Ray(info.point, target - info.point, ray.time());
        attenuation = albedo_->value(0.0f, 0.0f, info.point);
        return true; 
    }
};
