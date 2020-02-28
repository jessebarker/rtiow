#pragma once

#include "random.h"

class Perlin
{
    static constexpr unsigned int numValues_ = 256;

    float scale_;
    vec3 ranVec_[numValues_];
    unsigned int permX_[numValues_];
    unsigned int permY_[numValues_];
    unsigned int permZ_[numValues_];
    static void generatePerm(RandomGenerator& rg, unsigned int* permArray)
    {
        // perlin_generate_perm()
        for (unsigned int i = 0; i < numValues_; i++)
        {
            permArray[i] = i;
        }
        // permute()
        for (unsigned int i = numValues_ - 1; i > 0; i--)
        {
            unsigned int target = rg.getZeroToOne() * (i + 1);
            std::swap(permArray[i], permArray[target]);
        }
    }
public:
    Perlin(float s)
        : scale_(s)
    {
        RandomGenerator rg;
        generatePerm(rg, permX_);
        generatePerm(rg, permY_);
        generatePerm(rg, permZ_);

        // perlin_generate()
        for (unsigned int i = 0; i < numValues_; i++)
        {
            vec3 tmpVec(rg.getMinusOneToOne(), rg.getMinusOneToOne(), rg.getMinusOneToOne());
            tmpVec.normalize();
            ranVec_[i] = tmpVec;
        }
    }
    float scale() const
    {
        return scale_;
    }
    float noise(const vec3& position) const
    {
        vec3 scalePos = scale_ * position;
        vec3 floorPos = vec3(floor(scalePos.x()), floor(scalePos.y()), floor(scalePos.z()));
        vec3 uvw = scalePos - floorPos;
        vec3 hermiteUvw = uvw * uvw * (vec3(3.0f) - 2.0f * uvw);
        uvec3 ijk = uvec3(static_cast<unsigned int>(floorPos.x()), static_cast<unsigned int>(floorPos.y()), static_cast<unsigned int>(floorPos.z()));
        float accum = 0.0f;
        for (unsigned int i = 0; i < 2; i++)
        {
            for (unsigned int j = 0; j < 2; j++)
            {
                for (unsigned int k = 0; k < 2; k++)
                {
                    vec3 latticeVec = ranVec_[permX_[(i + ijk.x()) & 0xff] ^
                                              permY_[(j + ijk.y()) & 0xff] ^
                                              permZ_[(k + ijk.z()) & 0xff]];
                    vec3 weight = uvw - vec3(i, j, k);
                    accum += (i * hermiteUvw.x() + (1.0f - i) * (1.0f - hermiteUvw.x())) *
                             (j * hermiteUvw.y() + (1.0f - j) * (1.0f - hermiteUvw.y())) *
                             (k * hermiteUvw.z() + (1.0f - k) * (1.0f - hermiteUvw.z())) *
                             vec3::dot(weight, latticeVec);
                }
            }
        }

        return accum;
    }
    float turbulence(const vec3& position, unsigned int depth = 7) const
    {
        vec3 lPos(position);
        float accum = 0.0f;
        float weight = 1.0f;
        for (unsigned int i = 0; i < depth; i++)
        {
            accum += weight * noise(lPos);
            weight *= 0.5f;
            lPos *= 2.0f;
        }
        return fabs(accum);
    }
};
