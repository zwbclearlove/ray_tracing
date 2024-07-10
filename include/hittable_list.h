//
// Created by atwbzhang on 2024/7/10.
//
#pragma once
#include <vector>
#include <memory>

#include "hittable.h"

class HittableList : public Hittable {
  public:
    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects_.clear(); }
    void add(std::shared_ptr<Hittable> object) { objects_.emplace_back(object); }

    virtual bool hit(const Ray& r, Interval ray_t, HitRecord& record) const {
        HitRecord temp_rec;
        bool hit_anything = false;
        double closest_so_far = ray_t.max();

        for (const auto& object : objects_) {
            if (object->hit(r, Interval(ray_t.min(), closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                record = temp_rec;
            }
        }

        return hit_anything;
    }

  private:
    std::vector<std::shared_ptr<Hittable>> objects_;
};