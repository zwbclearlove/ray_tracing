//
// Created by atwbzhang on 2024/7/10.
//
#pragma once
#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable {
  public:
    Sphere() {}
    Sphere(const Point3& center, double radius) : center_(center), radius_(fmax(0, radius)) {}

    const Point3 center() const { return center_; }
    const double radius() const { return radius_; }
    
    bool hit(const Ray& r, Interval ray_t, HitRecord& record) const override {
        Vec3 oc = center_ - r.origin();
        auto a = r.direction().length_squared();
        auto b_2 = dot(oc, r.direction());
        auto c = oc.length_squared() - radius_ * radius_;
        auto discriminant = b_2 * b_2 - a * c;
        
        if (discriminant < 0) {
            return false;
        }

        auto sqrt_discriminant = sqrt(discriminant);
        auto root = (b_2 - sqrt_discriminant) / a;
        if (!ray_t.surrounds(root)) {
            root = (b_2 + sqrt_discriminant) / a;
            if (!ray_t.surrounds(root)) {
                return false;
            }
        }

        record.t = root;
        record.p = r.at(root);
        Vec3 outward_normal = (record.p - center_) / radius_;
        record.set_face_normal(r, outward_normal);

        return true;
    }
  
  private:
    Point3 center_;
    double radius_;
};