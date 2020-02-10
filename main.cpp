// Uncomment this to validate that all objects were successfully
// sorted into the BVH
//#define DEBUG_BVH_SORT
#include "ppm.h"
#include "camera.h"
#include "sphere.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"

namespace
{
RandomGenerator rg;

vec3 colorAt(const Ray& r, const Hittable& world, int depth = 0)
{
    HitInfo info;
    if (world.hit(r, 0.001f, MAXFLOAT, info))
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

void createRandomScene(HittableSet& set)
{
    set.add(new Sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new Lambertian(vec3(0.5f, 0.5f, 0.5f))));
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float randMat = rg.getZeroToOne();
            vec3 center(a + 0.9f * rg.getZeroToOne(), 0.2f, b + 0.9f * rg.getZeroToOne());
            if ((center - vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f)
            {
                if (randMat < 0.8f) // diffuse
                {
                    set.add(new Sphere(center, center + vec3(0.0f, 0.5f * rg.getZeroToOne(), 0.0f),
                        0.0f, 1.0f, 0.2f, new Lambertian(vec3(rg.getZeroToOne() * rg.getZeroToOne(),
                                                              rg.getZeroToOne() * rg.getZeroToOne(),
                                                              rg.getZeroToOne() * rg.getZeroToOne()))));
                }
                else if (randMat < 0.95f) // metal
                {
                    set.add(new Sphere(center, 0.2,
                        new Metal(vec3(0.5f * (1.0f + rg.getZeroToOne()), 0.5f * (1.0f + rg.getZeroToOne()), 0.5f * (1.0f + rg.getZeroToOne())),
                            0.5f * rg.getZeroToOne())));
                }
                else // glass
                {
                    set.add(new Sphere(center, 0.2f, new Dielectric(1.5f)));
                }
            }
        }
    }

    set.add(new Sphere(vec3(0.0f, 1.0f, 0.0f), 1.0f, new Dielectric(1.5f)));
    set.add(new Sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new Lambertian(vec3(0.4f, 0.2f, 0.1f))));
    set.add(new Sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new Metal(vec3(0.7f, 0.6f, 0.5f), 0.0f)));
}

int main(int argc, char** argv)
{
    const unsigned int width(200);
    const unsigned int height(100);
    const unsigned int maxUIColor(255);
    const float maxColor(255.99f);
    const unsigned int numSamples(100);

    vec3 lookFrom(13.0f, 2.0f, 3.0f);
    vec3 lookAt(0.0f, 0.0f, -1.0f);
    vec3 vUp(0.0f, 1.0f, 0.0f);
    float distToFocus = (lookFrom - lookAt).length();
    float aperture(0.0f);
    Camera camera(lookFrom, lookAt, vUp, 20.0f, float(width) / float(height), aperture, distToFocus, 0.0f, 1.0f);
    HittableSet set;
    createRandomScene(set);
    set.sortMe(0.0f, 1.0f);
#ifdef DEBUG_BVH_SORT
    if (set.anyoneLeftBehind())
    {
        std::cerr << "rtiow: not all objects were sorted properly" << std::endl;
        return 1;
    }
#endif
    std::vector<uvec3> imageData;
    imageData.reserve(width * height);
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
                color += colorAt(r, set);
            }
            color /= float(numSamples);
            applyGamma(color);
            unsigned int ur = static_cast<unsigned int>(maxColor * color.x());
            unsigned int ug = static_cast<unsigned int>(maxColor * color.y());
            unsigned int ub = static_cast<unsigned int>(maxColor * color.z());
            imageData.push_back(uvec3(ur, ug, ub));
        }
    }

    PPMImage ppm(width, height, maxUIColor);
    ppm.emitHeader();
    for (auto pixel : imageData)
    {
        ppm.emitOneColor(pixel.x(), pixel.y(), pixel.z());
    }
}
