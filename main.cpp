#include "ppm.h"
#include "ray.h"

bool hitSphere(const vec3& center, float radius, const Ray& r)
{
    vec3 oc = r.origin() - center;
    float a = vec3::dot(r.direction(), r.direction());
    float b = 2.0f * vec3::dot(oc, r.direction());
    float c = vec3::dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    return (discriminant > 0);
}

vec3 colorAt(const Ray& r)
{
    vec3 center(0.0, 0.0, -1.0);
    if (hitSphere(center, 0.5f, r))
    {
        return vec3(1.0f, 0.0f, 0.0f);
    }
    vec3 direction = r.direction();
    direction.normalize();
    float t = 0.5f * (direction.y() + 1.0f);
    return (1.0f - t) * vec3(1.0) + t * vec3(0.5, 0.7, 1.0);
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
    for (unsigned int y = height - 1; y >= 0 && y < height; y--)
    {
        float v = float(y) / float(height);
        for (unsigned int x = 0; x < width; x++)
        {
            float u = float(x) / float(width);
            Ray r(origin, lowerLeft + u * horizontal + v * vertical);
            vec3 color = colorAt(r);
            unsigned int ur = static_cast<unsigned int>(maxColor * color.x());
            unsigned int ug = static_cast<unsigned int>(maxColor * color.y());
            unsigned int ub = static_cast<unsigned int>(maxColor * color.z());
            ppm.EmitOneColor(ur, ug, ub);
        }
    }
}
