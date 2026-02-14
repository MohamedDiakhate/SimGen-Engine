#pragma once
#include "nodes/randomizers/randomizer_node.h"
#include "core/types.h"

struct TransformConfig {
    unsigned int seed = 12345;
    vec3 minPos = vec3(-15, 0, -30);
    vec3 maxPos = vec3(10, 20, 15);
    vec3 minRot = vec3(0, 0, 0);
    vec3 maxRot = vec3(360.0f, 360.0f, 360.0f);
    vec3 minScale = vec3(1.0f, 1.0f, 1.0f);
    vec3 maxScale = vec3(1.5f, 1.5f, 1.5f);
};

class TransformRandomizer : public RandomizerNode
{
public:
    TransformConfig transf_conf;
    int target_entity_id; 

    TransformRandomizer(std::string name, int ID, const TransformConfig& tfg = TransformConfig());

    void execute(Scene &scene) override;

};