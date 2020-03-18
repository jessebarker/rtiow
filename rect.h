#pragma once

#include "hittable.h"

class Rect : public Hittable
{
    vec2 min_;
    vec2 max_;
    float k_;
    AABB bounds_;
    Material* material_;
public:
    Rect(const vec2& min, const vec2& max, float k, Material* material)
        : min_(min)
        , max_(max)
        , k_(k)
        , material_(material)
    {
        bounds_ = AABB(vec3(min_.x(), min.y(), k_ - 0.0001f), vec3(max_.x(), max.y(), k + 0.0001f));
    }
    virtual bool hit(const Ray& ray, float t0, float t1, HitInfo& info) const
    {
        const vec3& origin = ray.origin();
        const vec3& direction = ray.direction();
        float t = (k_ - origin.z()) / direction.z();
        if (t < t0 || t > t1)
        {
            return false;
        }
        float x = origin.x() + t * direction.x();
        float y = origin.y() + t * direction.y();
        if (x < min_.x() || x > max_.x() || y < min_.y() || y > max_.y())
        {
            return false;
        }
        info.uv = vec2((x - min_.x()) / (max_.x() - min_.x()), (y - min_.y()) / (max_.y() - min_.y()));
        info.t = t;
        info.material = material_;
        info.point = ray.pointAt(t);
        info.normal = vec3(0.0f, 0.0f, 1.0f);
        return true;
    }
    virtual AABB getBounds(float t0, float t1) const
    {
        return bounds_;
    }
};
