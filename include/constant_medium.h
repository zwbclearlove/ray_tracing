//
// Created by atwbzhang on 2024/7/14.
//
#pragma once
#include "common.h"

#include "hittable.h"
#include "many_materials.h"
#include "texture.h"


class ConstantMedium : public Hittable {
  public:
    ConstantMedium(std::shared_ptr<Hittable> boundary, double density, std::shared_ptr<Texture> texture)
    : boundary_(boundary), neg_inv_density_(-1.0 / density), 
    phase_function_(std::make_shared<Isotropic>(texture)) {}

    ConstantMedium(std::shared_ptr<Hittable> boundary, double density, const Color& color)
    : boundary_(boundary), neg_inv_density_(-1.0 / density),
    phase_function_(std::make_shared<Isotropic>(color)) {}

    bool hit(const Ray& r, Interval ray_t, HitRecord& record) const override {
        // Print occasional samples when debugging. To enable, set enableDebug true.
        const bool enableDebug = false;
        const bool debugging = enableDebug && random_double() < 0.00001;

        HitRecord rec1, rec2;

        if (!boundary_->hit(r, Interval::universe, rec1)) {
            return false;
        }

        if (!boundary_->hit(r, Interval(rec1.t + 0.0001, kInfinity), rec2)) {
            return false;
        }

        if (debugging) {
            std::clog << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << std::endl;
        }
        
        rec1.t = ffmax(rec1.t, ray_t.min());
        rec2.t = ffmin(rec2.t, ray_t.max());

        if (rec1.t >= rec2.t) {
            return false;
        }

        rec1.t = ffmax(rec1.t, 0.0);

        const auto ray_length = r.direction().length();
        const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
        const auto hit_distance = neg_inv_density_ * log(random_double());

        if (hit_distance > distance_inside_boundary) {
            return false;
        }

        record.t = rec1.t + hit_distance / ray_length;
        record.p = r.at(record.t);

        if (debugging) {
            std::clog << "hit_distance = " << hit_distance << std::endl;
            std::clog << "record.t = " << record.t << std::endl;
            std::clog << "record.p = " << record.p << std::endl;
        }

        record.normal = Vec3(1, 0, 0); // arbitrary
        record.front_face = true; // also arbitrary
        record.material_ptr = phase_function_;

        return true;
    }

    AABB bounding_box() const override {
        return boundary_->bounding_box();
    }

  private:
    std::shared_ptr<Hittable> boundary_;
    double neg_inv_density_;
    std::shared_ptr<Material> phase_function_;
    // AABB bbox_;

};