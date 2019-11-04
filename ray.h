#pragma once

#include "../libmatrix/vec.h"

class Ray
{
    vec3 origin_;
    vec3 direction_;
public:
    Ray() {}
    Ray(const vec3& origin, const vec3& direction)
        : origin_(origin)
        , direction_(direction) {}
    const vec3& origin() const { return origin_; }
    const vec3& direction() const { return direction_; }
    vec3 pointAt(float t) const
    {
        return origin_ + t * direction_;
    }
};
