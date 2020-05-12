#pragma once

#include "hittable.h"

class Isotropic : public Material
{
    Texture* albedo_;
public:
    Isotropic(Texture* t)
        : albedo_(t)
    {
    }
    bool scatter(const Ray& r, const HitInfo& info, vec3& attenuation, Ray& scattered) const
    {
        scattered = Ray(info.point, rg_.getPointInUnitSphere());
        attenuation = albedo_->value(info.uv, info.point);
        return true;
    }
};

class ConstantMedium : public Hittable
{
    Hittable* me_;
    Material* material_;
    float density_;
    mutable RandomGenerator rg_;
public:
    ConstantMedium(Hittable* h, float density, Texture* albedo)
        : me_(h)
        , density_(density)
    {
        material_ = new Isotropic(albedo);
    }
    bool hit(const Ray& r, float tMin, float tMax, HitInfo& info) const
    {
        HitInfo hit1;
        HitInfo hit2;
        if (me_->hit(r, -FLT_MAX, FLT_MAX, hit1))
        {
            if (me_->hit(r, hit1.t + 0.0001f, FLT_MAX, hit2))
            {
                if (hit1.t < tMin) hit1.t = tMin;
                if (hit2.t > tMax) hit2.t = tMax;
                if (hit1.t >= hit2.t) return false;
                if (hit1.t < 0.0f) hit1.t = 0.0f;
                float rayLength = r.direction().length();
                float dInBoundary = (hit2.t - hit1.t) * rayLength;
                float hitDistance = -(1.0f / density_) * log(rg_.getZeroToOne());
                if (hitDistance < dInBoundary)
                {
                    info.t = hit1.t + hitDistance / rayLength;
                    info.point = r.pointAt(info.t);
                    info.normal = vec3(1.0f, 0.0f, 0.0f);
                    info.material = material_;
                    return true;
                }
            }
        }
        return false;
    }
    AABB getBounds(float time0, float time1) const
    {
        return me_->getBounds(time0, time1);
    }
};
