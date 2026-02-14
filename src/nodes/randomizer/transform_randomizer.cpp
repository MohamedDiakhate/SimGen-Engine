#include <iostream>
#include "nodes/randomizers/transform_randomizer.h"
#include "fabric/scene_database.h"

TransformRandomizer::TransformRandomizer(std::string name, int ID,
                                        const TransformConfig& tfg)
    : RandomizerNode(std::move(name), ID, tfg.seed),
      transf_conf(tfg),
      target_entity_id(-1)
     
{
}

void TransformRandomizer::execute(Scene &scene)
{
    std::cout << "--- Randomizing with Spacing Check ---" << std::endl;

    // 1. Keep track of where we placed things
    std::vector<vec3> placed_positions;
    
    // Minimum distance between centers (e.g., 1.5 units)
    // If your spheres have scale 1.0, use at least 1.1 here.
    float min_spacing = 2.5f; 
    float min_spacing_sq = min_spacing * min_spacing; // Optimization

    for (auto &entity : scene.getAllEntities())
    {
        if (!entity) continue;
        
        if (entity->isStatic) continue;

        // Skip Floor/Light
        if (entity->name == "floor" || entity->name == "light") continue;

        vec3 candidatePos;
        bool valid_spot = false;
        int attempts = 0;

        // Try up to 10 times to find a free spot
        while (!valid_spot && attempts < 10)
        {
            // Generate a random spot
            candidatePos = vec3(getRandomFloat(transf_conf.minPos.x(), transf_conf.maxPos.x()),
                                getRandomFloat(transf_conf.minPos.y(), transf_conf.maxPos.y()),
                                getRandomFloat(transf_conf.minPos.z(), transf_conf.maxPos.z()));

            valid_spot = true;

            // Check against everyone we've placed so far
            for (const auto& existingPos : placed_positions)
            {
                vec3 diff = candidatePos - existingPos;
                // Use Squared Length to avoid slow sqrt()
                if (diff.squared_length() < min_spacing_sq) 
                {
                    valid_spot = false; 
                    break; // Too close! Try again.
                }
            }
            attempts++;
        }

        // Assign the best spot we found (even if it's imperfect after 10 tries)
        entity->position = candidatePos;
        placed_positions.push_back(candidatePos);

        // Randomize Rotation/Scale (These don't need collision checks)
        entity->rotation = vec3(getRandomFloat(transf_conf.minRot.x(), transf_conf.maxRot.x()),
                                getRandomFloat(transf_conf.minRot.y(), transf_conf.maxRot.y()),
                                getRandomFloat(transf_conf.minRot.z(), transf_conf.maxRot.z()));

        entity->scale = vec3(getRandomFloat(transf_conf.minScale.x(), transf_conf.maxScale.x()),
                             getRandomFloat(transf_conf.minScale.y(), transf_conf.maxScale.y()),
                             getRandomFloat(transf_conf.minScale.z(), transf_conf.maxScale.z()));
    }
}