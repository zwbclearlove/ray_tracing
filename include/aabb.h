//
// Created by atwbzhang on 2024/7/13.
//
#pragma once
#include "common.h"
#include "interval.h"
#include "vec3.h"
#include "ray.h"

class AABB {
  public:
    AABB() {}

    AABB(const Interval& x, const Interval& y, const Interval& z) : x_(x), y_(y), z_(z) {
        pad_to_minimums();
    }

    AABB(const Point3& a, const Point3& b) {
        x_ = (a.x() <= b.x()) ? Interval(a.x(), b.x()) : Interval(b.x(), a.x());
        y_ = (a.y() <= b.y()) ? Interval(a.y(), b.y()) : Interval(b.y(), a.y());
        z_ = (a.z() <= b.z()) ? Interval(a.z(), b.z()) : Interval(b.z(), a.z());
        pad_to_minimums();
    }

    AABB(const AABB& box0, const AABB& box1) {
        x_ = Interval(box0.axis_interval(0), box1.axis_interval(0));
        y_ = Interval(box0.axis_interval(1), box1.axis_interval(1));
        z_ = Interval(box0.axis_interval(2), box1.axis_interval(2));
    }

    const Interval& x() const { return x_; }
    const Interval& y() const { return y_; }
    const Interval& z() const { return z_; }

    const Interval& axis_interval(int n) const {
        if (n == 0) {
            return x_;
        } else if (n == 1) {
            return y_;
        } else {
            return z_;
        }
    }

    bool hit(const Ray& r, Interval ray_t) const {
        const Point3& origin = r.origin();
        const Vec3& direction = r.direction();

        for (int axis = 0; axis < 3; ++axis) {
            const Interval& ax = axis_interval(axis);
            const double adinv = 1.0 / direction[axis];

            auto t0 = (ax.min() - origin[axis]) * adinv;
            auto t1 = (ax.max() - origin[axis]) * adinv;

            if (t0 < t1) {
                if (t0 > ray_t.min()) {
                    ray_t.set_min(t0);
                }
                if (t1 < ray_t.max()) {
                    ray_t.set_max(t1);
                }
            } else {
                if (t1 > ray_t.min()) {
                    ray_t.set_min(t1);
                }
                if (t0 < ray_t.max()) {
                    ray_t.set_max(t0);
                }
            }

            if (ray_t.max() <= ray_t.min()) {
                return false;
            }
        }

        return true;
    }

    int longest_axis() const {
        if (x_.size() > y_.size() && x_.size() > z_.size()) {
            return 0;
        } else if (y_.size() > z_.size()) {
            return 1;
        } else {
            return 2;
        }
    }

    static const AABB empty, universe;

  private:
    Interval x_, y_, z_;

    void pad_to_minimums() {
        // Adjust the AABB so that no side is narrower than some delta, padding if necessary.

        double delta = 0.0001;
        if (x_.size() < delta) {
            x_ = x_.expand(delta);
        }
        if (y_.size() < delta) {
            y_ = y_.expand(delta);
        }
        if (z_.size() < delta) {
            z_ = z_.expand(delta);
        }
    }
};

const AABB AABB::empty(Interval::empty, Interval::empty, Interval::empty);
const AABB AABB::universe(Interval::universe, Interval::universe, Interval::universe);

AABB operator+(const AABB& bbox, const Vec3& offset) {
    return AABB(bbox.x() + offset.x(),
                bbox.y() + offset.y(),
                bbox.z() + offset.z());
}

AABB operator+(const Vec3& offset, const AABB& bbox) {
    return bbox + offset;
}