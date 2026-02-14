#pragma once
#include "core/node.h"
#include "core/entity.h"
#include "fabric/scene_database.h"


class BoxSpawnerNode : public Node{
public:
    int count;
    float _scale;

    BoxSpawnerNode(std::string name, int c, float s) : 
        Node(name, 0), count(c), _scale(s){}

    void execute(Scene& scene){
        for (int i = 0; i < count; i++){
            auto entity = scene.createEntity("box_" + std::to_string(i));
            entity->scale = vec3(_scale, _scale, _scale);

            if (i % 2 == 0){
                entity->material_ = std::make_shared<metal>(entity->color, 0.5);
            } else {
                entity->material_ = std::make_shared<lambertian>(entity->color);
            }
        }
    }
};