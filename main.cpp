#include "ppm.h"

int main(int argc, char** argv)
{
    const unsigned int width = 200;
    const unsigned int height = 100;
    const unsigned int maxUIColor = 255;
    PPMImage ppm(width, height, maxUIColor);
    ppm.EmitHeader();
    const float b = 0.2;
    const float maxColor = 255.99f;
    const unsigned int ub = static_cast<unsigned int>(maxColor * b);
    //const unsigned int maxUInt = 0 - 1;
    for (unsigned int y = height - 1; y >= 0 && y < height; y--)
    {
        float g = float(y) / float(height);
        unsigned int ug = static_cast<unsigned int>(maxColor * g);
        for (unsigned int x = 0; x < width; x++)
        {
            float r = float(x) / float(width);
            unsigned int ur = static_cast<unsigned int>(maxColor * r);
            ppm.EmitOneColor(ur, ug, ub);
        }
    }
}
