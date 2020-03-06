#pragma once

#include "vec.h"

class Image
{
    unsigned char* pixels_;
    uvec2 dimensions_;
    unsigned int components_;
    void release(unsigned char* image);
public:
    Image()
        : pixels_(nullptr)
    {
    }
    ~Image()
    {
        release(pixels_);
    }
    bool load(const char* filename);
    unsigned char* pixels() const
    {
        return pixels_;
    }
    const uvec2& dimensions() const
    {
        return dimensions_;
    }
    unsigned int components() const
    {
        return components_;
    }
    static bool store(const char* filename, int width, int height, int components, unsigned char* pixels);
};
