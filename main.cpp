#include "ppm.h"
#include "ray.h"
#include "sphere.h"
#include "camera.h"
#include "random.h"

namespace
{
RandomGenerator rg;

vec3 randomPointInUnitSphere()
{
    vec3 point;
    float length = 0.0f;
    do
    {
        point = 2.0f * vec3(rg.GetMinusOneToOne(), rg.GetMinusOneToOne(), rg.GetMinusOneToOne()) - vec3(1.0f);
        length = point.length();
    } while ((length * length) >= 1.0f);
    return point;
}

vec3 colorAt(const Ray& r, const Hittable* world)
{
    HitInfo info;
    if (world->hit(r, 0.001f, MAXFLOAT, info))
    {
        vec3 target = info.point + info.normal + randomPointInUnitSphere();
        return 0.5f * colorAt(Ray(info.point, target - info.point), world);
    }
    else
    {
        vec3 direction = r.direction();
        direction.normalize();
        float t = 0.5f * (direction.y() + 1.0f);
        return (1.0f - t) * vec3(1.0) + t * vec3(0.5f, 0.7f, 1.0f);
    }
}

void ApplyGamma(vec3& linearColor)
{
    const float gammaPower(1.0f / 2.2f);
    linearColor.x(pow(linearColor.x(), gammaPower));
    linearColor.y(pow(linearColor.y(), gammaPower));
    linearColor.z(pow(linearColor.z(), gammaPower));
}

} // namespace

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

    for (unsigned int y = height - 1; y >= 0 && y < height; y--)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            vec3 color;
            for (unsigned int s = 0; s < numSamples; s++)
            {
                float v = float(y + rg.GetZeroToOne()) / float(height);
                float u = float(x + rg.GetZeroToOne()) / float(width);
                Ray r = camera.GetRay(u, v);
                vec3 point = r.pointAt(2.0f);
                color += colorAt(r, world);
            }
            color /= float(numSamples);
            ApplyGamma(color);
            unsigned int ur = static_cast<unsigned int>(maxColor * color.x());
            unsigned int ug = static_cast<unsigned int>(maxColor * color.y());
            unsigned int ub = static_cast<unsigned int>(maxColor * color.z());
            ppm.EmitOneColor(ur, ug, ub);
        }
    }
}
