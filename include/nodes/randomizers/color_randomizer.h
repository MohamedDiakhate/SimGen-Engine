#pragma once
#include "nodes/randomizers/randomizer_node.h"
#include "core/types.h"


struct ColorConfig {
    unsigned int seed = 12345;
    vec3 minColor = vec3(0, 0, 0);
    vec3 maxColor = vec3(1, 1, 1);
};

class ColorRandomizer : public RandomizerNode
{
public:

    ColorConfig col_conf;

    ColorRandomizer(std::string name, int ID, const ColorConfig& cfg = ColorConfig());

    void execute(Scene& scene) override;

};