#pragma once

#include "libmatrix/vec.h"

class Ray
{
    vec3 origin_;
    vec3 direction_;
    float time_;
public:
    Ray() {}
    Ray(const vec3& origin, const vec3& direction, float t = 0.0f)
        : origin_(origin)
        , direction_(direction)
        , time_(t)
    {
    }
    const vec3& origin() const { return origin_; }
    const vec3& direction() const { return direction_; }
    float time() const { return time_; }
    vec3 pointAt(float t) const
    {
        return origin_ + t * direction_;
    }
};
