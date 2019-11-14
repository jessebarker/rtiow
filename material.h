#pragma once

#include "random.h"

class Material
{
public:
    virtual bool scatter(const Ray& ray, const HitInfo& info, vec3& attenuation, Ray& scattered) const = 0;
    mutable RandomGenerator rg_;
};
