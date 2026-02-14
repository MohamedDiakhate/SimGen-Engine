#pragma once

#include "ray.h"
#include "hitable.h"
#include "core/types.h"

class material {
public:
    virtual ~material() = default;

    // Core function: How does light bounce off this surface?
    virtual bool scatter(const ray&, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;

    // For lights: How much light does this surface generate?
    virtual vec3 emitted(float, float, const vec3&) const {
        return vec3(0, 0, 0);
    }

    // This allows the Entity to update the material's color dynamically.
    // The base implementation does nothing (useful for Glass/Dielectric which has no color).
    virtual void set_color(const vec3&) {} 
};

// --- LIGHT SOURCE ---
class diffuse_light : public material {
public:
    vec3 emit;

    diffuse_light(vec3 c) : emit(c) {}

    virtual bool scatter(const ray&, const hit_record&, vec3&, ray&) const override {
        return false; 
    }

    virtual vec3 emitted(float, float, const vec3&) const override {
        return emit;
    }

    //Update the light color
    void set_color(const vec3& c) override {
        emit = c;
    }
};

// --- MATTE ---
class lambertian : public material {
public:
    vec3 albedo;

    lambertian(const vec3& a) : albedo(a) {}

    virtual bool scatter(const ray&, const hit_record& rec, vec3& attenuation, ray& scattered) const override {
        vec3 target = rec.p + rec.normal + random_unit_sphere();
        scattered = ray(rec.p, target - rec.p);
        attenuation = albedo;
        return true;
    }

    //Update the matte color
    void set_color(const vec3& c) override {
        albedo = c;
    }
};

// --- METAL (Shiny) ---
class metal : public material {
public:
    vec3 albedo;
    float fuzz;

    metal(const vec3& a, float f) : albedo(a) { 
        if (f < 1) fuzz = f; else fuzz = 1; 
    }

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    //Update the metal reflection color
    void set_color(const vec3& c) override {
        albedo = c;
    }
};

// --- GLASS (Dielectric) ---
class dielectric : public material {
public:
    float ref_idx;

    dielectric(float ri) : ref_idx(ri) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0); // Glass is usually clear/white
        vec3 refracted;
        float reflect_prob;
        float cosine;

        if (dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
        } else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }

        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
            reflect_prob = schlick(cosine, ref_idx);
        } else {
            reflect_prob = 1.0;
        }

        if (random_double() < reflect_prob) {
            scattered = ray(rec.p, reflected);
        } else {
            scattered = ray(rec.p, refracted);
        }

        return true;
    }

    //We do not override set_color here. 
    // The base implementation of set_color (do nothing) is used because clear glass has no "color" to change.
};