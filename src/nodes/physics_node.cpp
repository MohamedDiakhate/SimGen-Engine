#include "nodes/physics/physics_node.h"
#include "fabric/scene_database.h"

class Scene;
class types;
PhysicsNode::PhysicsNode(std::string name, int ID)
    : Node(name, ID)
{
}

void PhysicsNode::execute(Scene &scene)
{
    vec3 gravity = vec3(0, -9.81f, 0);
    for (const auto &entity : scene.getAllEntities())
    {

        if (entity.get()->isStatic)
            continue;

        entity->acceleration = vec3(0, 0, 0);
        entity->acceleration += gravity;

        entity->velocity += (entity->acceleration * dt);
        entity->position += (entity->velocity * dt);

        float floor_y = 0.0f;
        float radius = entity->scale.x() * 0.5f;
        

        if (entity->position.y() - radius < floor_y)
        {

            entity->position[1] = floor_y + radius;

            // effects of bouncing in reducing velocity, velocity < 0 means ball was falling
            if (entity->velocity.y() < 0)
            {

                entity->velocity[1] *= -0.8f;

                // friction because slowing down after bouncing happens horizontally (applied to X() and Z())
                entity->velocity[0] *= 0.9f;
                entity->velocity[2] *= 0.9f;
            }
        }
    }
}