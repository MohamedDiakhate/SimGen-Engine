#pragma once
#include "core/node.h"
#include "fabric/scene_database.h"
#include <iostream>

class SphereSpawnerNode : public Node {
public:
    int count;
    float default_radius;

    SphereSpawnerNode(std::string name, int c, float r) : 
        Node(name, 0), count(c), default_radius(r){}

    void execute(Scene& scene){
        std::cout << "[SPAWNER] creating" << count << " spheres \n";

        for (int i = 0; i < count; i++){

            float diameter = default_radius * 2;
            auto entity = scene.createEntity("sphere_" + std::to_string(i));
            entity->scale = vec3(diameter, diameter, diameter);
            entity->ID = 0;

            if (i % 2 == 0){
                entity->material_ = std::make_shared<metal>(entity->color, 0);
            } else {
                entity->material_ = std::make_shared<lambertian>(entity->color);
            }

        }
        
    }

};