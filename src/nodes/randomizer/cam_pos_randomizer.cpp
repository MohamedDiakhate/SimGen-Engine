
#include "nodes/randomizers/cam_pos_randomizer.h"
#include "fabric/scene_database.h"
#include <random> // Required for std::random_device
#include <ctime>  // Required for time()
struct Entity;





CamPosRandomizer::CamPosRandomizer(std::string name, int ID, const CamRandomizerConfig &cfg)
    : RandomizerNode(name, ID, cfg.seed), cam_conf(cfg)

{
}

void CamPosRandomizer::execute(Scene &scene)
{
    CameraData &cam = scene.cameraSettings;

    float dist_from_center = getRandomFloat(cam_conf.min_dist,cam_conf.max_dist);
    float new_angle = getRandomFloat(cam_conf.min_angle, cam_conf.max_angle);
    float new_height = getRandomFloat(cam_conf.min_height, cam_conf.max_height);

    // Convert to Radians (Math uses radians, not degrees)
    float theta = new_angle * (M_PI / 180.0f);

    // Calculate Position
    float camX = dist_from_center * sin(theta);
    float camY = new_height;
    float camZ = dist_from_center * cos(theta);

    cam.look_from = vec3(camX, camY, camZ);
    cam.look_at = vec3(0, 0, 0);
    
}