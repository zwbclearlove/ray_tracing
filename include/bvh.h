//
// Created by atwbzhang on 2024/7/13.
//
#pragma once
#include <vector>
#include <algorithm>
#include <memory>

#include "common.h"
#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"

class BVHNode : public Hittable {
public:
    BVHNode(HittableList& list) : BVHNode(list.objects(), 0, list.objects().size()) {

    }

    BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end) {
        int axis = random_int(0, 2);

        auto comparator = (axis == 0) ? box_x_compare
                        : (axis == 1) ? box_y_compare
                        : box_z_compare;

        size_t object_span = end - start;
        if (object_span == 1) {
            left_ = right_ = objects[start];
        } else if (object_span == 2) {
            left_ = objects[start];
            right_ = objects[start + 1];
        } else {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            auto mid = start + object_span / 2;
            left_ = std::make_shared<BVHNode>(objects, start, mid);
            right_ = std::make_shared<BVHNode>(objects, mid, end);
        }

        bbox_ = AABB(left_->bounding_box(), right_->bounding_box());
    }

    bool hit(const Ray& r, Interval ray_t, HitRecord& record) const override {
        if (!bbox_.hit(r, ray_t)) {
            return false;
        }

        bool hit_left = left_->hit(r, ray_t, record);
        bool hit_right = right_->hit(r, Interval(ray_t.min(), hit_left ? record.t : ray_t.max()), record); 

        return hit_left || hit_right;
    }

    AABB bounding_box() const override {
        return bbox_;
    }

private:
    std::shared_ptr<Hittable> left_;
    std::shared_ptr<Hittable> right_;
    AABB bbox_;

    static bool box_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis) {
        return a->bounding_box().axis_interval(axis).min() < b->bounding_box().axis_interval(axis).min(); 
    }

    static bool box_x_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
        return box_compare(a, b, 2);
    }
};