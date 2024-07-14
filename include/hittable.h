//
// Created by atwbzhang on 2024/7/10.
//
#pragma once
#include "common.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"
#include "material.h"
// #include "many_materials.h"
#include "aabb.h"
#include "texture.h"

class Material;

struct HitRecord {
    Vec3 p;
    Vec3 normal;
    std::shared_ptr<Material> material_ptr;
    double t;
    double u;
    double v;
    bool front_face;


    inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
  public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& r, Interval ray_t, HitRecord& record) const = 0;

    virtual AABB bounding_box() const = 0;
};

class Translate : public Hittable {
  public:
    Translate(std::shared_ptr<Hittable> object, const Vec3& offset)
        : object_(object), offset_(offset) {
        bbox_ = object->bounding_box() + offset;
    }

    bool hit(const Ray& r, Interval ray_t, HitRecord& record) const override {
        // Move the ray backwards by the offset.
        Ray offset_r(r.origin() - offset_, r.direction(), r.time());

        // Determin whether an intersection exists along the offset ray.
        if (!object_->hit(offset_r, ray_t, record)) {
            return false;
        }

        // Move the intersection point forward by the offset.
        record.p += offset_;

        return true;
    }

    AABB bounding_box() const override {
        return bbox_;
    }

  private:
    std::shared_ptr<Hittable> object_;
    Vec3 offset_;
    AABB bbox_;
};

class RotateY : public Hittable {
  public:
    RotateY(std::shared_ptr<Hittable> object, double angle) : object_(object) {
        auto radians = degrees2radians(angle);
        sin_theta_ = sin(radians);
        cos_theta_ = cos(radians);
        bbox_ = object->bounding_box();

        Point3 min(kInfinity, kInfinity, kInfinity);
        Point3 max(-kInfinity, -kInfinity, -kInfinity);

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    auto x = i * bbox_.x().max() + (1 - i) * bbox_.x().min();
                    auto y = j * bbox_.y().max() + (1 - j) * bbox_.y().min();
                    auto z = k * bbox_.z().max() + (1 - k) * bbox_.z().min();

                    auto newx = cos_theta_ * x + sin_theta_ * z;
                    auto newz = -sin_theta_ * x + cos_theta_ * z;

                    Vec3 tester(newx, y, newz);

                    for (int c = 0; c < 3; c++) {
                        min[c] = ffmin(min[c], tester[c]);
                        max[c] = ffmax(max[c], tester[c]);
                    }
                }
            }
        }

        bbox_ = AABB(min, max);
    }

    bool hit(const Ray& r, Interval ray_t, HitRecord& record) const override {
        // Change the ray from world space to object space.
        auto origin = r.origin();
        auto direction = r.direction();

        origin[0] = cos_theta_ * r.origin()[0] - sin_theta_ * r.origin()[2];
        origin[2] = sin_theta_ * r.origin()[0] + cos_theta_ * r.origin()[2];

        direction[0] = cos_theta_ * r.direction()[0] - sin_theta_ * r.direction()[2];
        direction[2] = sin_theta_ * r.direction()[0] + cos_theta_ * r.direction()[2];

        Ray rotated_r(origin, direction, r.time());

        // Determine whether an intersection exists in object space.
        if (!object_->hit(rotated_r, ray_t, record)) {
            return false;
        }

        // Change the intersection point from object space to world space.
        auto p = record.p;
        p[0] = cos_theta_ * record.p[0] + sin_theta_ * record.p[2];
        p[2] = -sin_theta_ * record.p[0] + cos_theta_ * record.p[2];

        // Change the normal vector from object space to world space.
        auto normal = record.normal;
        normal[0] = cos_theta_ * record.normal[0] + sin_theta_ * record.normal[2];
        normal[2] = -sin_theta_ * record.normal[0] + cos_theta_ * record.normal[2];

        record.p = p;
        record.normal = normal;
        return true;
    }

    AABB bounding_box() const override {
        return bbox_;
    }

  private:
    std::shared_ptr<Hittable> object_;
    double sin_theta_;
    double cos_theta_;
    AABB bbox_;
};

