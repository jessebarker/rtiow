// Uncomment this to validate that all objects were successfully
// sorted into the BVH
//#define DEBUG_BVH_SORT
#include <cfloat>
#include "camera.h"
#include "sphere.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "diffuse-light.h"
#include "box.h"
#include "instance.h"
#include "volume.h"

namespace
{
RandomGenerator rg;

vec3 colorAt(const Ray& r, const Hittable& world, const vec3& attenuation, int depth = 0)
{
    vec3 emitted;
    // If we've gone as deep as we want, or if we're just not making a contribution, we're done
    // (I _think_ it's ok not to take attenuation.length())
    if (depth > 49 || vec3::dot(attenuation, attenuation) < 0.001f)
    {
        return emitted;
    }

    HitInfo info;
    if (world.hit(r, 0.001f, FLT_MAX, info))
    {
        Ray scattered;
        vec3 curAttenuation;
        emitted = info.material->emitted(info.uv, info.point);
        if (info.material->scatter(r, info, curAttenuation, scattered))
        {
            // We scattered.  Accumulate attenuation and go again.
            return colorAt(scattered, world, attenuation * curAttenuation, depth + 1);
        }

        // We didn't scatter, just return the current contribution
        return emitted * attenuation;
    }

    return emitted;
}

void applyGamma(vec3& linearColor)
{
    const float gammaPower(1.0f / 2.2f);
    linearColor.x(pow(linearColor.x(), gammaPower));
    linearColor.y(pow(linearColor.y(), gammaPower));
    linearColor.z(pow(linearColor.z(), gammaPower));
}

} // namespace

void createRandomScene(HittableSet& set, Camera& camera)
{
    Material* light = new DiffuseLight(new ConstantTexture(vec3(4.0f, 4.0f, 4.0f))); 
    set.add(new Sphere(vec3(0.0f, 3000.0f, 0.0f), 1000.0f, light));
    Texture* checker = new CheckerTexture(new ConstantTexture(vec3(0.2f, 0.3f, 0.1f)), new ConstantTexture(vec3(0.9f)));
    set.add(new Sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new Lambertian(checker)));
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
                        0.0f, 1.0f, 0.2f, new Lambertian(new ConstantTexture(vec3(rg.getZeroToOne() * rg.getZeroToOne(),
                                                                                  rg.getZeroToOne() * rg.getZeroToOne(),
                                                                                  rg.getZeroToOne() * rg.getZeroToOne())))));
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
    set.add(new Sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new Lambertian(new ConstantTexture(vec3(0.4f, 0.2f, 0.1f)))));
    set.add(new Sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new Metal(vec3(0.7f, 0.6f, 0.5f), 0.0f)));
    vec3 lookFrom(13.0f, 2.0f, 3.0f);
    vec3 lookAt(0.0f, 2.0f, 0.0f);
    vec3 up(0.0f, 1.0f, 0.0f);
    float distToFocus = (lookFrom - lookAt).length();
    float aperture(0.0f);
    float vFov = 40.0f;
    camera.setOrientation(lookFrom, lookAt, up);
    camera.setFov(vFov);
    camera.setFocus(aperture, distToFocus);
    camera.setExposure(0.0f, 1.0f);
}

void createTwoSpheresScene(HittableSet& set)
{
    Texture* checker = new CheckerTexture(new ConstantTexture(vec3(0.2f, 0.3f, 0.1f)), new ConstantTexture(vec3(0.9f)));
    set.add(new Sphere(vec3(0.0f, -10.0f, 0.0f), 10.0f, new Lambertian(checker)));
    set.add(new Sphere(vec3(0.0f, 10.0f, 0.0f), 10.0f, new Lambertian(checker)));
}

void createEarthScene(HittableSet& set)
{
    set.add(new Sphere(vec3(0.0f, 0.0f, 0.0f), 2.0f, new Lambertian(new ImageTexture("earthmap.jpg"))));
}

void createPerlinSpheres(HittableSet& set)
{
    Texture* noise = new NoiseTexture(4.0f);
    set.add(new Sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new Lambertian(noise)));
    set.add(new Sphere(vec3(0.0f, 2.0f, 0.0f), 2.0f, new Lambertian(noise)));
}

