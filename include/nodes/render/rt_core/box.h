#pragma once
#include "hitable.h"
#include <algorithm> // for std::swap

class box : public hitable {
public:
    box() {}
    box(vec3 p0, vec3 p1, std::shared_ptr<material> m) : pmin(p0), pmax(p1), mat_ptr(m) {}

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
        // Optimized AABB Hit Test
        for (int i = 0; i < 3; i++) {
            float invD = 1.0f / r.direction()[i];
            float t0 = (pmin[i] - r.origin()[i]) * invD;
            float t1 = (pmax[i] - r.origin()[i]) * invD;
            
            if (invD < 0.0f) std::swap(t0, t1);
            
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            
            if (t_max <= t_min) return false;
        }
        
        // If we hit, calculate details
        rec.t = t_min;
        rec.p = r.point_at_parameter(rec.t);
        rec.mat_ptr = mat_ptr;

        // Calculate Normal (Which face of the box did we hit?)
        vec3 center = (pmin + pmax) * 0.5f;
        vec3 size = (pmax - pmin) * 0.5f;
        vec3 pc = rec.p - center;
        
        // Check which axis is closest to the surface
        float bias = 1.0001f;
        rec.normal = vec3(0,0,0);
        
        if (std::abs(pc.x()) >= size.x() / bias) rec.normal.e[0] = (pc.x() > 0) ? 1 : -1;
        else if (std::abs(pc.y()) >= size.y() / bias) rec.normal.e[1] = (pc.y() > 0) ? 1 : -1;
        else if (std::abs(pc.z()) >= size.z() / bias) rec.normal.e[2] = (pc.z() > 0) ? 1 : -1;
        
        return true;
    }

    vec3 pmin, pmax;
    std::shared_ptr<material> mat_ptr;
};