#pragma once
#include "core/types.h"
#include "nodes/randomizers/randomizer_node.h"


// This Config Struct hold the data + defaults
struct CamRandomizerConfig
{
    unsigned int seed = 12345;
    float min_dist = 10.0f;
    float max_dist = 30.f;
    float min_height = 2.0f;
    float max_height = 10.0f;
    float min_angle = 0.0f;
    float max_angle = 360.0f;
};

class CamPosRandomizer : public  RandomizerNode {

public:
    CamPosRandomizer(std::string name, int ID, const CamRandomizerConfig& cfg = CamRandomizerConfig());
    void execute(Scene& scene);

private:
    CamRandomizerConfig cam_conf;
};