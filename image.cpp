#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "image.h"

bool Image::load(const char* filename)
{
    int w;
    int h;
    int c;
    pixels_ = stbi_load(filename, &w, &h, &c, 0);
    if (pixels_ == nullptr)
    {
        return false;
    }
    dimensions_.x(w);
    dimensions_.y(h);
    components_ = c;
    return true;
}

bool Image::store(const char* filename, int width, int height, int components, unsigned char* pixels)
{
    return (stbi_write_png(filename, width, height, components, pixels, 0) > 0);
}

void Image::release(unsigned char* pixels)
{
    stbi_image_free(pixels);
}
