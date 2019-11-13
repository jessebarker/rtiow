#pragma once

#include <random>
#include "ray.h"

class Camera
{
    vec3 origin_;
    vec3 horizontal_;
    vec3 vertical_;
    vec3 lowerLeft_;
public:
    Camera()
        : origin_(0.0f)
        , horizontal_(4.0f, 0.0f, 0.0f)
        , vertical_(0.0f, 2.0f, 0.0f)
        , lowerLeft_(-2.0f, -1.0f, -1.0f)
    {
    }
    Ray GetRay(float u, float v)
    {
        return Ray(origin_, lowerLeft_ + u * horizontal_ + v * vertical_ - origin_);
    }
};