void createEmissiveScene(HittableSet& set, Camera& camera)
{
    createPerlinSpheres(set);
    Material* light = new DiffuseLight(new ConstantTexture(vec3(4.0f))); 
    set.add(new Sphere(vec3(0.0f, 7.0f, 0.0f), 2.0f, light));
    vec2 min(3.0f, 1.0f);
    vec2 max(5.0f, 3.0f);
    set.add(new RectXY(min, max, -2.0f, light));
    vec3 lookFrom(13.0f, 2.0f, 3.0f);
    vec3 lookAt(0.0f, 2.0f, 0.0f);
    vec3 up(0.0f, 1.0f, 0.0f);
    float distToFocus = (lookFrom - lookAt).length();
    float aperture(0.0f);
    float vFov = 40.0f;
    camera.setOrientation(lookFrom, lookAt, up);
    camera.setFov(vFov);
    camera.setFocus(aperture, distToFocus);
    camera.setExposure(0.0f, 1.0f);
}

void createCornellBox(HittableSet& set, Camera& camera)
{
    Material* red = new Lambertian(new ConstantTexture(vec3(0.65f, 0.05f, 0.05f)));
    Material* white = new Lambertian(new ConstantTexture(vec3(0.73f)));
    Material* green = new Lambertian(new ConstantTexture(vec3(0.12f, 0.45f, 0.15f)));
    Material* light = new DiffuseLight(new ConstantTexture(vec3(4.0f)));

    // The surrounding box itself
    vec2 boxMin(0.0f, 0.0f);
    vec2 boxMax(555.0f, 555.0f);
    vec2 lightMin(113.0f, 127.0f);
    vec2 lightMax(443.0f, 432.0f);
    set.add(new RectXZ(lightMin, lightMax, 554.0f, light));
    set.add(new FlipNormals(new RectYZ(boxMin, boxMax, 555.0f, green))); // left
    set.add(new RectYZ(boxMin, boxMax, 0.0f, red));                      // right
    set.add(new FlipNormals(new RectXZ(boxMin, boxMax, 555.0f, white))); // top
    set.add(new RectXZ(boxMin, boxMax, 0.0f, white));                    // bottom
    set.add(new FlipNormals(new RectXY(boxMin, boxMax, 555.0f, white))); // back

    // The blocks inside the box
    Hittable* box1 = new Translate(new RotateY(new Box(vec3(0.0f, 0.0f, 0.0f),
        vec3(165.0f, 165.0f, 165.0f), white), -18.0f), vec3(130.0f, 0.0f, 65.0f));
    Hittable* box2 = new Translate(new RotateY(new Box(vec3(0.0f, 0.0f, 0.0f),
        vec3(165.0f, 330.0f, 165.0f), white), 15.0f), vec3(265.0f, 0.0f, 295.0f));
    set.add(new ConstantMedium(box1, 0.01f, new ConstantTexture(vec3(1.0f, 1.0f, 1.0f))));
    set.add(new ConstantMedium(box2, 0.01f, new ConstantTexture(vec3(0.0f, 0.0f, 0.0f))));

    vec3 lookFrom(278.0f, 278.0f, -800.0f);
    vec3 lookAt(278.0f, 278.0f, 0.0f);
    vec3 up(0.0f, 1.0f, 0.0f);
    float distToFocus = (lookFrom - lookAt).length();
    float aperture(0.0f);
    float vFov = 40.0f;
    camera.setOrientation(lookFrom, lookAt, up);
    camera.setFov(vFov);
    camera.setFocus(aperture, distToFocus);
    camera.setExposure(0.0f, 1.0f);
}

