//
// Created by atwbzhang on 2024/7/14.
//
#pragma once
#include "common.h"
#include "hittable.h"


class Quad : public Hittable {
  public:
    Quad(const Point3& Q, const Vec3& u, const Vec3& v, const std::shared_ptr<Material>& material_ptr) 
    : Q_(Q), u_(u), v_(v), material_ptr_(material_ptr) {
        auto n = cross(u_, v_);
        normal_ = unit_vector(n);
        D_ = dot(Q_, normal_);
        w_ = n / dot(n, n);
        set_bounding_box();
    }

    virtual void set_bounding_box() {
        auto bbox1 = AABB(Q_, Q_ + u_ + v_);
        auto bbox2 = AABB(Q_ + u_, Q_ + v_);
        bbox_ = AABB(bbox1, bbox2);
    }

    AABB bounding_box() const override {
        return bbox_;
    }

    bool hit(const Ray& r, Interval ray_t, HitRecord& record) const override {
        auto denom = dot(r.direction(), normal_);

        // No hit if the ray is parallel to the plane.
        if (fabs(denom) < 1e-6) {
            return false;
        }

        // Return false if the hit point para t is outside the ray interval.
        auto t = (D_ - dot(r.origin(), normal_)) / denom;
        if (!ray_t.contains(t)) {
            return false;
        }

        // Determine the hit point lies within the planar shape using its plane coordinates.
        auto intersetion = r.at(t);
        Vec3 planar_hitpt_vector = intersetion - Q_;
        auto alpha = dot(w_, cross(planar_hitpt_vector, v_));
        auto beta = dot(w_, cross(u_, planar_hitpt_vector));

        if (!is_interior(alpha, beta, record)) {
            return false;
        }

        record.t = t;
        record.p = intersetion;
        record.material_ptr = material_ptr_;
        record.set_face_normal(r, normal_);

        return true;
    }

    virtual bool is_interior(double a, double b, HitRecord& record) const {
        Interval unit_interval(0, 1);
        if (!unit_interval.contains(a) || !unit_interval.contains(b)) {
            return false;
        }

        record.u = a;
        record.v = b;
        return true;
    }

  private:
    Point3 Q_;
    Vec3 u_, v_;
    Vec3 w_;
    std::shared_ptr<Material> material_ptr_;
    AABB bbox_;
    Vec3 normal_;
    double D_;
};
