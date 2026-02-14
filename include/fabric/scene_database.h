#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include "core/entity.h"
#include "json.hpp"

// Forward declaration only! Don't include sphere.h here.
// This keeps compile times fast and dependencies clean.
struct CameraData{
    vec3 look_from = vec3(0, 12, -20);
    vec3 look_at = vec3(0.0f, 0.0f, 0.0f);
    float vfov = 40.0f;
    float aspect = 16.0f / 9.0f;
    float aperture = 0.0f;
    float focus_dist = 10.0f;
};

class Scene {
public: 
    CameraData cameraSettings; // Public access for easy tweaking

    // Create Floor (Huge Sphere Trick)
    void createFloor() {
        auto floor = std::make_shared<Entity>(nextCounter++, "floor");

        floor->position = vec3(0, -1000, 0);
        floor->scale = vec3(2000, 2000, 2000); // Diameter = 2000
        floor->isStatic = true; // Physics will ignore gravity for this
        floor->setColor(vec3(0.8, 0.8, 0.2));
        floor->material_ = std::make_shared<lambertian>(floor->color);
        
        addEntity(floor); 
    }

    // Create light s¡Source
    void createLight() {
        auto light = std::make_shared<Entity>(nextCounter++, "light");

        light->position = vec3(0, 10, 0);
        light->scale = vec3(2, 2, 2);
        light->isStatic = true;

        light->setColor(vec3 (4, 4, 4));
        light->material_ = std::make_shared<diffuse_light>(light->color);

        addEntity(light);
    }

    // Generic Entity Creation
    std::shared_ptr<Entity> createEntity(std::string name) {
        
        int newID = nextCounter++;
        auto newEntity = std::make_shared<Entity>(newID, name);
        addEntity(newEntity);
        return newEntity;
    }

    // STORAGE MANAGEMENT
    void addEntity(std::shared_ptr<Entity> entity) {
        if (entity) {
             entityMap[entity->ID] = entity;
             entityList.push_back(entity);
        }
    }

    // Optimizing access: Raw pointer is faster than shared_ptr copying
    Entity* getEntity(int id) {
        auto it = entityMap.find(id);
        return (it != entityMap.end()) ? it->second.get() : nullptr;
    }



    const std::unordered_map<int, std::shared_ptr<Entity>>& getEntities() const {
        return entityMap;
    }

    const std::vector<std::shared_ptr<Entity>>& getAllEntities() const {
        return entityList;
    }
  

    // SERIALIZATION
    nlohmann::json toJson() const {
        nlohmann::json root;
        root["entities"] = nlohmann::json::array(); 
        for (const auto& entity : entityList) {
            root["entities"].push_back(entity->toJson());
        }
        return root;
    }

private:
    std::unordered_map<int, std::shared_ptr<Entity>> entityMap;
    std::vector<std::shared_ptr<Entity>> entityList;
    int nextCounter = 1;
};