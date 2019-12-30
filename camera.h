#pragma once

#include "ray.h"
#include "random.h"

class Camera
{
    vec3 randomInUnitDisc()
    {
        vec3 p;
        do
        {
            p = 2.0f * vec3(rg_.getZeroToOne(), rg_.getZeroToOne(), 0.0f) - vec3(1.0f, 1.0f, 0.0f);
        } while (vec3::dot(p, p) >= 1.0f);
        return p;
    }
    vec3 origin_;
    vec3 horizontal_;
    vec3 vertical_;
    vec3 lowerLeft_;
    vec3 u_;
    vec3 v_;
    vec3 w_;
    float time0_;
    float time1_;
    float lensRadius_;
    RandomGenerator rg_;
public:
    Camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, float vFov, // vFov is top to bottom in degrees
           float aspect, float aperture, float focusDistance,
           float t0, float t1) 
        : origin_(lookFrom)
        , time0_(t0)
        , time1_(t1)
        , lensRadius_(aperture / 2.0f)
    {
        float theta = vFov * M_PI / 180.0f;
        float halfHeight = tan(theta / 2.0f);
        float halfWidth = aspect * halfHeight;
        w_ = (lookFrom - lookAt);
        w_.normalize();
        u_ = vec3::cross(vUp, w_);
        u_.normalize();
        v_ = vec3::cross(w_, u_);
        lowerLeft_ = origin_ - halfWidth * focusDistance * u_ - halfHeight * focusDistance * v_ - focusDistance * w_;
        horizontal_ = 2.0f * halfWidth * focusDistance * u_;
        vertical_ = 2.0f * halfHeight * focusDistance * v_;
    }
    Ray getRay(float u, float v)
    {
        vec3 rd = lensRadius_ * randomInUnitDisc();
        vec3 offset = u_ * rd.x() + v_ * rd.y();
        float time = time0_ + rg_.getZeroToOne() * (time1_ - time0_);
        return Ray(origin_ + offset, lowerLeft_ + u * horizontal_ + v * vertical_ - origin_ - offset, time);
    }
};
