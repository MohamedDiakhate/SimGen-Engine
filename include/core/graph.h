#pragma once
#include <memory>
#include <vector>
#include "core/node.h"
#include "fabric/scene_database.h"

enum class Stage{
    setUp,
    Randomize,
    Physics,
    Render,
    Sensor,
    Output,

};

class Graph
{
public:
    /*
    Add an node to the graph
    share ownership for other parts of the engine
    */

    void addNode(Stage stage, std::shared_ptr<Node> node);
    
    
    void run(Scene& scene, int total_frames, unsigned int seed);
    
    void saveToFile(const std::string& filename);

    void executeList(Scene& scene, const std::vector<std::shared_ptr<Node>>& nodeList);

    const std::vector<std::shared_ptr<Node>>& getNodes() const; 


private:
    std::map<Stage, std::vector<std::shared_ptr<Node>>> pipeline;
    std::vector<std::shared_ptr<Node>> nodes;

    nlohmann::json simulation_history; 
};