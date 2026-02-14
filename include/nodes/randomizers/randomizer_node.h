#pragma once
#include "core/node.h"
#include <random>
#include <algorithm>

class RandomizerNode : public Node
{

public:
    RandomizerNode(std::string name, int ID,
                   unsigned int seed) : Node(std::move(name), ID), rng(seed)
    {
    }

protected:
    float getRandomFloat(float min, float max)
    {
        if (min > max){
            std::swap(min, max);
        }

        std::uniform_real_distribution<float> dist(min, max);
        return dist(this->rng); // Uses the generator we just seeded!
    }

    int getRandomInt(int min, int max)
    {
        if (min > max){
            std::swap(min, max);
        }

        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

protected:
    std::mt19937 rng; // each node has its rng to prevent race conditions
};