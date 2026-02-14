#pragma once
#include "core/node.h"
#include <string>
#include <vector>


class hitable_list; 

class RayTraceNode : public Node {
private:
    int width;
    int height;
    int samples;

    // Converts Engine Scene -> RT World
    std::shared_ptr<hitable_list> convertSceneToRT(Scene& scene);

    
public:
    std::string filename;
    int frame_counter = 0;

    RayTraceNode(std::string name, int priority, int w, int h, int ns, std::string outfile);
    
    void execute(Scene& scene) override;
};