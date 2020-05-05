#pragma once

#include "rect.h" // Already includes "hittable.h"

class Box : public Hittable
{
    vec3 min_;
    vec3 max_;
    Material* material_;
    HittableSet set_;

public:
    Box() {};
    Box(const vec3& p0, const vec3& p1, Material* material)
        : min_(p0)
        , max_(p1)
        , material_(material)
    {
        set_.add(new RectXY(min_.xy(), max_.xy(), max_.z(), material_));
        set_.add(new FlipNormals(new RectXY(min_.xy(), max_.xy(), min_.z(), material_)));
        set_.add(new RectXZ(min_.xz(), max_.xz(), max_.y(), material_));
        set_.add(new FlipNormals(new RectXZ(min_.xz(), max_.xz(), max_.y(), material_)));
        set_.add(new RectYZ(min_.yz(), max_.yz(), max_.x(), material_));
        set_.add(new FlipNormals(new RectYZ(min_.yz(), max_.yz(), min_.x(), material_)));
    }
    bool hit(const Ray& r, float tMin, float tMax, HitInfo& info) const
    {
        return set_.hit(r, tMin, tMax, info);
    }
    AABB getBounds(float time0, float time1) const
    {
        return AABB(min_, max_);
    }
};
