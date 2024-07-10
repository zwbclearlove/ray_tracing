//
// Created by atwbzhang on 2024/7/10.
//
#pragma once
#include "common.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"
#include "material.h"

class Material;

typedef struct HitRecord {
    Vec3 p;
    Vec3 normal;
    std::shared_ptr<Material> material_ptr;
    double t;
    bool front_face;


    inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
} HitRecord;

class Hittable {
  public:
    virtual bool hit(const Ray& r, Interval ray_t, HitRecord& record) const = 0;
};


