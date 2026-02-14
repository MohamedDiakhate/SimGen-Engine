#pragma once
#include "core/graph.h"

class Scene;
class Scheduler{

public:
    void Run(const Graph& graph){
        
        for (const auto& node : graph.getNodes()){
            //node->execute(Scene& scene);
        }
    }
};