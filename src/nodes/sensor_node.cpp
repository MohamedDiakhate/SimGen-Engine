#include "nodes/sensor/sensor_node.h"

class Scene;

SensorNode::SensorNode(std::string name, int ID, const CameraData& params) : 
                        Node(name, ID), 
                        cam_params(params) {}

void SensorNode::execute(Scene& scene){

    std::vector<vec3> radiance_buffer = scene.getRadianceBuffer();

    applyExposure(radiance_buffer);
    applyPSF(radiance_buffer);
    applyNoise(radiance_buffer);
    quantize(radiance_buffer);
}

void SensorNode::applyExposure(std::vector<vec3>& img){

}

void SensorNode::applyPSF(std::vector<vec3>& img){
    int h = cam_params.height;
    int w = cam_params.width;
    int size = 2 * kernel_radius + 1;
    std::vector<vec3> temp = img;

    for (int y = kernel_radius; y < h - kernel_radius; y++){
        for (int x = kernel_radius; x < w - kernel_radius; x++){
            vec3 sum = vec3(0, 0, 0);

            for (int ky = -kernel_radius; ky < kernel_radius; ky++){
                for (int kx = -kernel_radius; kx < kernel_radius; kx++){

                    int pixel_idx = (y + ky) * w + (x + kx);
                    vec3 color = temp[pixel_idx];

                    float weight = kernel[(ky + kernel_radius) * size + (kx + kernel_radius)];

                    sum += (color * weight);
                }
            }
            img[(y * w) + x] = sum;

        }
    }
}

void SensorNode::generateGaussianKernel(float sigma){
    kernel_radius = std::ceil(3.0f * sigma);
    int size = 2 * kernel_radius + 1;
    kernel.resize(size * size);

    float sum = 0.0f;

    for (int y = -kernel_radius; y < kernel_radius; y++){
        for (int x = -kernel_radius; x < kernel_radius; x++){
            int row = y + kernel_radius;
            int col = x + kernel_radius;
            float val = std::exp(-(x*x + y*y) / (2 * sigma));
            kernel[row * size + col] = val;
            sum += val;
        }
    }

    for (float& v : kernel){
        v /= sum;
    }

}

void SensorNode::applyNoise(std::vector<vec3>& img){

}

void SensorNode::quantize(std::vector<vec3>& img){
    
}