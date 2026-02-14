#include <iostream>
#include "core/graph.h"
#include <fstream> 
#include "json.hpp" 

void Graph::addNode(Stage stage, std::shared_ptr<Node> node){
    pipeline[stage].push_back(node);
    nodes.push_back(node);
}

const std::vector<std::shared_ptr<Node>>& Graph::getNodes() const{
    return nodes;
}

void Graph::executeList(Scene& scene, const std::vector<std::shared_ptr<Node>>& nodeList){
    for (const auto& node : nodeList){
        if (node) {
            node->execute(scene);
        }
    }
}

// 1. UPDATE SIGNATURE: Accept 'master_seed' here
void Graph::run(Scene& scene, int total_frames, unsigned int master_seed) {
    
    // 2. STORE METADATA: Save the seed so we can reproduce this later!
    nlohmann::json root;
    root["metadata"] = {
        {"version", "1.0"},
        {"master_seed", master_seed}, // <--- CRITICAL: Store the recipe
        {"frame_count", total_frames},
        {"fps", 60}
    };
    
    root["frames"] = nlohmann::json::array();
    
    // Assign to internal history
    simulation_history = root; 

    std::cout << "--- Simulation Start (Seed: " << master_seed << ") ---\n";

    // --- PHASE 1: INITIALIZATION (Run Once) ---
    // Best for: Creating Floor, Lights, and Initial Positioning
    executeList(scene, pipeline[Stage::setUp]);

    // --- PHASE 2: SIMULATION LOOP (Run Many Times) ---
    for (int i = 0; i < total_frames; i++) {
        
        // A. PER-FRAME LOGIC
        // Best for: ColorRandomizer (Disco Effect), Wind, Continuous Forces
        executeList(scene, pipeline[Stage::Randomize]);
        
        executeList(scene, pipeline[Stage::Physics]); // Update positions
        executeList(scene, pipeline[Stage::Render]);  // Take picture
        executeList(scene, pipeline[Stage::Sensor]);  // Analyze
        executeList(scene, pipeline[Stage::Output]);  // Log/Network
      
        // B. CAPTURE FRAME
        nlohmann::json frame_snapshot = {
            {"frame_id", i},
            {"data", scene.toJson()} 
        };

        // Access the "frames" array directly
        simulation_history["frames"].push_back(std::move(frame_snapshot));
        
        if (i % 10 == 0) {
            std::cout << "Frame " << i << "/" << total_frames << " Captured.\n";
        }
    }
    std::cout << "--- Simulation End ---\n";
}

void Graph::saveToFile(const std::string& filename) {
    std::ofstream file(filename);

    if (file.is_open()) {
        // Pretty print with indent of 4 spaces
        file << simulation_history.dump(4);
        std::cout << "Saved trajectory (" << simulation_history["frames"].size() << " frames) to: " << filename << "\n";
    } else {
        std::cerr << "Error: Could not open file " << filename << "\n";
    }
}