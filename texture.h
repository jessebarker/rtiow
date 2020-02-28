#pragma once

#include "perlin.h"

class Texture
{
public:
    virtual vec3 value(float u, float v, const vec3& point) const = 0;
};

class ConstantTexture : public Texture
{
    vec3 color_;
public:
    ConstantTexture() = delete;
    ConstantTexture(const vec3& c) : color_(c)
    {
    }
    virtual vec3 value(float u, float v, const vec3& point) const
    {
        return color_;
    }
};

class CheckerTexture : public Texture
{
    Texture* odd_;
    Texture* even_;
public:
    CheckerTexture() = delete;
    CheckerTexture(Texture* even, Texture* odd)
        : odd_(odd)
        , even_(even)
    {
    }
    virtual vec3 value(float u, float v, const vec3& point) const
    {
        float sines = sin(10.0f * point.x()) * sin(10.0f * point.y()) * sin(10.0f * point.z());
        if (sines < 0.0f)
        {
            return odd_->value(u, v, point);
        }
        return even_->value(u, v, point);
    }
};

class NoiseTexture : public Texture
{
    Perlin perlin_;
public:
    NoiseTexture(float scale = 1.0f)
        : perlin_(scale)
    {
    }
    virtual vec3 value(float u, float v, const vec3& point) const
    {
        //return vec3(1.0f) * perlin_.noise(point);
        //return vec3(1.0f) * 0.5f * (1.0f + sin(perlin_.scale() * point.z() + 10.0f * perlin_.turbulence(point)));
        return vec3(1.0f) * 0.5f * (1.0f + sin(perlin_.scale() * point.z() + 10.0f * perlin_.turbulence(point * (1.0f / perlin_.scale()))));
    }
};
