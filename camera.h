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
    // Provided values
    float time0_;
    float time1_;
    float vFov_;
    float aspect_;
    float aperture_;
    float focalDistance_;
    vec3 from_;
    vec3 to_;
    vec3 up_;
    // Computed values
    float lensRadius_;
    vec3 horizontal_;
    vec3 vertical_;
    vec3 lowerLeft_;
    vec3 u_;
    vec3 v_;
    vec3 w_;
    RandomGenerator rg_;
public:
    Camera()
        : time0_(0.0f)
        , time1_(0.0f)
        , vFov_(40.0f)
        , aspect_(1.0f)
        , aperture_(0.0f)
        , focalDistance_(1.0f)
        , lensRadius_(0.0f)
    {
    }
    void applySettings()
    {
        float theta = vFov_ * M_PI / 180.0f;
        float halfHeight = tan(theta / 2.0f);
        float halfWidth = aspect_ * halfHeight;
        w_ = from_ - to_;
        w_.normalize();
        u_ = vec3::cross(up_, w_);
        u_.normalize();
        v_ = vec3::cross(w_, u_);
        lowerLeft_ = from_ - halfWidth * focalDistance_ * u_ - halfHeight * focalDistance_ * v_ - focalDistance_ * w_;
        horizontal_ = 2.0f * halfWidth * focalDistance_ * u_;
        vertical_ = 2.0f * halfHeight * focalDistance_ * v_;
    }
    void setAspect(float aspect)
    {
        aspect_ = aspect;
    }
    void setFov(float vFov)
    {
        vFov_ = vFov;
    }
    void setOrientation(const vec3& from, const vec3& to, const vec3& up)
    {
        from_ = from;
        to_ = to;
        up_ = up;
    }
    void setExposure(float time0, float time1)
    {
        time0_ = time0;
        time1_ = time1;
    }
    void setFocus(float aperture, float focalDistance)
    {
        aperture_ = aperture;
        focalDistance_ = focalDistance;
    }
    Ray getRay(float u, float v)
    {
        vec3 rd = lensRadius_ * randomInUnitDisc();
        vec3 offset = u_ * rd.x() + v_ * rd.y();
        float time = time0_ + rg_.getZeroToOne() * (time1_ - time0_);
        return Ray(from_ + offset, lowerLeft_ + u * horizontal_ + v * vertical_ - from_ - offset, time);
    }
};
