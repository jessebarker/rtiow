#pragma once

#include "ray.h"

class Camera
{
    vec3 origin_;
    vec3 horizontal_;
    vec3 vertical_;
    vec3 lowerLeft_;
public:
    Camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, float vFov, float aspect) // vFov is top to bottom in degrees
        : origin_(lookFrom)
    {
        float theta = vFov * M_PI / 180.0f;
        float halfHeight = tan(theta / 2.0f);
        float halfWidth = aspect * halfHeight;
        vec3 w = (lookFrom - lookAt);
        w.normalize();
        vec3 u = vec3::cross(vUp, w);
        u.normalize();
        vec3 v = vec3::cross(w, u);
        lowerLeft_ = origin_ - halfWidth * u - halfHeight * v - w;
        horizontal_ = 2.0f * halfWidth * u;
        vertical_ = 2.0f * halfHeight * v;
    }
    Ray getRay(float u, float v)
    {
        return Ray(origin_, lowerLeft_ + u * horizontal_ + v * vertical_ - origin_);
    }
};
