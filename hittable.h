#pragma once

struct HitInfo
{
    float t;
    vec3 point;
    vec3 normal;
};

class Hittable
{
public:
    virtual bool hit(const Ray& r, float tMin, float tMax, HitInfo& info) const = 0;
};

class HittableList : public Hittable
{
    Hittable** list_;
    unsigned int size_;
public:
    HittableList() : list_(nullptr), size_(0) {}
    HittableList(Hittable** list, unsigned int nElements) : list_(list), size_(nElements) {}
    bool hit(const Ray& r, float tMin, float tMax, HitInfo& info) const
    {
        HitInfo tempInfo;
        bool hitAnything(false);
        double closestHit(tMax);

        for (unsigned int i = 0; i < size_; i++)
        {
            if (list_[i]->hit(r, tMin, closestHit, tempInfo))
            {
                hitAnything = true;
                closestHit = tempInfo.t;
                info = tempInfo;
            }
        }

        return hitAnything;
    }
};
