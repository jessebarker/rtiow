#include "ppm.h"
#include "camera.h"
#include "sphere.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"

namespace
{
RandomGenerator rg;

vec3 colorAt(const Ray& r, const Hittable* world, int depth)
{
    HitInfo info;
    if (world->hit(r, 0.001f, MAXFLOAT, info))
    {
        Ray scattered;
        vec3 attenuation;
        if (depth < 50 && info.material->scatter(r, info, attenuation, scattered))
        {
            return attenuation * colorAt(scattered, world, depth + 1);
        }
        return vec3(0.0f);
    }
    else
    {
        vec3 direction = r.direction();
        direction.normalize();
        float t = 0.5f * (direction.y() + 1.0f);
        return (1.0f - t) * vec3(1.0) + t * vec3(0.5f, 0.7f, 1.0f);
    }
}

void applyGamma(vec3& linearColor)
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
    ppm.emitHeader();

    const unsigned int numHittables(5);
    Hittable* list[numHittables];
    list[0] = new Sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(vec3(0.1f, 0.2f, 0.5f)));
    list[1] = new Sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f, new Lambertian(vec3(0.8f, 0.8f, 0.0f)));
    list[2] = new Sphere(vec3(1.0f, 0.0f, -1.0f), 0.5f, new Metal(vec3(0.8f, 0.6f, 0.2f), 0.2f));
    list[3] = new Sphere(vec3(-1.0f, 0.0f, -1.0f), 0.5f, new Dielectric(1.5f));
    list[4] = new Sphere(vec3(-1.0f, 0.0f, -1.0f), -0.45f, new Dielectric(1.5f));
    Hittable* world = new HittableList(list, numHittables);
    vec3 lookFrom(3.0f, 3.0f, 2.0f);
    vec3 lookAt(0.0f, 0.0f, -1.0f);
    vec3 vUp(0.0f, 1.0f, 0.0f);
    float distToFocus = (lookFrom - lookAt).length();
    float aperture(2.0f);
    Camera camera(lookFrom, lookAt, vUp, 20.0f, float(width) / float(height), aperture, distToFocus);

    for (unsigned int y = height - 1; y >= 0 && y < height; y--)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            vec3 color;
            for (unsigned int s = 0; s < numSamples; s++)
            {
                float v = float(y + rg.getZeroToOne()) / float(height);
                float u = float(x + rg.getZeroToOne()) / float(width);
                Ray r = camera.getRay(u, v);
                vec3 point = r.pointAt(2.0f);
                color += colorAt(r, world, 0);
            }
            color /= float(numSamples);
            applyGamma(color);
            unsigned int ur = static_cast<unsigned int>(maxColor * color.x());
            unsigned int ug = static_cast<unsigned int>(maxColor * color.y());
            unsigned int ub = static_cast<unsigned int>(maxColor * color.z());
            ppm.emitOneColor(ur, ug, ub);
        }
    }
}
