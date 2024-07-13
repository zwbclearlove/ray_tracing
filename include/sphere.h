//
// Created by atwbzhang on 2024/7/10.
//
#pragma once
#include "common.h"
#include "hittable.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"
#include "material.h"

class Sphere : public Hittable {
  public:
    Sphere() {}

    // Stationary sphere.
    Sphere(const Point3& center, double radius, std::shared_ptr<Material> material) 
        : center_(center), radius_(ffmax(0, radius)), material_ptr_(material), is_moving_(false) {
        auto rvec = Vec3(radius, radius, radius);
        bbox_ = AABB(center - rvec, center + rvec);
    }
    // Moving sphere.
    Sphere(const Point3& center1, const Point3& center2, double radius, std::shared_ptr<Material> material)
        : center_(center1), radius_(ffmax(0, radius)), material_ptr_(material), is_moving_(true) {
        auto rvec = Vec3(radius, radius, radius);
        AABB box1(center1 - rvec, center1 + rvec);
        AABB box2(center2 - rvec, center2 + rvec);
        bbox_ = AABB(box1, box2);

        center_vec_ = center2 - center1;
    }

    const Point3 center() const { return center_; }
    const double radius() const { return radius_; }
    
    bool hit(const Ray& r, Interval ray_t, HitRecord& record) const override {
        Point3 center = is_moving_ ? sphere_center(r.time()) : center_;
        Vec3 oc = center - r.origin();
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
        Vec3 outward_normal = (record.p - center) / radius_;
        record.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, record.u, record.v);
        record.material_ptr = material_ptr_;

        return true;
    }

    AABB bounding_box() const override {
        return bbox_;
    }
  
  private:
    Point3 center_;
    double radius_;
    std::shared_ptr<Material> material_ptr_;
    bool is_moving_;
    Vec3 center_vec_;
    AABB bbox_;

    Point3 sphere_center(double time) const {
        return center_ + center_vec_ * time;
    }

    static void get_sphere_uv(const Point3& p, double& u, double& v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>
        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + kPi;
        
        u = phi / (2 * kPi);
        v = theta / kPi;
    }
};