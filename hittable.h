#pragma once

class Material;

struct HitInfo
{
    float t;
    vec3 point;
    vec3 normal;
    Material* material;
};

class Hittable
{
public:
    virtual bool hit(const Ray& r, float tMin, float tMax, HitInfo& info) const = 0;
};

class HittableSet : public Hittable
{
    std::vector<Hittable*> hittables_;
public:
    HittableSet() {}
    void add(Hittable* hittable)
    {
        hittables_.push_back(hittable);
    }
    bool hit(const Ray& r, float tMin, float tMax, HitInfo& info) const
    {
        HitInfo tempInfo;
        bool hitAnything(false);
        double closestHit(tMax);

        for (auto hittable : hittables_)
        {
            if (hittable->hit(r, tMin, closestHit, tempInfo))
            {
                hitAnything = true;
                closestHit = tempInfo.t;
                info = tempInfo;
            }
        }

        return hitAnything;
    }
};
