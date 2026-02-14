#pragma once

#include "hitable.h"

class hitable_list : public hitable {
public:
    hitable_list() {}

    std::vector<std::shared_ptr<hitable>> objects;
    
    void add(std::shared_ptr<hitable> object){
        objects.push_back(object);
    }

    void clearScene() {
        objects.clear();
    }

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
    
    
};

inline bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    
    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}