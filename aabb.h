#pragma once

#include <limits>
#include "vec.h"

namespace aabb
{
inline float ffmin(float a, float b) { return (a < b) ? a : b; }
inline float ffmax(float a, float b) { return (a > b) ? a : b; }
}

class AABB
{
    vec3 min_;
    vec3 max_;

public:
    AABB()
    {
    }
    AABB(const vec3& a, const vec3& b)
        : min_(a)
        , max_(b)
    {
    }

    vec3 min() const { return min_; }
    vec3 max() const { return max_; }

    inline bool hit(const Ray& r, float tMin, float tMax) const
    {
        for (unsigned int a = 0; a < 3; a++)
        {
#define PIXAR_OPT
#ifdef PIXAR_OPT
            float invD = 1.0f / r.direction()[a];
            float t0 = (min()[a] - r.origin()[a]) * invD;
            float t1 = (max()[a] - r.origin()[a]) * invD;
            if (invD < 0.0f)
            {
                std::swap(t0, t1);
            }
            tMin = t0 > tMin ? t0 : tMin;
            tMax = t1 < tMax ? t1 : tMax;
            if (tMax <= tMin)
            {
                return false;
            }
#else // original
            float oa = r.origin()[a];
            float da = r.direction()[a];
            float t0 = aabb::ffmin((min()[a] - oa) / da, (max()[a] - oa) / da);
            float t1 = aabb::ffmax((min()[a] - oa) / da, (max()[a] - oa) / da);
            tMin = aabb::ffmax(t0, tMin);
            tMax = aabb::ffmin(t1, tMax);
            if (tMax <= tMin)
            {
                return false;
            }
#endif
        }
        return true;
    }
};

AABB surroundingBox(const AABB& box0, const AABB& box1)
{
    return AABB(vmin(box0.min(), box1.min()), vmax(box0.max(), box1.max()));
}
