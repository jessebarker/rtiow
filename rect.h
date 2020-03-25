#pragma once

#include "hittable.h"

class RectXY : public Hittable
{
    vec3 min_;
    vec3 max_;
    float k_;
    AABB bounds_;
    Material* material_;
public:
    RectXY(const vec2& min, const vec2& max, float k, Material* material)
        : k_(k)
        , material_(material)
    {
        min_ = vec3(min.x(), min.y(), k_ - 0.0001f);
        max_ = vec3(max.x(), max.y(), k_ + 0.0001f);
        bounds_ = AABB(min_, max_);
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

class RectXZ : public Hittable
{
    vec3 min_;
    vec3 max_;
    float k_;
    AABB bounds_;
    Material* material_;
public:
    RectXZ(const vec2& min, const vec2& max, float k, Material* material)
        : k_(k)
        , material_(material)
    {
        min_ = vec3(min.x(), k_ - 0.0001f, min.y());
        max_ = vec3(max.x(), k_ + 0.0001f, max.y());
        bounds_ = AABB(min_, max_);
    }
    virtual bool hit(const Ray& ray, float t0, float t1, HitInfo& info) const
    {
        const vec3& origin = ray.origin();
        const vec3& direction = ray.direction();
        float t = (k_ - origin.y()) / direction.y();
        if (t < t0 || t > t1)
        {
            return false;
        }
        float x = origin.x() + t * direction.x();
        float z = origin.z() + t * direction.z();
        if (x < min_.x() || x > max_.x() || z < min_.z() || z > max_.z())
        {
            return false;
        }
        info.uv = vec2((x - min_.x()) / (max_.x() - min_.x()), (z - min_.z()) / (max_.z() - min_.z()));
        info.t = t;
        info.material = material_;
        info.point = ray.pointAt(t);
        info.normal = vec3(0.0f, 1.0f, 0.0f);
        return true;
    }
    virtual AABB getBounds(float t0, float t1) const
    {
        return bounds_;
    }
};

class RectYZ : public Hittable
{
    vec3 min_;
    vec3 max_;
    float k_;
    AABB bounds_;
    Material* material_;
public:
    RectYZ(const vec2& min, const vec2& max, float k, Material* material)
        : k_(k)
        , material_(material)
    {
        min_ = vec3(k_ - 0.0001f, min.x(), min.y());
        max_ = vec3(k_ + 0.0001f, max.x(), max.y());
        bounds_ = AABB(min_, max_);
    }
    virtual bool hit(const Ray& ray, float t0, float t1, HitInfo& info) const
    {
        const vec3& origin = ray.origin();
        const vec3& direction = ray.direction();
        float t = (k_ - origin.x()) / direction.x();
        if (t < t0 || t > t1)
        {
            return false;
        }
        float y = origin.y() + t * direction.y();
        float z = origin.z() + t * direction.z();
        if (y < min_.y() || y > max_.y() || z < min_.z() || z > max_.z())
        {
            return false;
        }
        info.uv = vec2((y - min_.y()) / (max_.y() - min_.y()), (z - min_.z()) / (max_.z() - min_.z()));
        info.t = t;
        info.material = material_;
        info.point = ray.pointAt(t);
        info.normal = vec3(1.0f, 0.0f, 0.0f);
        return true;
    }
    virtual AABB getBounds(float t0, float t1) const
    {
        return bounds_;
    }
};
