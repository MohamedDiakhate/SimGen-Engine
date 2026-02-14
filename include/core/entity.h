#pragma once
#include "core/types.h"
#include "nodes/render/rt_core/material.h"
#include <json.hpp> 



struct Entity
{       
    // 8 bytes Block
    int ID;
    float mass;

    // 8 bytes Block (3 padding bytes
    bool isStatic;
    char padding[3];
    float friction;

    // 12 bytes each
    vec3 position;
    vec3 rotation;
    vec3 scale;
    vec3 velocity; 
    vec3 acceleration; 
    vec3 color;

    // moved down to prevent cache misses 
    std::string name;
    std::shared_ptr<material> material_;
   
    

    Entity(int _id, std::string _name = "Entity")
        : ID(_id),
          mass(1.0f),
          isStatic(false),
          position(0, 0, 0),
          rotation(0, 0, 0),
          scale(1, 1, 1),
          velocity(0, 0, 0),
          acceleration(0, 0, 0),
          color(1, 1, 1),
          name(_name),
          // Default to grey color
          material_(std::make_shared<lambertian>(vec3(0.5, 0.5, 0.5)))
    {
    }

    void setColor(vec3 new_color){
        this->color = new_color;

        if (material_){
            material_->set_color(new_color);
        }
    }

    nlohmann::json toJson() const {
        return {
            {"id", ID},
            {"name", name},
            {"transform", {
                {"position", {position.x(), position.y(), position.z()}},
                {"rotation", {rotation.x(), rotation.y(), rotation.z()}},
                {"scale",    {scale.x(),    scale.y(),    scale.z()}}
            }},
            {"color", {color.r(), color.g(), color.b()}}
        };
    }
};