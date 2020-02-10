#pragma once

#include "aabb.h"

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
    virtual AABB getBounds(float time0, float time1) const = 0;
};

struct BvhNode
{
    std::vector<Hittable*> hittables_;
    AABB bounds_;
    BvhNode* left_;
    BvhNode* right_;

    BvhNode()
        : left_(nullptr)
        , right_(nullptr)
    {
    }
    ~BvhNode()
    {
        delete left_;
        delete right_;
    }
    void add(float time0, float time1, Hittable* hittable)
    {
        hittables_.push_back(hittable);
        bounds_ = surroundingBox(bounds_, hittable->getBounds(time0, time1));
    }
    bool hit(const Ray& r, float tMin, float tMax, HitInfo& info) const
    {
        // If the ray misses us, we're done
        bool gotHit = false;
        HitInfo closestHit;
        if (!bounds_.hit(r, tMin, tMax))
        {
            return gotHit;
        }

        // We know the ray hits us, let's see if it hits a child
        HitInfo leftHit;
        HitInfo rightHit;
        bool gotLeft = left_ && left_->hit(r, tMin, tMax, leftHit);
        bool gotRight = right_ && right_->hit(r, tMin, tMax, rightHit);
        // closestHit.t = tMax; // Might this simplify the 'gotHit' logic below?
        if (gotLeft && gotRight)
        {
            closestHit = (leftHit.t < rightHit.t) ? leftHit : rightHit;
            gotHit = true;
        }
        else if (gotLeft)
        {
            closestHit = leftHit;
            gotHit = true;
        }
        else if (gotRight)
        {
            closestHit = rightHit;
            gotHit = true;
        }

        // Let's see if the ray hits any of our objects (should this come before the child check?)
        float tCur = (gotHit) ? closestHit.t : tMax;
        HitInfo currentHit;
        for (auto hittable : hittables_)
        {
            if (hittable->hit(r, tMin, tCur, currentHit))
            {
                // If we don't have anything yet, just assign this object's info
                if (!gotHit)
                {
                    gotHit = true;
                    closestHit = currentHit;
                    tCur = currentHit.t;
                }
                else if (currentHit.t < closestHit.t) // We had a hit, but this one is closer
                {
                    closestHit = currentHit;
                    tCur = currentHit.t;
                }
            }
        }

        if (gotHit)
        {
            info = closestHit;
        }

        return gotHit;
    }
    AABB getBounds(float t0, float t1) const
    {
        return bounds_;
    }
    bool isThisYours(Hittable* hittable) const
    {
        // Check this node first
        for (auto current : hittables_)
        {
            if (current == hittable)
            {
                return true;
            }
        }

        // Check the children
        bool leftHasIt = false;
        if (left_)
        {
            leftHasIt = left_->isThisYours(hittable);
        }
        bool rightHasIt = false;
        if (right_)
        {
            rightHasIt = right_->isThisYours(hittable);
        }
        return (leftHasIt || rightHasIt);
    }
};

class HittableSet : public Hittable
{
    BvhNode bvh_;
    std::vector<Hittable*> masterList_;
public:
    HittableSet() {}
    void add(Hittable* hittable)
    {
        bvh_.add(0.0f, 0.0f, hittable);
    }
    bool hit(const Ray& r, float tMin, float tMax, HitInfo& info) const
    {
        return bvh_.hit(r, tMin, tMax, info);
    }
    AABB getBounds(float time0, float time1) const
    {
        return bvh_.getBounds(time0, time1);
    }
#ifdef DEBUG_BVH_SORT
    bool anyoneLeftBehind()
    {
        bool status = false;
        for (auto hittable : masterList_)
        {
            if (!bvh_.isThisYours(hittable))
            {
                std::cerr << "HittableSet::anyoneLeftBehind: can't find 0x" << hittable << std::endl;
                status = true;
            }
        }
        return status;
    }
#endif // DEBUG_BVH_SORT
    void sortMe(float time0, float time1, BvhNode* root = nullptr)
    {
        if (root == nullptr)
        {
            root = &bvh_;
#ifdef DEBUG_BVH_SORT
            // Needed for debugging the sorting using "anyoneLeftBehind"
            for (auto hittable : root->hittables_)
            {
                masterList_.push_back(hittable);
            }
#endif // DEBUG_BVH_SORT
        }

        if (root->hittables_.empty())
        {
            std::cerr << "HittableSet::sortMe: root node has no hittables" << std::endl;
            return;
        }
        else if (root->hittables_.size() == 1)
        {
            return;
        }

        root->left_ = new BvhNode();
        root->right_ = new BvhNode();

        // Pick an axis
        unsigned int axis = 0;
        const AABB& nodeBounds = root->bounds_;
        vec3 nodeDims = nodeBounds.max() - nodeBounds.min();
        for (unsigned int i = 1; i < 3; i++)
        {
            if (nodeDims[axis] < nodeDims[i])
            {
                axis = i;
            }
        }

        // Add this node's hittables to left or right child as appropriate
        float nca = nodeBounds.min()[axis] + nodeDims[axis] / 2.0f;
        for (auto hittable : root->hittables_)
        {
            AABB hittableBounds = hittable->getBounds(time0, time1);
            vec3 hittableCenter = (hittableBounds.min() + hittableBounds.max()) * 0.5f;
            float hca = hittableCenter[axis];
            BvhNode* which = (hc < nc) ? root->left_ : root->right_;
            which->add(time0, time1, hittable);
        }

        // If either child has no hittables, then 'root' is the leaf on this branch
        // Destroy the children and get out.
        bool leftEmpty = root->left_->hittables_.empty();
        bool rightEmpty = root->right_->hittables_.empty();
        if (leftEmpty || rightEmpty)
        {
            delete root->left_;
            delete root->right_;
            root->left_ = nullptr;
            root->right_ = nullptr;
            return;
        }

        // Sort the children
        root->hittables_.clear();
        sortMe(time0, time1, root->left_);
        sortMe(time0, time1, root->right_);
    }
};
