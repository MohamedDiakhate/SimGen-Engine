#include "nodes/render/ray_trace_node.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <memory>
#include <limits>
#include "fabric/scene_database.h"
#include "nodes/render/rt_core/sphere.h"
#include "nodes/render/rt_core/box.h"
#include "nodes/render/rt_core/camera.h"
#include "nodes/render/rt_core/material.h"
#include "nodes/render/rt_core/float.h"
#include "nodes/render/rt_core/hitable_list.h"
#include "core/global_pool.h"


static vec3 get_ray_color(const ray &r, const std::shared_ptr<hitable_list> &world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001f, std::numeric_limits<float>::max(), rec))
    {
        ray scattered;
        vec3 attenuation;
        vec3 emitted = rec.mat_ptr->emitted(0, 0, rec.p);

        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return emitted + attenuation * get_ray_color(scattered, world, depth + 1);
        }
        else
        {
            return emitted;
        }
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5f * (unit_direction.y() + 1.0f);
        return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

RayTraceNode::RayTraceNode(std::string name, int priority, int w, int h, int ns, std::string outfile)
    : Node(name, priority), width(w), height(h), samples(ns), filename(outfile) {}

std::shared_ptr<hitable_list> RayTraceNode::convertSceneToRT(Scene &scene)
{
    auto &entities = scene.getAllEntities();
    auto world = std::make_shared<hitable_list>();

    for (const auto &entity : entities)
    {
        if (!entity) continue;

        vec3 pos(entity->position.x(), entity->position.y(), entity->position.z());
        std::shared_ptr<material> mat = entity->material_;
        std::shared_ptr<hitable> object;

        if (entity->name.find("box") != std::string::npos) {
            vec3 center = entity->position;
            vec3 size = entity->scale;
            
            vec3 half_extent = size * 0.5f;
            object = std::make_shared<box>(center - half_extent, center + half_extent, mat);
        } else {
            float sphere_radius = std::max(0.5f, entity->scale.x() * 0.5f);
            object = std::make_shared<sphere>(pos, sphere_radius, mat);
        }

        if (object) world->add(object);
    }
    return world;
}

// --- EXECUTE: The Rendering Loop ---
void RayTraceNode::execute(Scene &scene)
{
    // 1. AUTO-GENERATE FILENAME 
    // This creates output_000.ppm, output_001.ppm, etc. automatically
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "output_%03d.ppm", frame_counter);
    this->filename = std::string(buffer);
    
    // Increment for next time
    frame_counter++; 

    std::cout << "Rendering Frame " << frame_counter << " to " << filename << "..." << std::endl;

    // SNAPSHOT DIMENSIONS
    // create local const variables. We use THESE for the rest of the function.
    // not using 'this->height' again, in case the window resizes mid-render.
    const int frame_width = this->width;
    const int frame_height = this->height;

    // Setup World & Camera
    std::shared_ptr<hitable_list> world = convertSceneToRT(scene);
    CameraData &cam = scene.cameraSettings;

   
    camera cam_obj(cam.look_from, cam.look_at, vec3(0, 1, 0), cam.vfov, 
                   float(frame_width) / float(frame_height),
                   cam.aperture, cam.focus_dist);

    // Buffer Setup
    std::vector<vec3> image_buffer(frame_width * frame_height);

    ThreadPool& pool = GlobalPool::get();

    std::atomic<int> jobs_remaining(0);
    int tile_size = 32;

    // Tiled Loop
    for (int j = frame_height - 1; j >= 0; j -= tile_size)
    {
        for (int i = 0; i < frame_width; i += tile_size)
        {
            jobs_remaining++;
            
                   
            pool.enqueue([this, i, j, world, cam_obj, &image_buffer, &jobs_remaining, tile_size, frame_height, frame_width]()mutable
            {
                int end_x = std::min(i + tile_size, frame_width);
                int start_y = std::max(j - tile_size + 1, 0); 
                int end_y = j + 1;

                for (int y = end_y - 1; y >= start_y; y--)
                {
                    for (int x = i; x < end_x; x++)
                    {
                        vec3 col(0, 0, 0);
                        for (int s = 0; s < samples; s++)
                        {
                            float u = float(x + random_double()) / float(frame_width);
                            float v = float(y + random_double()) / float(frame_height);
                            
                            ray r = cam_obj.get_ray(u, v);
                            col += get_ray_color(r, world, 0);
                        }
                        col /= float(samples);
                        col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2])); 

                        int index = (frame_height - 1 - y) * frame_width + x;

                        if (index >= 0 && static_cast<size_t> (index) < image_buffer.size()) {
                            image_buffer[index] = col;
                        } else {
                            printf("Bounds Error: %d\n", index);
                        }
                    }
                } 
                jobs_remaining--; 
            });
        }
    }

    // Wait Barrier
    while (jobs_remaining > 0) {
        std::this_thread::yield();
    }

    // Write File
    std::ofstream outfile(filename);
    outfile << "P3\n" << frame_width << " " << frame_height << "\n255\n";

    for (const auto &col : image_buffer)
    {
        int ir = int(255.99 * col[0]);
        int ig = int(255.99 * col[1]);
        int ib = int(255.99 * col[2]);


        outfile << ir << " " << ig << " " << ib << "\n";
    }

    outfile.close();
    std::cout << "Done! Saved to " << filename << std::endl;
}