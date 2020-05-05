#pragma once

#include "hittable.h"

class Translate : public Hittable
{
    Hittable* me_;
    vec3 offset_;

public:
    Translate() = delete;
    Translate(Hittable* h, const vec3& displacement)
        : me_(h)
        , offset_(displacement)
    {
    }
    bool hit(const Ray& r, float tMin, float tMax, HitInfo& info) const
    {
        Ray xlatR(r.origin() - offset_, r.direction(), r.time());
        bool gotHit = me_->hit(xlatR, tMin, tMax, info);
        if (gotHit)
        {
            info.point += offset_;
        }
        return gotHit;
    }
    AABB getBounds(float time0, float time1) const
    {
        AABB bounds = me_->getBounds(time0, time1);
        return AABB(bounds.min() + offset_, bounds.max() + offset_);
    }
};

class RotateY : public Hittable
{
    Hittable* me_;
    float sinTheta_;
    float cosTheta_;
    AABB bounds_;
public:
    RotateY(Hittable* h, float angle)
        : me_(h)
    {
        float radians = (M_PI / 180.0f) * angle;
        sinTheta_ = sin(radians);
        cosTheta_ = cos(radians);
        bounds_ = me_->getBounds(0.0f, 1.0f);
        vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
        vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        const vec3& boundsMin = bounds_.min();
        const vec3& boundsMax = bounds_.max();
        for (unsigned int i = 0; i < 2; i++)
        {
            for (unsigned int j = 0; j < 2; j++)
            {
                for (unsigned int k = 0; k < 2; k++)
                {
                    float x = i * boundsMax.x() + (1 - i) * boundsMin.x();
                    float y = j * boundsMax.y() + (1 - j) * boundsMin.y();
                    float z = k * boundsMax.z() + (1 - k) * boundsMin.z();
                    float newX = cosTheta_ * x + sinTheta_ * z;
                    float newZ = -sinTheta_ * x + cosTheta_ * z;
                    vec3 temp(newX, y, newZ);
                    max.x((temp.x() > max.x()) ? temp.x() : max.x());
                    min.x((temp.x() < min.x()) ? temp.x() : min.x());
                    max.y((temp.y() > max.y()) ? temp.y() : max.y());
                    min.y((temp.y() < min.y()) ? temp.y() : min.y());
                    max.z((temp.z() > max.z()) ? temp.z() : max.z());
                    min.z((temp.z() < min.z()) ? temp.z() : min.z());
                }
            }
        }
        bounds_ = AABB(min, max);
    }
    bool hit(const Ray& r, float tMin, float tMax, HitInfo& info) const
    {
        float oX = cosTheta_ * r.origin().x() - sinTheta_ * r.origin().z();
        float oZ = sinTheta_ * r.origin().x() + cosTheta_ * r.origin().z();
        float dX = cosTheta_ * r.direction().x() - sinTheta_ * r.direction().z();
        float dZ = sinTheta_ * r.direction().x() + cosTheta_ * r.direction().z();
        vec3 origin(oX, r.origin().y(), oZ);
        vec3 direction(dX, r.direction().y(), dZ);
        Ray rotated(origin, direction, r.time());
        bool gotHit = me_->hit(rotated, tMin, tMax, info);
        if (gotHit)
        {
            float pX = cosTheta_ * info.point.x() + sinTheta_ * info.point.z();
            float pZ = -sinTheta_ * info.point.x() + cosTheta_ * info.point.z();
            float nX = cosTheta_ * info.normal.x() + sinTheta_ * info.normal.z();
            float nZ = -sinTheta_ * info.normal.x() + cosTheta_ * info.normal.z();
            info.point.x(pX);
            info.point.z(pZ);
            info.normal.x(nX);
            info.normal.z(nZ);
        }
        return gotHit;
    }
    AABB getBounds(float time0, float time1) const
    {
        return bounds_;
    }
};
