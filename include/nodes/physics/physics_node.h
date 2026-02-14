#pragma once
#include "core/node.h"
#include "core/types.h"


class PhysicsNode : public Node {

public : 
    PhysicsNode(std::string name, int ID);
    
    void execute(Scene& scene) override;

private:
    float dt = 0.2; 
};