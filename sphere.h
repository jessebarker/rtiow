#pragma once

#include "hittable.h"

class Sphere : public Hittable
{
    vec3 center0_;
    vec3 center1_;
    float time0_;
    float time1_;
    float radius_;
    Material* material_;

    vec3 center(float time) const
    {
        return center0_ + ((time - time0_) / (time1_ - time0_)) * (center1_ - center0_);
    }
public:
    // Initialize a static sphere
    Sphere(const vec3& center, float radius, Material* material)
        : center0_(center)
        , center1_(center)
        , time0_(0.0f)
        , time1_(1.0f)
        , radius_(radius)
        , material_(material)
    {
    }
    // Initialize a moving sphere
    Sphere(const vec3& center0, const vec3& center1, float t0, float t1,
           float radius, Material* material)
        : center0_(center0)
        , center1_(center1)
        , time0_(t0)
        , time1_(t1)
        , radius_(radius)
        , material_(material)
    {
    }
    bool hit(const Ray& r, float tMin, float tMax, HitInfo& info) const
    {
        vec3 centerAt = center(r.time());
        vec3 oc = r.origin() - centerAt;
        float a = vec3::dot(r.direction(), r.direction());
        float b = vec3::dot(oc, r.direction());
        float c = vec3::dot(oc, oc) - radius_ * radius_;
        float discriminant = b * b - a * c;

        if (discriminant > 0)
        {
            float temp = (-b - sqrt(discriminant)) / a;
            if (temp < tMax && temp > tMin)
            {
                info.t = temp;
                info.point = r.pointAt(info.t);
                info.normal = (info.point - centerAt) / radius_;
                info.material = material_;
                return true;
            }

            temp = (-b + sqrt(discriminant)) / a;
            if (temp < tMax && temp > tMin)
            {
                info.t = temp;
                info.point = r.pointAt(info.t);
                info.normal = (info.point - centerAt) / radius_;
                info.material = material_;
                return true;
            }
        }

        return false;
    }
    AABB getBounds(float time0, float time1) const
    {
        vec3 centerAt0 = center(time0);
        vec3 centerAt1 = center(time1);
        vec3 radius(radius_);
        AABB box0(centerAt0 - radius, centerAt0 + radius);
        AABB box1(centerAt1 - radius, centerAt1 + radius);
        return surroundingBox(box0, box1);
    }
};
