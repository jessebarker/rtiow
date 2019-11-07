#include "ppm.h"
#include "ray.h"
#include "hittable.h"
#include "sphere.h"

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

    PPMImage ppm(width, height, maxUIColor);
    ppm.EmitHeader();

    const vec3 lowerLeft(-2.0f, -1.0f, -1.0f);
    const vec3 horizontal(4.0f, 0.0f, 0.0f);
    const vec3 vertical(0.0f, 2.0f, 0.0f);
    const vec3 origin(0.0f);
    const float maxColor(255.99f);
    Hittable* list[2];
    list[0] = new Sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f);
    list[1] = new Sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f);
    Hittable* world = new HittableList(list, 2);

    for (unsigned int y = height - 1; y >= 0 && y < height; y--)
    {
        float v = float(y) / float(height);
        for (unsigned int x = 0; x < width; x++)
        {
            float u = float(x) / float(width);
            Ray r(origin, lowerLeft + u * horizontal + v * vertical);
            vec3 point = r.pointAt(2.0f);
            vec3 color = colorAt(r, world);
            unsigned int ur = static_cast<unsigned int>(maxColor * color.x());
            unsigned int ug = static_cast<unsigned int>(maxColor * color.y());
            unsigned int ub = static_cast<unsigned int>(maxColor * color.z());
            ppm.EmitOneColor(ur, ug, ub);
        }
    }
}
