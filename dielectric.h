#pragma once

#include "material.h"

class Dielectric : public Material
{
    float ior_; // Index of refraction
    static bool refract(const vec3& v, const vec3& n, float niOverNt, vec3& refracted)
    {
        vec3 uv = v;
        uv.normalize();
        float dt = vec3::dot(uv, n);
        float discriminant = 1.0f - niOverNt * niOverNt * (1.0 - dt * dt);
        if (discriminant > 0.0f)
        {
            refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
            return true;
        }
        return false;
    }
    static float schlick(float cosine, float ior)
    {
        float r0 = (1.0f - ior) / (1.0f + ior);
        r0 = r0 * r0;
        return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
    }

public:
    Dielectric(float ior)
        : ior_(ior)
    {
    }
    virtual bool scatter(const Ray& r, const HitInfo& info, vec3& attenuation, Ray& scattered) const
    {
        attenuation = vec3(1.0f, 1.0f, 1.0f);
        vec3 reflected = reflect(r.direction(), info.normal);
        vec3 outwardNormal;
        float niOverNt;
        float cosine;

        if (vec3::dot(r.direction(), info.normal) > 0.0f)
        {
            outwardNormal = -1.0f * info.normal;
            niOverNt = ior_;
            cosine = ior_ * vec3::dot(r.direction(), info.normal) / r.direction().length();
        }
        else
        {
            outwardNormal = info.normal;
            niOverNt = 1.0f / ior_;
            cosine = -vec3::dot(r.direction(), info.normal) / r.direction().length();
        }

        vec3 refracted;
        float reflectP = 1.0f;
        if (refract(r.direction(), outwardNormal, niOverNt, refracted))
        {
            reflectP = schlick(cosine, ior_);
        }

        if (rg_.getZeroToOne() < reflectP)
        {
            scattered = Ray(info.point, reflected, r.time());
        }
        else
        {
            scattered = Ray(info.point, refracted, r.time());
        }

        return true;
    }
};
