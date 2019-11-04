#pragma once
#include <iostream>

class PPMImage
{
    unsigned int width_;
    unsigned int height_;
    unsigned int maxColor_;
public:
    PPMImage(unsigned int width, unsigned int height, unsigned int maxColor)
        : width_(width)
        , height_(height)
        , maxColor_(maxColor) {}
    void EmitHeader()
    {
        std::cout << "P3" << std::endl;
        std::cout << width_ << " " << height_ << std::endl;
        std::cout << maxColor_ << std::endl;
    }
    void EmitOneColor(unsigned int red, unsigned int green, unsigned int blue)
    {
        std::cout << red << " " << green << " " << blue << std::endl;
    }
};
