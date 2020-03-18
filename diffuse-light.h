#pragma once

#include "material.h"

class Texture;

class DiffuseLight : public Material
{
    Texture* emissive_;
public:
    DiffuseLight(Texture* t)
        : emissive_(t)
    {
    }
    virtual bool scatter(const Ray& ray, const HitInfo& info, vec3& attenuation, Ray& scattered) const
    {
        return false;
    }
    virtual vec3 emitted(const vec2& uv, const vec3& point) const
    {
        return emissive_->value(uv, point);
    }
};
