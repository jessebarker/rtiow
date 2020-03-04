#pragma once

class Image
{
    unsigned char* image_;
    void release(unsigned char* image);
public:
    Image()
        : image_(nullptr)
    {
    }
    ~Image()
    {
        release(image_);
    }
    bool load(const char* filename, int* width, int* height, int* components);
    static bool store(const char* filename, int width, int height, int components, unsigned char* image);
};
