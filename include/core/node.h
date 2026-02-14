#pragma once
#include <string>
#include <vector>
#include <atomic>



class Graph; // FORWARD DECLARATIOn to avoid circular dependencies
class Scheduler;
class Scene;
class Node{
public : 
    virtual ~Node() = default;


    virtual void execute(Scene& scene) = 0;
    //remember this needs data to exectude the scendedata fabric is needed.

public:
    
    Node(std::string nodeName, int nodeID) : name(std::move(nodeName)), ID(nodeID) {
        
    }

    std::string name;
    int ID;

private: 

    friend Graph;
    friend Scheduler;

};
