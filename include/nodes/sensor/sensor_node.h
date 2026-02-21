#pragma once 
#include "core/node.h"
#include "core/types.h"
#include "fabric/scene_database.h"

class SensorNode : public Node {
public:
    SensorNode(std::string name_, int ID, const CameraData& cam_params);

    void execute(Scene& scene) override;

private:
    CameraData cam_params;
    std::vector<float> kernel;
    float kernel_radius;

    void applyExposure(std::vector<vec3>& img);
    void applyPSF(std::vector<vec3>& img);
    void applyNoise(std::vector<vec3>& img);
    void quantize(std::vector<vec3>& img);
    void generateGaussianKernel(float sigma);
    
};