#pragma once
#include "hittable.h"

class Sphere : public Hittable
{
    vec3 center_;
    float radius_;
public:

    Sphere()
        : radius_(0.0f) {}
    Sphere(const vec3& center, float radius)
        : center_(center)
        , radius_(radius)
    {
    }

    bool hit(const Ray& r, float tMin, float tMax, HitInfo& info) const
    {
        vec3 oc = r.origin() - center_;
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
                info.normal = (info.point - center_) / radius_;
                return true;
            }

            temp = (-b + sqrt(discriminant)) / a;
            if (temp < tMax && temp > tMin)
            {
                info.t = temp;
                info.point = r.pointAt(info.t);
                info.normal = (info.point - center_) / radius_;
                return true;
            }
        }

        return false;
    }
};