void createMyScene(HittableSet& set, Camera& camera)
{
    Material* light = new DiffuseLight(new ConstantTexture(vec3(4.0f)));
    vec2 lightMin(0.0f, 0.0f);
    vec2 lightMax(2.0f, 2.0f);
    set.add(new RectXZ(lightMin, lightMax, 3.7f, light));

    // Ground boxes get their own BVH
    Material* ground = new Lambertian(new ConstantTexture(vec3(0.5f, 0.8f, 0.5f)));
    const unsigned int numGroundBoxes = 20;
    const float w = 0.5f;
    vec3 boxMin;
    vec3 boxMax;
    HittableSet* groundSet = new HittableSet();
    for (unsigned int i = 0; i < numGroundBoxes; i++)
    {
        for (unsigned int j = 0; j < numGroundBoxes; j++)
        {
            boxMin.x(-5 + i * w);
            boxMin.z(-5 + j * w);
            boxMin.y(0.0f);
            boxMax.x(boxMin.x() + w);
            boxMax.y(0.5f * (rg.getZeroToOne() + 0.01f));
            boxMax.z(boxMin.z() + w);
            groundSet->add(new Box(boxMin, boxMax, ground));
        }
    }
    groundSet->sortMe(0.0f, 1.0f);
    set.add(groundSet);

    const vec3 scPerlin(1.0f, 1.0f, 1.0f);
    const vec3 scGlass(0.75f, 1.5f, 1.5f);
    const vec3 scEnamel(1.0f, 1.0f, 0.0f);
    const vec3 scReflection(0.0f, 2.0f, -2.5f);
    const vec3 scReflection2(0.0f, 2.0f, 4.5f);
    const vec3 scBox(-1.0f, 2.5f, 2.0f);
    // Perlin ball
    Texture* noise = new NoiseTexture(4.0f);
    set.add(new Sphere(scPerlin, 0.4f, new Lambertian(noise)));
    // Glass ball
    set.add(new Sphere(scGlass, 0.4f, new Dielectric(1.5f)));
    // Metal balls
    set.add(new Sphere(scReflection, 2.0f, new Metal(vec3(0.9f))));
    set.add(new Sphere(scReflection2, 2.0f, new Metal(vec3(0.9f))));
    // Enamel ball
    Hittable* colorGlass = new Sphere(scEnamel, 0.4f, new Dielectric(1.5f));
    set.add(colorGlass);
    set.add(new ConstantMedium(colorGlass, 100.0f, new ConstantTexture(vec3(0.9f, 0.0f, 0.0f))));

    // Box of spheres
    Material* boxMaterial = new Lambertian(new ConstantTexture(vec3(0.2f, 0.2f, 0.9f)));
    HittableSet* boxSet = new HittableSet();
    const unsigned int numSpheres = 1000;
    for (unsigned int i = 0; i < numSpheres; i++)
    {
        boxSet->add(new Sphere(vec3(scBox.x() * rg.getZeroToOne(), scBox.y() * rg.getZeroToOne(), scBox.z() * rg.getZeroToOne()),
                               0.05f, boxMaterial));
    }
    boxSet->sortMe(0.0f, 1.0f);
    set.add(new RotateY(boxSet, 15.0f));

    // Large clear/white sphere that surrounds us
    Hittable* boundary = new Sphere(vec3(0.0f), 5.0f, new Dielectric(1.5));
    set.add(new ConstantMedium(boundary, 0.0001f, new ConstantTexture(vec3(1.0f))));

    vec3 lookFrom(8.0f, 2.0f, 3.0f);
    vec3 lookAt(0.0f, 2.0f, 0.0f);
    vec3 up(0.0f, 1.0f, 0.0f);
    float distToFocus = (lookFrom - lookAt).length();
    float aperture(0.0f);
    float vFov = 30.0f;
    camera.setOrientation(lookFrom, lookAt, up);
    camera.setFov(vFov);
    camera.setFocus(aperture, distToFocus);
    camera.setExposure(0.0f, 1.0f);
}

int main(int argc, char** argv)
{
    const unsigned int width(800);
    const unsigned int height(400);
    const float maxColor(255.99f);
    const unsigned int numSamples(400);

    Camera camera;
    camera.setAspect(float(width) / float(height));
    HittableSet set;
    //createRandomScene(set, camera);
    //createTwoSpheresScene(set);
    //createPerlinSpheres(set);
    //createEarthScene(set);
    //createEmissiveScene(set, camera);
    //createCornellBox(set, camera);
    createMyScene(set, camera);
    camera.applySettings();
    set.sortMe(0.0f, 1.0f);
#ifdef DEBUG_BVH_SORT
    if (set.anyoneLeftBehind())
    {
        std::cerr << "rtiow: not all objects were sorted properly" << std::endl;
        return 1;
    }
#endif
    typedef tvec3<unsigned char> ucvec3;
    std::vector<ucvec3> imageData;
    imageData.reserve(width * height);
    const vec3 initialAttenuation(1.0f);
    for (unsigned int y = height - 1; y >= 0 && y < height; y--)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            vec3 color;
            for (unsigned int s = 0; s < numSamples; s++)
            {
                vec2 uv(float(x + rg.getZeroToOne()) / float(width),
                        float(y + rg.getZeroToOne()) / float(height));
                Ray r = camera.getRay(uv);
                color += colorAt(r, set, initialAttenuation);
            }
            color /= float(numSamples);
            applyGamma(color);
            unsigned int ur = static_cast<unsigned int>(maxColor * color.x());
            unsigned int ug = static_cast<unsigned int>(maxColor * color.y());
            unsigned int ub = static_cast<unsigned int>(maxColor * color.z());
            imageData.push_back(ucvec3(ur, ug, ub));
        }
    }

    Image image;
    bool didIt = image.store("output.png", width, height, 3, reinterpret_cast<unsigned char*>(imageData.data()));
    if (!didIt)
    {
        std::cerr << "Failed to store the output image" << std::endl;
        return 1;
    }

    return 0;
}
