#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "image.h"

bool Image::load(const char* filename, int* width, int* height, int* components)
{
    image_ = stbi_load(filename, width, height, components, 0);
    return (image_ != nullptr);
}

bool Image::store(const char* filename, int width, int height, int components, unsigned char* image)
{
    return (stbi_write_png(filename, width, height, components, image, 0) > 0);
}

void Image::release(unsigned char* image)
{
    stbi_image_free(image);
}
