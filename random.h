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
    double GetZeroToOne()
    {
        return distributionZeroToOne_(generator_);
    }
    double GetMinusOneToOne()
    {
        return distributionMinusOneToOne_(generator_);
    }
};
