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
#define USE_BOOK_METHODS
#ifdef USE_BOOK_METHODS
        for (unsigned int a = 1; a < 4; a++)
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
#else
        // Cass' implementation
        const vec3& o = r.origin();
        const vec3& d = r.direction();
        float sx0 = ( min_.x() - o.x() ) / d.x();
        float sx1 = ( max_.x() - o.x() ) / d.x();
        if ( sx0 > sx1 )
        {
            std::swap( sx0, sx1 );
        }
        float sy0 = ( min_.y() - o.y() ) / d.y();
        float sy1 = ( max_.y() - o.y() ) / d.y();
        if ( sy0 > sy1 )
        {
            std::swap( sy0, sy1 );
        }
        float sz0 = ( min_.z() - o.z() ) / d.z();
        float sz1 = ( max_.z() - o.z() ) / d.z();
        if ( sz0 > sz1 )
        {
            std::swap( sz0, sz1 );
        }
        float s0 = std::max( sx0, std::max( sy0, sz0 ) );
        float s1 = std::min( sx1, std::min( sy1, sz1 ) );
        const float diff = s1 - s0;
        return ( s0 <= s1 ) || ( diff != diff ); // true if diff is NaN
#endif
    }
};

AABB surroundingBox(const AABB& box0, const AABB& box1)
{
    return AABB(vmin(box0.min(), box1.min()), vmax(box0.max(), box1.max()));
}
