#pragma once

#include <random>

class RandomGenerator
{
    std::uniform_real_distribution<double> distributionZeroToOne_;
    std::uniform_real_distribution<double> distributionMinusOneToOne_;
    std::mt19937 generator_;
public:
    RandomGenerator()
        : distributionZeroToOne_(0.0f, 1.0f)
        , distributionMinusOneToOne_(-1.0f, 1.0f)
    {
    }
    double getZeroToOne()
    {
        return distributionZeroToOne_(generator_);
    }
    double getMinusOneToOne()
    {
        return distributionMinusOneToOne_(generator_);
    }
    vec3 getPointInUnitSphere()
    {
        vec3 point;
        float length = 0.0f;
        do
        {
            point = 2.0f * vec3(getMinusOneToOne(), getMinusOneToOne(), getMinusOneToOne()) - vec3(1.0f);
            length = point.length();
        } while ((length * length) >= 1.0f);
        return point;
    }
};
