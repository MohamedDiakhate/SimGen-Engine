#include <iostream>
#include <memory>
#include <random>

// --- CORE ENGINE ---
#include "core/graph.h"
#include "fabric/scene_database.h"
#include "core/entity.h"
#include "core/types.h"

// --- NODES ---
#include "nodes/physics/physics_node.h"
#include "nodes/randomizers/cam_pos_randomizer.h"
#include "nodes/randomizers/transform_randomizer.h"
#include "nodes/randomizers/color_randomizer.h"
#include "nodes/render/ray_trace_node.h"
#include "nodes/spawners/box_spawner.h"
#include "nodes/spawners/sphere_spawner.h"

int main(int argc, char *argv[])
{
    std::cout << "--- SimGen++ Engine Starting ---" << std::endl;

    unsigned int master_seed = 12345;

    if (argc > 1)
    {
        master_seed = std::stoi(argv[1]);
    }
    // 1. Initialize Core Systems
    Graph graph;
    Scene scene;

    // 2. Setup the World (The "Authoring" Phase)
    scene.createFloor();

    // Commented out scene light due to gif throwing errors for RGB > 255
    // scene.createLight();

    // Spawn 30 Spheres with radius 1
    SphereSpawnerNode sphere_spawner("sphere_spawner", 30, 2.0f);
    sphere_spawner.execute(scene);

    // Spawn 8 boxes of scale 2
    BoxSpawnerNode box_spawner("box_spawner", 8, 2.0f);
    box_spawner.execute(scene);

    // --- CAMERA SETUP ---
    vec3 camPos(0, 12, 30);
    vec3 camTarget(0, 0, 0);

    scene.cameraSettings.look_from = camPos;
    scene.cameraSettings.look_at = camTarget;
    scene.cameraSettings.vfov = 40.0f;
    scene.cameraSettings.aperture = 0.0f;

    // --- PIPELINE SETUP (The "Runtime" Phase) ---

    // 1. Transform Randomizer (FIXED ARGUMENTS)
    TransformConfig tfg = TransformConfig();
    tfg.seed = master_seed;
    auto set_up = std::make_shared<TransformRandomizer>("Transformer", 0, tfg);
    graph.addNode(Stage::setUp, set_up);

    // 2. Color Randomizer
    // Randomize between Black (0,0,0) and White (1,1,1)
    ColorConfig cfg = ColorConfig();
    cfg.seed = master_seed + 2;
    auto col_randomizer = std::make_shared<ColorRandomizer>("Colorer", 0, cfg);
    graph.addNode(Stage::Randomize, col_randomizer);

    // 3. Physics
    auto physics = std::make_shared<PhysicsNode>("Gravity_Solver", 0);
    graph.addNode(Stage::Physics, physics);

    // 4. Camera Position Randomizer, tested but ignored to avoid random jumps
    // graph.addNode(Stage::Randomize, std::make_shared<CamPosRandomizer>("Camera Pos Randomizer", 0));

    //  4. Ray Tracing
    //  Render output filename
    auto renderer = std::make_shared<RayTraceNode>("Renderer", 2, 400, 225, 300, "init.ppm");
    graph.addNode(Stage::Render, renderer);

    // 5. Run the Simulation
    std::cout << "[SimGen] Generated " << scene.getAllEntities().size() << " entities." << std::endl;

    std::cout << "--- STARTING ANIMATION RENDER ---" << std::endl;

    // This will now loop 40 times, run physics, AND render with correct filenames.
    graph.run(scene, 40, master_seed);


    graph.saveToFile("simulation.json");

    std::cout << "--- Simulation Finished ---" << std::endl;
    return 0;
}