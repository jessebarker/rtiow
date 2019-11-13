#include "ppm.h"
#include "ray.h"
#include "sphere.h"
#include "camera.h"

vec3 colorAt(const Ray& r, const Hittable* world)
{
    HitInfo info;
    if (world->hit(r, 0.0, MAXFLOAT, info))
    {
        return 0.5f * (info.normal + 1.0f);
    }
    else
    {
        vec3 direction = r.direction();
        direction.normalize();
        float t = 0.5f * (direction.y() + 1.0f);
        return (1.0f - t) * vec3(1.0) + t * vec3(0.5f, 0.7f, 1.0f);
    }
}

int main(int argc, char** argv)
{
    const unsigned int width(200);
    const unsigned int height(100);
    const unsigned int maxUIColor(255);
    const float maxColor(255.99f);
    const unsigned int numSamples(100);

    PPMImage ppm(width, height, maxUIColor);
    ppm.EmitHeader();

    Hittable* list[2];
    list[0] = new Sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f);
    list[1] = new Sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f);
    Hittable* world = new HittableList(list, 2);
    Camera camera;
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    std::mt19937 generator;

    for (unsigned int y = height - 1; y >= 0 && y < height; y--)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            vec3 color;
            for (unsigned int s = 0; s < numSamples; s++)
            {
                float v = float(y + distribution(generator)) / float(height);
                float u = float(x + distribution(generator)) / float(width);
                Ray r = camera.GetRay(u, v);
                vec3 point = r.pointAt(2.0f);
                color += colorAt(r, world);
            }
            color /= float(numSamples);
            unsigned int ur = static_cast<unsigned int>(maxColor * color.x());
            unsigned int ug = static_cast<unsigned int>(maxColor * color.y());
            unsigned int ub = static_cast<unsigned int>(maxColor * color.z());
            ppm.EmitOneColor(ur, ug, ub);
        }
    }
}
